pub mod ffi;
pub mod graph;
pub mod index;
pub mod journal;
pub mod memory;
pub mod projection;
pub mod query;
pub mod reconcile;
pub mod record;
pub mod store;

#[cfg(test)]
mod tests {
    use crate::journal::Journal;
    use crate::projection::ProjectionSummary;
    use crate::record::{Record, RecordKind};

    #[test]
    fn append_record_and_build_projection_summary() {
        let mut journal = Journal::new();
        journal.append(Record::new(
            "record:attempt",
            RecordKind::Attempt,
            "op:file.write attempted",
        ));
        journal.append(Record::new(
            "record:receipt",
            RecordKind::Receipt,
            "receipt:blocked",
        ));

        let projection = ProjectionSummary::from_journal("operator", &journal);

        assert_eq!(journal.count(), 2);
        assert_eq!(projection.source_record_count, 2);
        assert!(projection.summary.contains("operator"));
    }

    #[test]
    fn parse_jsonl_and_build_projection_summary() {
        let record = Record::from_parts(
            "record:receipt",
            "case:new2-persistent",
            RecordKind::Receipt,
            "subject:repo-test",
            "op:file-write-test",
            "decision:new1",
            "receipt:file-write-blocked",
            "receipt:blocked",
        );
        let journal = Journal::from_jsonl_str(&record.to_jsonl());
        let projection = ProjectionSummary::from_journal("operator", &journal);

        assert_eq!(journal.count(), 1);
        assert_eq!(projection.case_ref, "case:new2-persistent");
        assert_eq!(projection.receipt_count, 1);
        assert_eq!(projection.subject_count, 1);
    }

    #[test]
    fn build_control_projection_summary() {
        let mut journal = Journal::new();
        journal.append(Record::from_parts(
            "record:rule",
            "case:new3-control",
            RecordKind::PolicyRule,
            "subject:repo-test",
            "",
            "",
            "",
            "rule:mutative_operation_requires_review",
        ));
        journal.append(Record::from_parts(
            "record:gate",
            "case:new3-control",
            RecordKind::GateResult,
            "subject:repo-test",
            "op:file-write-test",
            "",
            "",
            "gate:operation require_review",
        ));
        journal.append(Record::from_parts(
            "record:decision",
            "case:new3-control",
            RecordKind::Decision,
            "subject:repo-test",
            "op:file-write-test",
            "decision:new3",
            "",
            "decision:require_review",
        ));

        let projection = ProjectionSummary::from_journal("control", &journal);

        assert_eq!(projection.source_record_count, 3);
        assert_eq!(projection.policy_rule_count, 1);
        assert_eq!(projection.gate_count, 1);
        assert_eq!(projection.decision_count, 1);
        assert!(projection.summary.contains("rules:1"));
    }

    #[test]
    fn build_filesystem_projection_summary() {
        let mut journal = Journal::new();
        journal.append(Record::from_parts(
            "record:fs-receipt",
            "case:new4-filesystem",
            RecordKind::FilesystemReceipt,
            "subject:file-input",
            "op:fs-read",
            "decision:allow",
            "receipt:fs-read",
            "effect:fs.read status:observed hash:abc",
        ));
        journal.append(Record::from_parts(
            "record:subject-state",
            "case:new4-filesystem",
            RecordKind::SubjectState,
            "subject:file-input",
            "",
            "",
            "receipt:fs-read",
            "state:observed hash:abc",
        ));

        let projection = ProjectionSummary::from_journal("filesystem", &journal);

        assert_eq!(projection.source_record_count, 2);
        assert_eq!(projection.filesystem_receipt_count, 1);
        assert_eq!(projection.subject_state_count, 1);
        assert_eq!(projection.effect_count, 1);
    }

