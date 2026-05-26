//! YAI - LMDB record store
//!
//! Purpose:
//!   Provide durable indexed record lookup for normalized record envelopes.
//!
//! Ownership:
//!   LMDB environment opening, record writes, summary counts and record index
//!   reads.
//!
//! Boundary:
//!   Does not own journal replay/audit truth, hot-state freshness, graph truth
//!   or analytical facts.
//!
//! Status:
//!   active

use crate::journal::Journal;
use crate::record::Record;
use lmdb::{
    Cursor, Database, DatabaseFlags, Environment, EnvironmentFlags, Error, RoTransaction,
    RwTransaction, Transaction, WriteFlags,
};
use std::fs;
use std::path::{Path, PathBuf};
use std::time::{SystemTime, UNIX_EPOCH};

const MAP_SIZE: usize = 16 * 1024 * 1024;
pub const RECORD_SCHEMA: &str = "yai.record.v1";
pub const GRAPH_RELATION_SCHEMA: &str = "yai.graph_relation.v1";
pub const GRAPH_RELATION_STORE_NAME: &str = "lmdb_graph_relations_v0";

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum RecordStoreStatusKind {
    Missing,
    NotInitialized,
    Ready,
    Unavailable,
}

impl RecordStoreStatusKind {
    pub fn as_str(&self) -> &'static str {
        match self {
            Self::Missing => "missing",
            Self::NotInitialized => "not_initialized",
            Self::Ready => "ready",
            Self::Unavailable => "unavailable",
        }
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct RecordStoreStatus {
    pub path: PathBuf,
    pub backend: &'static str,
    pub status: RecordStoreStatusKind,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct RecordStoreSummary {
    pub records_total: usize,
    pub records_by_case: usize,
    pub records_by_kind: usize,
    pub records_by_subject: usize,
    pub records_by_receipt: usize,
}

pub struct LmdbRecordStore {
    env: Environment,
    records_by_id: Database,
    records_by_case: Database,
    records_by_kind: Database,
    records_by_subject: Database,
    records_by_receipt: Database,
    graph_relations_by_id: Database,
    graph_relations_by_case: Database,
    graph_relations_by_kind: Database,
    schema_meta: Database,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct StoredRecordEnvelope {
    pub raw_json: String,
    pub schema: String,
    pub record_id: String,
    pub record_kind: String,
    pub case_ref: String,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct RecordListResult {
    pub records_total: usize,
    pub records: Vec<StoredRecordEnvelope>,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct GraphRelation {
    pub relation_id: String,
    pub case_ref: String,
    pub from_ref: String,
    pub to_ref: String,
    pub edge_kind: String,
    pub from_kind: String,
    pub to_kind: String,
    pub source_record_id: String,
    pub source_record_kind: String,
    pub confidence: String,
    pub created_at_unix_ms: u128,
    pub provenance: String,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct GraphRelationListResult {
    pub relations_total: usize,
    pub relations: Vec<GraphRelation>,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct GraphMaterializeReport {
    pub relations_seen: usize,
    pub relations_written: usize,
    pub relations_duplicate: usize,
    pub relations_skipped: usize,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct RuntimeGraphNode {
    pub node_ref: String,
    pub node_kind: String,
    pub case_ref: String,
    pub source_record_ref: String,
    pub generation: usize,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct RuntimeGraphEdge {
    pub relation_id: String,
    pub edge_kind: String,
    pub from_ref: String,
    pub to_ref: String,
    pub case_ref: String,
    pub source_record_id: String,
    pub generation: usize,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct RuntimeGraphLoadResult {
    pub case_ref: String,
    pub nodes_total: usize,
    pub edges_total: usize,
    pub outgoing_index_entries: usize,
    pub incoming_index_entries: usize,
    pub generation: usize,
    pub dirty: bool,
    pub stale: bool,
    pub source: &'static str,
    pub durable_truth: &'static str,
    pub nodes: Vec<RuntimeGraphNode>,
    pub edges: Vec<RuntimeGraphEdge>,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct JournalImportReport {
    pub records_seen: usize,
    pub records_written: usize,
    pub records_duplicate: usize,
    pub records_skipped: usize,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct ReplayMetadata {
    pub replay_id: String,
    pub journal_identity: String,
    pub journal_path: String,
    pub record_schema: String,
    pub journal_schema: String,
    pub started_at: String,
    pub completed_at: String,
    pub lines_total: usize,
    pub lines_replayed: usize,
    pub records_written: usize,
    pub records_duplicate: usize,
    pub records_skipped: usize,
    pub invalid_entries: usize,
    pub unsupported_entries: usize,
    pub cursor_line: usize,
    pub status: String,
    pub compatibility: String,
}

impl LmdbRecordStore {
    pub fn open(path: impl AsRef<Path>) -> Result<Self, String> {
        let path = path.as_ref();
        fs::create_dir_all(path)
            .map_err(|error| format!("failed to create {}: {error}", path.display()))?;
        let env = Environment::new()
            .set_max_dbs(16)
            .set_map_size(MAP_SIZE)
            .open(path)
            .map_err(|error| format!("failed to open LMDB env {}: {error}", path.display()))?;
        let records_by_id = env
            .create_db(Some("records_by_id"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open records_by_id: {error}"))?;
        let records_by_case = env
            .create_db(Some("records_by_case"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open records_by_case: {error}"))?;
        let records_by_kind = env
            .create_db(Some("records_by_kind"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open records_by_kind: {error}"))?;
        let records_by_subject = env
            .create_db(Some("records_by_subject"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open records_by_subject: {error}"))?;
        let records_by_receipt = env
            .create_db(Some("records_by_receipt"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open records_by_receipt: {error}"))?;
        let graph_relations_by_id = env
            .create_db(Some("graph_relations_by_id"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open graph_relations_by_id: {error}"))?;
        let graph_relations_by_case = env
            .create_db(Some("graph_relations_by_case"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open graph_relations_by_case: {error}"))?;
        let graph_relations_by_kind = env
            .create_db(Some("graph_relations_by_kind"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open graph_relations_by_kind: {error}"))?;
        let schema_meta = env
            .create_db(Some("schema_meta"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open schema_meta: {error}"))?;
        let store = Self {
            env,
            records_by_id,
            records_by_case,
            records_by_kind,
            records_by_subject,
            records_by_receipt,
            graph_relations_by_id,
            graph_relations_by_case,
            graph_relations_by_kind,
            schema_meta,
        };
        store.ensure_schema()?;
        Ok(store)
    }

    pub fn status(path: impl AsRef<Path>) -> RecordStoreStatus {
        let path = path.as_ref().to_path_buf();
        let status = if !path.exists() {
            RecordStoreStatusKind::Missing
        } else if !path.is_dir() {
            RecordStoreStatusKind::Unavailable
        } else if !path.join("data.mdb").exists() {
            RecordStoreStatusKind::NotInitialized
        } else {
            match Self::schema_ready(&path) {
                Ok(true) => RecordStoreStatusKind::Ready,
                Ok(false) => RecordStoreStatusKind::NotInitialized,
                Err(()) => RecordStoreStatusKind::Unavailable,
            }
        };
        RecordStoreStatus {
            path,
            backend: "lmdb",
            status,
        }
    }

    pub fn append_record(&self, record: &Record, source_ref: &str) -> Result<(), String> {
        let mut txn = self
            .env
            .begin_rw_txn()
            .map_err(|error| format!("failed to start LMDB write transaction: {error}"))?;
        self.put_record(&mut txn, record, source_ref)?;
        txn.commit()
            .map_err(|error| format!("failed to commit LMDB record write: {error}"))
    }

    pub fn import_journal(&self, journal: &Journal, journal_ref: &str) -> Result<(), String> {
        self.import_journal_with_report(journal, journal_ref)
            .map(|_| ())
    }

    pub fn import_journal_with_report(
        &self,
        journal: &Journal,
        journal_ref: &str,
    ) -> Result<JournalImportReport, String> {
        let mut txn = self
            .env
            .begin_rw_txn()
            .map_err(|error| format!("failed to start LMDB journal import: {error}"))?;
        let mut report = JournalImportReport::default();
        for (index, record) in journal.records().iter().enumerate() {
            report.records_seen += 1;
            let source_ref = format!("{journal_ref}#{}", index + 1);
            if self.record_exists_txn(&txn, &record.id)? {
                report.records_duplicate += 1;
                continue;
            }
            self.put_record(&mut txn, record, &source_ref)?;
            report.records_written += 1;
        }
        txn.commit()
            .map_err(|error| format!("failed to commit LMDB journal import: {error}"))?;
        Ok(report)
    }

    pub fn put_replay_metadata(&self, metadata: &ReplayMetadata) -> Result<(), String> {
        let key = replay_metadata_key(&metadata.journal_identity);
        let value = metadata.to_json();
        let mut txn = self
            .env
            .begin_rw_txn()
            .map_err(|error| format!("failed to start LMDB replay metadata write: {error}"))?;
        txn.put(self.schema_meta, &key, &value, WriteFlags::empty())
            .map_err(|error| {
                format!(
                    "failed to write replay metadata for {}: {error}",
                    metadata.journal_identity
                )
            })?;
        txn.commit()
            .map_err(|error| format!("failed to commit LMDB replay metadata: {error}"))
    }

    pub fn replay_metadata(
        &self,
        journal_identity: &str,
    ) -> Result<Option<ReplayMetadata>, String> {
        let key = replay_metadata_key(journal_identity);
        let txn = self
            .env
            .begin_ro_txn()
            .map_err(|error| format!("failed to start LMDB replay metadata read: {error}"))?;
        match txn.get(self.schema_meta, &key) {
            Ok(value) => ReplayMetadata::from_bytes(value).map(Some),
            Err(Error::NotFound) => Ok(None),
            Err(error) => Err(format!(
                "failed to read replay metadata for {journal_identity}: {error}"
            )),
        }
    }

    pub fn summary(&self) -> Result<RecordStoreSummary, String> {
        let txn = self
            .env
            .begin_ro_txn()
            .map_err(|error| format!("failed to start LMDB read transaction: {error}"))?;
        Ok(RecordStoreSummary {
            records_total: count_entries(&txn, self.records_by_id)?,
            records_by_case: count_entries(&txn, self.records_by_case)?,
            records_by_kind: count_entries(&txn, self.records_by_kind)?,
            records_by_subject: count_entries(&txn, self.records_by_subject)?,
            records_by_receipt: count_entries(&txn, self.records_by_receipt)?,
        })
    }

    pub fn get_record_by_id(
        &self,
        record_id: &str,
    ) -> Result<Option<StoredRecordEnvelope>, String> {
        let txn = self
            .env
            .begin_ro_txn()
            .map_err(|error| format!("failed to start LMDB record read: {error}"))?;
        self.get_record_by_id_txn(&txn, record_id)
    }

    pub fn list_records_by_case(
        &self,
        case_ref: &str,
        limit: usize,
    ) -> Result<RecordListResult, String> {
        let txn = self
            .env
            .begin_ro_txn()
            .map_err(|error| format!("failed to start LMDB case index read: {error}"))?;
        let prefix = format!("record:case:{case_ref}:");
        self.list_records_by_index(&txn, self.records_by_case, &prefix, limit)
    }

    pub fn list_records_by_kind(
        &self,
        record_kind: &str,
        limit: usize,
    ) -> Result<RecordListResult, String> {
        let txn = self
            .env
            .begin_ro_txn()
            .map_err(|error| format!("failed to start LMDB kind index read: {error}"))?;
        let prefix = format!("record:kind:{record_kind}:");
        self.list_records_by_index(&txn, self.records_by_kind, &prefix, limit)
    }

    pub fn list_records_by_subject(
        &self,
        subject_ref: &str,
        limit: usize,
    ) -> Result<RecordListResult, String> {
        let txn = self
            .env
            .begin_ro_txn()
            .map_err(|error| format!("failed to start LMDB subject index read: {error}"))?;
        let prefix = format!("record:subject:{subject_ref}:");
        self.list_records_by_index(&txn, self.records_by_subject, &prefix, limit)
    }

    pub fn list_records_by_receipt(
        &self,
        receipt_ref: &str,
        limit: usize,
    ) -> Result<RecordListResult, String> {
        let txn = self
            .env
            .begin_ro_txn()
            .map_err(|error| format!("failed to start LMDB receipt index read: {error}"))?;
        let prefix = format!("record:receipt:{receipt_ref}:");
        self.list_records_by_index(&txn, self.records_by_receipt, &prefix, limit)
    }

    pub fn materialize_graph_relations_for_case(
        &self,
        case_ref: &str,
    ) -> Result<GraphMaterializeReport, String> {
        let source_records = self.list_records_by_case(case_ref, usize::MAX)?;
        let created_at_unix_ms = unix_time_ms();
        let mut candidates = Vec::new();
        let mut skipped = 0usize;
        for record in &source_records.records {
            let mut derived = derive_graph_relations(record, created_at_unix_ms, &mut skipped);
            candidates.append(&mut derived);
        }

        let mut txn = self
            .env
            .begin_rw_txn()
            .map_err(|error| format!("failed to start LMDB graph relation write: {error}"))?;
        let mut report = GraphMaterializeReport {
            relations_seen: candidates.len(),
            relations_skipped: skipped,
            ..Default::default()
        };
        for relation in candidates {
            if self.graph_relation_exists_txn(&txn, &relation.relation_id)? {
                report.relations_duplicate += 1;
                continue;
            }
            self.put_graph_relation(&mut txn, &relation)?;
            report.relations_written += 1;
        }
        txn.commit()
            .map_err(|error| format!("failed to commit LMDB graph relations: {error}"))?;
        Ok(report)
    }

    pub fn list_graph_relations_by_case(
        &self,
        case_ref: &str,
        limit: usize,
    ) -> Result<GraphRelationListResult, String> {
        let txn = self
            .env
            .begin_ro_txn()
            .map_err(|error| format!("failed to start LMDB graph relation read: {error}"))?;
        let prefix = format!("graph_relation:case:{case_ref}:");
        self.list_graph_relations_by_index(&txn, self.graph_relations_by_case, &prefix, limit)
    }

    pub fn load_runtime_graph_for_case(
        &self,
        case_ref: &str,
    ) -> Result<RuntimeGraphLoadResult, String> {
        let relations = self.list_graph_relations_by_case(case_ref, usize::MAX)?;
        let generation = 1;
        let mut nodes = Vec::new();
        let mut edges = Vec::new();
        let mut outgoing_refs: Vec<String> = Vec::new();
        let mut incoming_refs: Vec<String> = Vec::new();

        for relation in relations.relations {
            push_runtime_node(
                &mut nodes,
                RuntimeGraphNode {
                    node_ref: relation.from_ref.clone(),
                    node_kind: relation.from_kind.clone(),
                    case_ref: relation.case_ref.clone(),
                    source_record_ref: relation.source_record_id.clone(),
                    generation,
                },
            );
            push_runtime_node(
                &mut nodes,
                RuntimeGraphNode {
                    node_ref: relation.to_ref.clone(),
                    node_kind: relation.to_kind.clone(),
                    case_ref: relation.case_ref.clone(),
                    source_record_ref: relation.source_record_id.clone(),
                    generation,
                },
            );
            push_unique_string(&mut outgoing_refs, &relation.from_ref);
            push_unique_string(&mut incoming_refs, &relation.to_ref);
            edges.push(RuntimeGraphEdge {
                relation_id: relation.relation_id,
                edge_kind: relation.edge_kind,
                from_ref: relation.from_ref,
                to_ref: relation.to_ref,
                case_ref: relation.case_ref,
                source_record_id: relation.source_record_id,
                generation,
            });
        }

        Ok(RuntimeGraphLoadResult {
            case_ref: case_ref.to_string(),
            nodes_total: nodes.len(),
            edges_total: edges.len(),
            outgoing_index_entries: outgoing_refs.len(),
            incoming_index_entries: incoming_refs.len(),
            generation,
            dirty: false,
            stale: false,
            source: "graph_relations",
            durable_truth: "graph_persistence",
            nodes,
            edges,
        })
    }

    fn ensure_schema(&self) -> Result<(), String> {
        let mut txn = self
            .env
            .begin_rw_txn()
            .map_err(|error| format!("failed to start LMDB schema transaction: {error}"))?;
        txn.put(
            self.schema_meta,
            &"meta:schema",
            &RECORD_SCHEMA,
            WriteFlags::empty(),
        )
        .map_err(|error| format!("failed to write LMDB schema meta: {error}"))?;
        txn.put(
            self.schema_meta,
            &"meta:rebuild",
            &"not_started",
            WriteFlags::empty(),
        )
        .map_err(|error| format!("failed to write LMDB rebuild meta: {error}"))?;
        txn.put(
            self.schema_meta,
            &"meta:graph_relation_schema",
            &GRAPH_RELATION_SCHEMA,
            WriteFlags::empty(),
        )
        .map_err(|error| format!("failed to write LMDB graph relation schema meta: {error}"))?;
        txn.commit()
            .map_err(|error| format!("failed to commit LMDB schema meta: {error}"))
    }

    fn put_record(
        &self,
        txn: &mut RwTransaction<'_>,
        record: &Record,
        source_ref: &str,
    ) -> Result<(), String> {
        let id_key = format!("record:id:{}", record.id);
        let case_key = format!("record:case:{}:{}", record.case_ref, record.id);
        let kind_key = format!("record:kind:{}:{}", record.kind.as_str(), record.id);
        let value = record.to_record_plane_json(source_ref);
        txn.put(self.records_by_id, &id_key, &value, WriteFlags::empty())
            .map_err(|error| format!("failed to write records_by_id {}: {error}", record.id))?;
        txn.put(
            self.records_by_case,
            &case_key,
            &record.id,
            WriteFlags::empty(),
        )
        .map_err(|error| format!("failed to write records_by_case {}: {error}", record.id))?;
        txn.put(
            self.records_by_kind,
            &kind_key,
            &record.id,
            WriteFlags::empty(),
        )
        .map_err(|error| format!("failed to write records_by_kind {}: {error}", record.id))?;
        if !record.subject_ref.is_empty() && record.subject_ref != "subject:none" {
            let subject_key = format!("record:subject:{}:{}", record.subject_ref, record.id);
            txn.put(
                self.records_by_subject,
                &subject_key,
                &record.id,
                WriteFlags::empty(),
            )
            .map_err(|error| {
                format!("failed to write records_by_subject {}: {error}", record.id)
            })?;
        }
        if !record.receipt_id.is_empty() {
            let receipt_key = format!("record:receipt:{}:{}", record.receipt_id, record.id);
            txn.put(
                self.records_by_receipt,
                &receipt_key,
                &record.id,
                WriteFlags::empty(),
            )
            .map_err(|error| {
                format!("failed to write records_by_receipt {}: {error}", record.id)
            })?;
        }
        Ok(())
    }

    fn put_graph_relation(
        &self,
        txn: &mut RwTransaction<'_>,
        relation: &GraphRelation,
    ) -> Result<(), String> {
        let id_key = format!("graph_relation:id:{}", relation.relation_id);
        let case_key = format!(
            "graph_relation:case:{}:{}",
            relation.case_ref, relation.relation_id
        );
        let kind_key = format!(
            "graph_relation:kind:{}:{}",
            relation.edge_kind, relation.relation_id
        );
        let value = relation.to_json();
        txn.put(
            self.graph_relations_by_id,
            &id_key,
            &value,
            WriteFlags::empty(),
        )
        .map_err(|error| {
            format!(
                "failed to write graph_relations_by_id {}: {error}",
                relation.relation_id
            )
        })?;
        txn.put(
            self.graph_relations_by_case,
            &case_key,
            &relation.relation_id,
            WriteFlags::empty(),
        )
        .map_err(|error| {
            format!(
                "failed to write graph_relations_by_case {}: {error}",
                relation.relation_id
            )
        })?;
        txn.put(
            self.graph_relations_by_kind,
            &kind_key,
            &relation.relation_id,
            WriteFlags::empty(),
        )
        .map_err(|error| {
            format!(
                "failed to write graph_relations_by_kind {}: {error}",
                relation.relation_id
            )
        })?;
        Ok(())
    }

    fn record_exists_txn(&self, txn: &RwTransaction<'_>, record_id: &str) -> Result<bool, String> {
        let id_key = format!("record:id:{record_id}");
        match txn.get(self.records_by_id, &id_key) {
            Ok(_) => Ok(true),
            Err(Error::NotFound) => Ok(false),
            Err(error) => Err(format!(
                "failed to check records_by_id {record_id}: {error}"
            )),
        }
    }

    fn graph_relation_exists_txn(
        &self,
        txn: &RwTransaction<'_>,
        relation_id: &str,
    ) -> Result<bool, String> {
        let id_key = format!("graph_relation:id:{relation_id}");
        match txn.get(self.graph_relations_by_id, &id_key) {
            Ok(_) => Ok(true),
            Err(Error::NotFound) => Ok(false),
            Err(error) => Err(format!(
                "failed to check graph_relations_by_id {relation_id}: {error}"
            )),
        }
    }

    fn get_record_by_id_txn(
        &self,
        txn: &RoTransaction<'_>,
        record_id: &str,
    ) -> Result<Option<StoredRecordEnvelope>, String> {
        let id_key = format!("record:id:{record_id}");
        match txn.get(self.records_by_id, &id_key) {
            Ok(value) => StoredRecordEnvelope::from_bytes(value).map(Some),
            Err(Error::NotFound) => Ok(None),
            Err(error) => Err(format!("failed to read records_by_id {record_id}: {error}")),
        }
    }

    fn get_graph_relation_by_id_txn(
        &self,
        txn: &RoTransaction<'_>,
        relation_id: &str,
    ) -> Result<Option<GraphRelation>, String> {
        let id_key = format!("graph_relation:id:{relation_id}");
        match txn.get(self.graph_relations_by_id, &id_key) {
            Ok(value) => GraphRelation::from_bytes(value).map(Some),
            Err(Error::NotFound) => Ok(None),
            Err(error) => Err(format!(
                "failed to read graph_relations_by_id {relation_id}: {error}"
            )),
        }
    }

    fn list_records_by_index(
        &self,
        txn: &RoTransaction<'_>,
        db: Database,
        prefix: &str,
        limit: usize,
    ) -> Result<RecordListResult, String> {
        let mut cursor = txn
            .open_ro_cursor(db)
            .map_err(|error| format!("failed to open LMDB index cursor: {error}"))?;
        let mut result = RecordListResult::default();
        for (key, value) in cursor.iter() {
            if !key.starts_with(prefix.as_bytes()) {
                continue;
            }
            result.records_total += 1;
            if result.records.len() >= limit {
                continue;
            }
            let record_id = std::str::from_utf8(value)
                .map_err(|error| format!("invalid LMDB index record id: {error}"))?;
            if let Some(record) = self.get_record_by_id_txn(txn, record_id)? {
                result.records.push(record);
            }
        }
        Ok(result)
    }

    fn list_graph_relations_by_index(
        &self,
        txn: &RoTransaction<'_>,
        db: Database,
        prefix: &str,
        limit: usize,
    ) -> Result<GraphRelationListResult, String> {
        let mut cursor = txn
            .open_ro_cursor(db)
            .map_err(|error| format!("failed to open LMDB graph relation cursor: {error}"))?;
        let mut result = GraphRelationListResult::default();
        for (key, value) in cursor.iter() {
            if !key.starts_with(prefix.as_bytes()) {
                continue;
            }
            result.relations_total += 1;
            if result.relations.len() >= limit {
                continue;
            }
            let relation_id = std::str::from_utf8(value)
                .map_err(|error| format!("invalid LMDB graph relation id: {error}"))?;
            if let Some(relation) = self.get_graph_relation_by_id_txn(txn, relation_id)? {
                result.relations.push(relation);
            }
        }
        Ok(result)
    }

    fn schema_ready(path: &Path) -> Result<bool, ()> {
        let mut builder = Environment::new();
        builder
            .set_max_dbs(16)
            .set_map_size(MAP_SIZE)
            .set_flags(EnvironmentFlags::READ_ONLY);
        let env = builder.open(path).map_err(|_| ())?;
        let Ok(schema_meta) = env.open_db(Some("schema_meta")) else {
            return Ok(false);
        };
        let Ok(txn) = env.begin_ro_txn() else {
            return Err(());
        };
        Ok(
            matches!(txn.get(schema_meta, &"meta:schema"), Ok(value) if value == RECORD_SCHEMA.as_bytes()),
        )
    }
}

impl ReplayMetadata {
    fn to_json(&self) -> String {
        format!(
            "{{\"replay_id\":\"{}\",\"journal_identity\":\"{}\",\"journal_path\":\"{}\",\"record_schema\":\"{}\",\"journal_schema\":\"{}\",\"started_at\":\"{}\",\"completed_at\":\"{}\",\"lines_total\":{},\"lines_replayed\":{},\"records_written\":{},\"records_duplicate\":{},\"records_skipped\":{},\"invalid_entries\":{},\"unsupported_entries\":{},\"cursor_line\":{},\"status\":\"{}\",\"compatibility\":\"{}\"}}",
            escape_json(&self.replay_id),
            escape_json(&self.journal_identity),
            escape_json(&self.journal_path),
            escape_json(&self.record_schema),
            escape_json(&self.journal_schema),
            escape_json(&self.started_at),
            escape_json(&self.completed_at),
            self.lines_total,
            self.lines_replayed,
            self.records_written,
            self.records_duplicate,
            self.records_skipped,
            self.invalid_entries,
            self.unsupported_entries,
            self.cursor_line,
            escape_json(&self.status),
            escape_json(&self.compatibility)
        )
    }

    fn from_bytes(value: &[u8]) -> Result<Self, String> {
        let raw_json = std::str::from_utf8(value)
            .map_err(|error| format!("invalid LMDB replay metadata utf8: {error}"))?;
        Ok(Self {
            replay_id: json_string_field(raw_json, "replay_id").unwrap_or_default(),
            journal_identity: json_string_field(raw_json, "journal_identity").unwrap_or_default(),
            journal_path: json_string_field(raw_json, "journal_path").unwrap_or_default(),
            record_schema: json_string_field(raw_json, "record_schema").unwrap_or_default(),
            journal_schema: json_string_field(raw_json, "journal_schema").unwrap_or_default(),
            started_at: json_string_field(raw_json, "started_at").unwrap_or_default(),
            completed_at: json_string_field(raw_json, "completed_at").unwrap_or_default(),
            lines_total: json_usize_field(raw_json, "lines_total"),
            lines_replayed: json_usize_field(raw_json, "lines_replayed"),
            records_written: json_usize_field(raw_json, "records_written"),
            records_duplicate: json_usize_field(raw_json, "records_duplicate"),
            records_skipped: json_usize_field(raw_json, "records_skipped"),
            invalid_entries: json_usize_field(raw_json, "invalid_entries"),
            unsupported_entries: json_usize_field(raw_json, "unsupported_entries"),
            cursor_line: json_usize_field(raw_json, "cursor_line"),
            status: json_string_field(raw_json, "status").unwrap_or_default(),
            compatibility: json_string_field(raw_json, "compatibility").unwrap_or_default(),
        })
    }
}

impl StoredRecordEnvelope {
    fn from_bytes(value: &[u8]) -> Result<Self, String> {
        let raw_json = std::str::from_utf8(value)
            .map_err(|error| format!("invalid LMDB record envelope utf8: {error}"))?
            .to_string();
        Ok(Self {
            schema: json_string_field(&raw_json, "schema").unwrap_or_default(),
            record_id: json_string_field(&raw_json, "record_id").unwrap_or_default(),
            record_kind: json_string_field(&raw_json, "record_kind").unwrap_or_default(),
            case_ref: json_string_field(&raw_json, "case_ref").unwrap_or_default(),
            raw_json,
        })
    }
}

impl GraphRelation {
    fn to_json(&self) -> String {
        format!(
            "{{\"schema\":\"{}\",\"relation_id\":\"{}\",\"case_ref\":\"{}\",\"from_ref\":\"{}\",\"to_ref\":\"{}\",\"edge_kind\":\"{}\",\"from_kind\":\"{}\",\"to_kind\":\"{}\",\"source_record_id\":\"{}\",\"source_record_kind\":\"{}\",\"confidence\":\"{}\",\"created_at_unix_ms\":{},\"provenance\":\"{}\"}}",
            GRAPH_RELATION_SCHEMA,
            escape_json(&self.relation_id),
            escape_json(&self.case_ref),
            escape_json(&self.from_ref),
            escape_json(&self.to_ref),
            escape_json(&self.edge_kind),
            escape_json(&self.from_kind),
            escape_json(&self.to_kind),
            escape_json(&self.source_record_id),
            escape_json(&self.source_record_kind),
            escape_json(&self.confidence),
            self.created_at_unix_ms,
            escape_json(&self.provenance)
        )
    }

    fn from_bytes(value: &[u8]) -> Result<Self, String> {
        let raw_json = std::str::from_utf8(value)
            .map_err(|error| format!("invalid LMDB graph relation utf8: {error}"))?;
        Ok(Self {
            relation_id: json_string_field(raw_json, "relation_id").unwrap_or_default(),
            case_ref: json_string_field(raw_json, "case_ref").unwrap_or_default(),
            from_ref: json_string_field(raw_json, "from_ref").unwrap_or_default(),
            to_ref: json_string_field(raw_json, "to_ref").unwrap_or_default(),
            edge_kind: json_string_field(raw_json, "edge_kind").unwrap_or_default(),
            from_kind: json_string_field(raw_json, "from_kind").unwrap_or_default(),
            to_kind: json_string_field(raw_json, "to_kind").unwrap_or_default(),
            source_record_id: json_string_field(raw_json, "source_record_id").unwrap_or_default(),
            source_record_kind: json_string_field(raw_json, "source_record_kind")
                .unwrap_or_default(),
            confidence: json_string_field(raw_json, "confidence").unwrap_or_default(),
            created_at_unix_ms: json_u128_field(raw_json, "created_at_unix_ms"),
            provenance: json_string_field(raw_json, "provenance").unwrap_or_default(),
        })
    }
}

fn derive_graph_relations(
    record: &StoredRecordEnvelope,
    created_at_unix_ms: u128,
    skipped: &mut usize,
) -> Vec<GraphRelation> {
    let mut relations = Vec::new();
    let subject_ref = json_string_field(&record.raw_json, "subject_ref").unwrap_or_default();
    let attempt_id = json_string_field(&record.raw_json, "attempt_id").unwrap_or_default();
    let decision_id = json_string_field(&record.raw_json, "decision_id").unwrap_or_default();
    let receipt_id = json_string_field(&record.raw_json, "receipt_id").unwrap_or_default();

    add_relation(
        &mut relations,
        skipped,
        relation_from_record(
            record,
            "record_materializes_node",
            "record",
            &record.record_id,
            node_kind_for_record(&record.record_kind),
            &node_ref_for_record(record, &subject_ref, &attempt_id, &decision_id, &receipt_id),
            created_at_unix_ms,
        ),
    );

    if has_subject_ref(&subject_ref) {
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "subject_participates_in_case",
                "subject",
                &subject_ref,
                "case",
                &record.case_ref,
                created_at_unix_ms,
            ),
        );
    }

    if matches!(record.record_kind.as_str(), "attempt" | "carrier_request") {
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "attempt_targets_subject",
                "attempt",
                &attempt_id,
                "subject",
                &subject_ref,
                created_at_unix_ms,
            ),
        );
    }

    if record.record_kind == "decision" {
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "decision_controls_attempt",
                "decision",
                &decision_id,
                "attempt",
                &attempt_id,
                created_at_unix_ms,
            ),
        );
    }

    if matches!(
        record.record_kind.as_str(),
        "receipt" | "effect_receipt" | "filesystem_receipt"
    ) {
        let effect_ref = if attempt_id.is_empty() {
            format!("effect:{}", record.record_id)
        } else {
            format!("effect:{attempt_id}")
        };
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "receipt_records_effect",
                "receipt",
                &receipt_id,
                "effect",
                &effect_ref,
                created_at_unix_ms,
            ),
        );
    }

    if matches!(
        record.record_kind.as_str(),
        "policy_rule" | "receipt_requirement" | "authority_scope"
    ) {
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "policy_constrains_subject",
                "policy",
                &record.record_id,
                "subject",
                &subject_ref,
                created_at_unix_ms,
            ),
        );
    }

    if matches!(
        record.record_kind.as_str(),
        "projection" | "projection_request" | "projection_result"
    ) {
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "projection_exposes_record",
                "projection",
                &record.record_id,
                "record",
                &record.record_id,
                created_at_unix_ms,
            ),
        );
    }

    if record.record_kind == "model_interpretation" {
        let model_output_ref = format!("model_output:{}", record.record_id);
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "model_output_produces_interpretation",
                "model_output",
                &model_output_ref,
                "model_interpretation",
                &record.record_id,
                created_at_unix_ms,
            ),
        );
    }

    if record.record_kind == "divergence" {
        let (target_kind, target_ref) = if !receipt_id.is_empty() {
            ("receipt", receipt_id.as_str())
        } else if !decision_id.is_empty() {
            ("decision", decision_id.as_str())
        } else {
            ("attempt", attempt_id.as_str())
        };
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "divergence_describes_mismatch",
                "divergence",
                &record.record_id,
                target_kind,
                target_ref,
                created_at_unix_ms,
            ),
        );
    }

    if record.record_kind == "review_request" {
        let summary = json_string_field(&record.raw_json, "summary").unwrap_or_default();
        let review_ref = summary_token_value_or(&summary, "review_id", &record.record_id);
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "review_request_for_attempt",
                "review_request",
                &review_ref,
                "attempt",
                &attempt_id,
                created_at_unix_ms,
            ),
        );
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "review_resolution_produces_receipt",
                "review_request",
                &review_ref,
                "receipt",
                &receipt_id,
                created_at_unix_ms,
            ),
        );
    }

    if record.record_kind == "review_decision" {
        let summary = json_string_field(&record.raw_json, "summary").unwrap_or_default();
        let review_ref = summary_token_value_or(&summary, "review_id", "");
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "review_decision_resolves_request",
                "review_decision",
                &record.record_id,
                "review_request",
                &review_ref,
                created_at_unix_ms,
            ),
        );
    }

    if record.record_kind == "control_pending" {
        let summary = json_string_field(&record.raw_json, "summary").unwrap_or_default();
        let pending_ref = summary_token_value_or(&summary, "pending_id", &record.record_id);
        add_relation(
            &mut relations,
            skipped,
            relation_from_record(
                record,
                "control_pending_blocks_attempt",
                "control_pending",
                &pending_ref,
                "attempt",
                &attempt_id,
                created_at_unix_ms,
            ),
        );
    }

    relations
}

fn relation_from_record(
    record: &StoredRecordEnvelope,
    edge_kind: &str,
    from_kind: &str,
    from_ref: &str,
    to_kind: &str,
    to_ref: &str,
    created_at_unix_ms: u128,
) -> Option<GraphRelation> {
    if record.case_ref.is_empty()
        || edge_kind.is_empty()
        || from_kind.is_empty()
        || from_ref.is_empty()
        || to_kind.is_empty()
        || to_ref.is_empty()
        || to_ref == "subject:none"
    {
        return None;
    }
    Some(GraphRelation {
        relation_id: format!(
            "edge:{}:{}",
            edge_kind,
            relation_id_component(&record.record_id)
        ),
        case_ref: record.case_ref.clone(),
        from_ref: from_ref.to_string(),
        to_ref: to_ref.to_string(),
        edge_kind: edge_kind.to_string(),
        from_kind: from_kind.to_string(),
        to_kind: to_kind.to_string(),
        source_record_id: record.record_id.clone(),
        source_record_kind: record.record_kind.clone(),
        confidence: "derived".to_string(),
        created_at_unix_ms,
        provenance: "record".to_string(),
    })
}

fn add_relation(
    relations: &mut Vec<GraphRelation>,
    skipped: &mut usize,
    relation: Option<GraphRelation>,
) {
    if let Some(relation) = relation {
        relations.push(relation);
    } else {
        *skipped += 1;
    }
}

fn push_runtime_node(nodes: &mut Vec<RuntimeGraphNode>, node: RuntimeGraphNode) {
    if nodes
        .iter()
        .any(|existing| existing.node_ref == node.node_ref)
    {
        return;
    }
    nodes.push(node);
}

fn push_unique_string(values: &mut Vec<String>, value: &str) {
    if values.iter().any(|existing| existing == value) {
        return;
    }
    values.push(value.to_string());
}

fn node_ref_for_record(
    record: &StoredRecordEnvelope,
    subject_ref: &str,
    attempt_id: &str,
    decision_id: &str,
    receipt_id: &str,
) -> String {
    match record.record_kind.as_str() {
        "case" => record.case_ref.clone(),
        "subject_binding" | "subject_state" => subject_ref.to_string(),
        "attempt" | "carrier_request" => fallback_ref(attempt_id, &record.record_id),
        "decision" | "decision_basis" | "gate_result" => {
            fallback_ref(decision_id, &record.record_id)
        }
        "review_request" => {
            let summary = json_string_field(&record.raw_json, "summary").unwrap_or_default();
            summary_token_value_or(&summary, "review_id", &record.record_id)
        }
        "control_pending" => {
            let summary = json_string_field(&record.raw_json, "summary").unwrap_or_default();
            summary_token_value_or(&summary, "pending_id", &record.record_id)
        }
        "review_decision" => record.record_id.clone(),
        "receipt" | "effect_receipt" | "filesystem_receipt" => {
            fallback_ref(receipt_id, &record.record_id)
        }
        "policy_rule" | "receipt_requirement" | "authority_scope" => record.record_id.clone(),
        "projection" | "projection_request" | "projection_result" => record.record_id.clone(),
        "memory_candidate" => record.record_id.clone(),
        "model_interpretation" => record.record_id.clone(),
        "divergence" => record.record_id.clone(),
        _ => record.record_id.clone(),
    }
}

fn node_kind_for_record(record_kind: &str) -> &'static str {
    match record_kind {
        "case" => "case",
        "subject_binding" | "subject_state" => "subject",
        "attempt" => "attempt",
        "decision" | "decision_basis" | "gate_result" => "decision",
        "review_request" => "review_request",
        "review_decision" => "review_decision",
        "control_pending" => "control_pending",
        "carrier_request" => "dispatch",
        "receipt" | "effect_receipt" | "filesystem_receipt" => "receipt",
        "policy_rule" | "receipt_requirement" | "authority_scope" => "policy",
        "projection" | "projection_request" | "projection_result" => "projection",
        "memory_candidate" => "memory_candidate",
        "model_interpretation" => "model_interpretation",
        "divergence" => "divergence",
        _ => "record",
    }
}

fn fallback_ref(preferred: &str, fallback: &str) -> String {
    if preferred.is_empty() {
        fallback.to_string()
    } else {
        preferred.to_string()
    }
}

fn summary_token_value_or(summary: &str, key: &str, fallback: &str) -> String {
    let prefix = format!("{key}:");
    summary
        .split_whitespace()
        .find_map(|part| part.strip_prefix(&prefix))
        .filter(|value| !value.is_empty())
        .unwrap_or(fallback)
        .to_string()
}

fn has_subject_ref(subject_ref: &str) -> bool {
    !subject_ref.is_empty() && subject_ref != "subject:none"
}

fn relation_id_component(value: &str) -> String {
    value
        .chars()
        .map(|ch| {
            if ch.is_ascii_alphanumeric() || matches!(ch, ':' | '_' | '-' | '.') {
                ch
            } else {
                '_'
            }
        })
        .collect()
}

fn count_entries(txn: &RoTransaction<'_>, db: Database) -> Result<usize, String> {
    let mut cursor = txn
        .open_ro_cursor(db)
        .map_err(|error| format!("failed to open LMDB cursor: {error}"))?;
    Ok(cursor.iter().count())
}

fn json_string_field(content: &str, key: &str) -> Option<String> {
    let marker = format!("\"{key}\":\"");
    let start = content.find(&marker)? + marker.len();
    let rest = &content[start..];
    let end = rest.find('"')?;
    Some(rest[..end].to_string())
}

fn json_usize_field(content: &str, key: &str) -> usize {
    let marker = format!("\"{key}\":");
    let Some(start) = content.find(&marker).map(|index| index + marker.len()) else {
        return 0;
    };
    let rest = &content[start..];
    let end = rest
        .find(|ch: char| matches!(ch, ',' | '}'))
        .unwrap_or(rest.len());
    rest[..end].trim().parse::<usize>().unwrap_or(0)
}

fn json_u128_field(content: &str, key: &str) -> u128 {
    let marker = format!("\"{key}\":");
    let Some(start) = content.find(&marker).map(|index| index + marker.len()) else {
        return 0;
    };
    let rest = &content[start..];
    let end = rest
        .find(|ch: char| matches!(ch, ',' | '}'))
        .unwrap_or(rest.len());
    rest[..end].trim().parse::<u128>().unwrap_or(0)
}

fn replay_metadata_key(journal_identity: &str) -> String {
    format!("meta:replay:{journal_identity}")
}

fn escape_json(value: &str) -> String {
    value
        .replace('\\', "\\\\")
        .replace('"', "\\\"")
        .replace('\n', "\\n")
        .replace('\r', "\\r")
}

fn unix_time_ms() -> u128 {
    SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .map(|duration| duration.as_millis())
        .unwrap_or(0)
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::record::{Record, RecordKind};
    use std::time::{SystemTime, UNIX_EPOCH};

    fn temp_store_path(name: &str) -> PathBuf {
        let now = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .expect("system time before unix epoch")
            .as_nanos();
        std::env::temp_dir().join(format!("yai-{name}-{}-{now}", std::process::id()))
    }

    #[test]
    fn freeze_supports_control_carrier_and_divergence_records() {
        let path = temp_store_path("record-freeze");
        let store = LmdbRecordStore::open(&path).expect("open LMDB test store");
        let records = [
            Record::from_parts(
                "rec:freeze-attempt",
                "case:spine34-freeze",
                RecordKind::Attempt,
                "subject:filesystem-sandbox",
                "op:freeze-write",
                "",
                "",
                "attempt:file.write",
            ),
            Record::from_parts(
                "rec:freeze-decision",
                "case:spine34-freeze",
                RecordKind::Decision,
                "subject:filesystem-sandbox",
                "op:freeze-write",
                "decision:freeze-deny",
                "",
                "decision:deny",
            ),
            Record::from_parts(
                "rec:freeze-carrier-request",
                "case:spine34-freeze",
                RecordKind::CarrierRequest,
                "subject:filesystem-sandbox",
                "op:freeze-write",
                "decision:freeze-deny",
                "",
                "carrier:filesystem requested_outcome:blocked",
            ),
            Record::from_parts(
                "rec:freeze-effect-receipt",
                "case:spine34-freeze",
                RecordKind::EffectReceipt,
                "subject:filesystem-sandbox",
                "op:freeze-write",
                "decision:freeze-deny",
                "receipt:freeze-blocked",
                "receipt:blocked",
            ),
            Record::from_parts(
                "rec:freeze-divergence",
                "case:spine34-freeze",
                RecordKind::Divergence,
                "subject:filesystem-sandbox",
                "op:freeze-write",
                "decision:freeze-deny",
                "receipt:freeze-blocked",
                "divergence:none result:consistent",
            ),
        ];

        for record in &records {
            store
                .append_record(record, "spine34-freeze-test")
                .expect("append freeze record");
        }

        let summary = store.summary().expect("summary");
        assert_eq!(summary.records_total, 5);
        assert_eq!(summary.records_by_case, 5);
        assert_eq!(summary.records_by_kind, 5);
        assert_eq!(summary.records_by_subject, 5);
        assert_eq!(summary.records_by_receipt, 2);

        let divergence = store
            .list_records_by_kind("divergence", 10)
            .expect("list divergence records");
        assert_eq!(divergence.records_total, 1);
        assert_eq!(
            divergence
                .records
                .first()
                .map(|record| record.record_id.as_str()),
            Some("rec:freeze-divergence")
        );

        let receipt_records = store
            .list_records_by_receipt("receipt:freeze-blocked", 10)
            .expect("list receipt records");
        assert_eq!(receipt_records.records_total, 2);
        assert!(receipt_records
            .records
            .iter()
            .any(|record| record.record_kind == "effect_receipt"));
        assert!(receipt_records
            .records
            .iter()
            .any(|record| record.record_kind == "divergence"));

        let carrier_request = store
            .get_record_by_id("rec:freeze-carrier-request")
            .expect("get carrier request")
            .expect("carrier request present");
        assert_eq!(carrier_request.schema, RECORD_SCHEMA);
        assert_eq!(carrier_request.record_kind, "carrier_request");
        assert!(carrier_request
            .raw_json
            .contains("\"schema\":\"yai.record.v1\""));
        assert!(carrier_request
            .raw_json
            .contains("\"source\":{\"plane\":\"journal\""));

        drop(store);
        fs::remove_dir_all(path).expect("remove LMDB test store");
    }

    #[test]
    fn journal_import_report_is_idempotent() {
        let path = temp_store_path("journal-import-idempotent");
        let store = LmdbRecordStore::open(&path).expect("open LMDB test store");
        let mut journal = Journal::new();
        journal.append(Record::from_parts(
            "rec:journal-import-one",
            "case:journal-import",
            RecordKind::Receipt,
            "subject:journal-import",
            "op:journal-import",
            "decision:journal-import",
            "receipt:journal-import",
            "receipt:journal-import",
        ));
        journal.append(Record::from_parts(
            "rec:journal-import-two",
            "case:journal-import",
            RecordKind::Divergence,
            "subject:journal-import",
            "op:journal-import",
            "decision:journal-import",
            "receipt:journal-import",
            "divergence:none",
        ));

        let first = store
            .import_journal_with_report(&journal, "journal-import-test")
            .expect("first journal import");
        assert_eq!(first.records_seen, 2);
        assert_eq!(first.records_written, 2);
        assert_eq!(first.records_duplicate, 0);

        let second = store
            .import_journal_with_report(&journal, "journal-import-test")
            .expect("second journal import");
        assert_eq!(second.records_seen, 2);
        assert_eq!(second.records_written, 0);
        assert_eq!(second.records_duplicate, 2);

        let summary = store.summary().expect("summary");
        assert_eq!(summary.records_total, 2);
        assert_eq!(summary.records_by_case, 2);
        assert_eq!(summary.records_by_kind, 2);
        assert_eq!(summary.records_by_subject, 2);
        assert_eq!(summary.records_by_receipt, 2);

        drop(store);
        fs::remove_dir_all(path).expect("remove LMDB test store");
    }
}
