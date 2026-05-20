pub mod ffi;
pub mod graph;
pub mod index;
pub mod journal;
pub mod memory;
pub mod projection;
pub mod query;
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
}