    #[test]
    fn build_graph_projection_summary() {
        let mut journal = Journal::new();
        journal.append(Record::from_parts(
            "record:edge-case-subject",
            "case:new5-graph",
            RecordKind::GraphEdge,
            "subject:repo-test",
            "",
            "",
            "",
            "edge:case_binds_subject from:case:new5-graph to:subject:repo-test",
        ));
        journal.append(Record::from_parts(
            "record:edge-decision-op",
            "case:new5-graph",
            RecordKind::GraphEdge,
            "subject:repo-test",
            "op:fs-write",
            "decision:new5",
            "",
            "edge:decision_controls_op from:decision:new5 to:op:fs-write",
        ));

        let graph = crate::graph::GraphSummary::from_summaries(
            journal
                .records()
                .iter()
                .map(|record| record.summary.as_str()),
        );
        let projection = ProjectionSummary::from_journal("graph", &journal);

        assert_eq!(graph.graph_edges, 2);
        assert_eq!(graph.case_binds_subject, 1);
        assert_eq!(graph.decision_controls_op, 1);
        assert_eq!(projection.graph_edge_count, 2);
    }

    #[test]
    fn build_memory_projection_summary() {
        let mut journal = Journal::new();
        journal.append(Record::from_parts(
            "record:memory-candidate",
            "case:new6-memory",
            RecordKind::MemoryCandidate,
            "subject:repo-test",
            "op:fs-write-memory",
            "decision:new6",
            "receipt:fs-write-blocked",
            "memory:decision scope:subject freshness:fresh confidence:high",
        ));

        let memory = crate::memory::MemorySummary::from_journal(&journal);
        let projection = ProjectionSummary::from_journal("memory", &journal);

        assert_eq!(memory.memory_candidates, 1);
        assert_eq!(memory.decision, 1);
        assert_eq!(projection.memory_candidate_count, 1);
        assert_eq!(projection.decision_memory_candidate_count, 1);
    }

    #[test]
    fn build_reconcile_projection_summary() {
        let mut journal = Journal::new();
        journal.append(Record::from_parts(
            "divergence:denied-but-executed",
            "case:new7-reconcile",
            RecordKind::Divergence,
            "subject:repo-test",
            "op:dangerous-write",
            "decision:deny-write",
            "receipt:executed-conflict",
            "divergence:denied_but_executed severity:critical",
        ));
        journal.append(Record::from_parts(
            "reconciliation:denied-but-executed",
            "case:new7-reconcile",
            RecordKind::Reconciliation,
            "subject:repo-test",
            "",
            "",
            "",
            "reconcile:requires_review divergence:divergence:denied-but-executed",
        ));

        let reconcile = crate::reconcile::ReconcileSummary::from_journal(&journal);
        let projection = ProjectionSummary::from_journal("reconcile", &journal);

        assert_eq!(reconcile.divergences, 1);
        assert_eq!(reconcile.reconciliations, 1);
        assert_eq!(reconcile.critical, 1);
        assert_eq!(projection.divergence_count, 1);
        assert_eq!(projection.reconciliation_count, 1);
    }

    #[test]
    fn build_hardened_projection_summary() {
        let mut journal = Journal::new();
        journal.append(Record::from_parts(
            "projection-request:operator",
            "case:new8-projection",
            RecordKind::ProjectionRequest,
            "subject:repo-test",
            "",
            "",
            "",
            "projection_request:projection-request:operator consumer:operator kind:operator_summary scope:case",
        ));
        journal.append(Record::from_parts(
            "projection-result:operator",
            "case:new8-projection",
            RecordKind::ProjectionResult,
            "subject:repo-test",
            "",
            "",
            "",
            "projection_result:projection-result:operator consumer:operator kind:operator_summary redaction:none freshness:fresh source_records:4 source_receipts:1 source_memory:1 source_divergences:0",
        ));
        journal.append(Record::from_parts(
            "projection-request:model",
            "case:new8-projection",
            RecordKind::ProjectionRequest,
            "subject:repo-test",
            "",
            "",
            "",
            "projection_request:projection-request:model consumer:model kind:model_context scope:case",
        ));
        journal.append(Record::from_parts(
            "projection-result:model",
            "case:new8-projection",
            RecordKind::ProjectionResult,
            "subject:repo-test",
            "",
            "",
            "",
            "projection_result:projection-result:model consumer:model kind:model_context redaction:summary_only freshness:fresh source_records:4 source_receipts:1 source_memory:1 source_divergences:0",
        ));

        let projection = ProjectionSummary::from_journal("projection", &journal);

        assert_eq!(projection.projection_request_count, 2);
        assert_eq!(projection.projection_result_count, 2);
        assert_eq!(projection.operator_projection_count, 2);
        assert_eq!(projection.model_projection_count, 2);
        assert_eq!(projection.limited_projection_count, 1);
    }
}
