use crate::journal::Journal;
use crate::record::Record;
use lmdb::{
    Cursor, Database, DatabaseFlags, Environment, EnvironmentFlags, Error, RoTransaction,
    RwTransaction, Transaction, WriteFlags,
};
use std::fs;
use std::path::{Path, PathBuf};

const MAP_SIZE: usize = 16 * 1024 * 1024;
const SCHEMA: &str = "yai.record.v1";

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
}

pub struct LmdbRecordStore {
    env: Environment,
    records_by_id: Database,
    records_by_case: Database,
    records_by_kind: Database,
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

impl LmdbRecordStore {
    pub fn open(path: impl AsRef<Path>) -> Result<Self, String> {
        let path = path.as_ref();
        fs::create_dir_all(path)
            .map_err(|error| format!("failed to create {}: {error}", path.display()))?;
        let env = Environment::new()
            .set_max_dbs(8)
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
        let schema_meta = env
            .create_db(Some("schema_meta"), DatabaseFlags::empty())
            .map_err(|error| format!("failed to open schema_meta: {error}"))?;
        let store = Self {
            env,
            records_by_id,
            records_by_case,
            records_by_kind,
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
        let mut txn = self
            .env
            .begin_rw_txn()
            .map_err(|error| format!("failed to start LMDB journal import: {error}"))?;
        for (index, record) in journal.records().iter().enumerate() {
            let source_ref = format!("{journal_ref}#{}", index + 1);
            self.put_record(&mut txn, record, &source_ref)?;
        }
        txn.commit()
            .map_err(|error| format!("failed to commit LMDB journal import: {error}"))
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

    fn ensure_schema(&self) -> Result<(), String> {
        let mut txn = self
            .env
            .begin_rw_txn()
            .map_err(|error| format!("failed to start LMDB schema transaction: {error}"))?;
        txn.put(
            self.schema_meta,
            &"meta:schema",
            &SCHEMA,
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
        Ok(())
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

    fn schema_ready(path: &Path) -> Result<bool, ()> {
        let mut builder = Environment::new();
        builder
            .set_max_dbs(8)
            .set_map_size(MAP_SIZE)
            .set_flags(EnvironmentFlags::READ_ONLY);
        let env = builder.open(path).map_err(|_| ())?;
        let Ok(schema_meta) = env.open_db(Some("schema_meta")) else {
            return Ok(false);
        };
        let Ok(txn) = env.begin_ro_txn() else {
            return Err(());
        };
        Ok(matches!(txn.get(schema_meta, &"meta:schema"), Ok(value) if value == SCHEMA.as_bytes()))
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
