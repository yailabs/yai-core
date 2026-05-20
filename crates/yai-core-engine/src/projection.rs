use crate::journal::Journal;
use crate::record::RecordKind;
use std::collections::BTreeSet;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct ProjectionSummary {
    pub consumer: String,
    pub case_ref: String,
    pub summary: String,
    pub source_record_count: usize,
    pub receipt_count: usize,
    pub subject_count: usize,
    pub decision_count: usize,
    pub policy_rule_count: usize,
    pub gate_count: usize,
    pub obligation_count: usize,
    pub receipt_requirement_count: usize,
    pub filesystem_receipt_count: usize,
    pub subject_state_count: usize,
    pub effect_count: usize,
}

impl ProjectionSummary {
    pub fn from_journal(consumer: impl Into<String>, journal: &Journal) -> Self {
        let consumer = consumer.into();
        let source_record_count = journal.count();
        let case_ref = journal
            .records()
            .iter()
            .find(|record| !record.case_ref.is_empty())
            .map(|record| record.case_ref.clone())
            .unwrap_or_default();
        let receipt_count = journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::Receipt)
            .count();
        let decision_count = journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::Decision)
            .count();
        let policy_rule_count = journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::PolicyRule)
            .count();
        let gate_count = journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::GateResult)
            .count();
        let obligation_count = journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::Obligation)
            .count();
        let receipt_requirement_count = journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::ReceiptRequirement)
            .count();
        let filesystem_receipt_count = journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::FilesystemReceipt)
            .count();
        let subject_state_count = journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::SubjectState)
            .count();
        let effect_count = journal
            .records()
            .iter()
            .filter(|record| {
                matches!(
                    record.kind,
                    RecordKind::Receipt | RecordKind::EffectReceipt | RecordKind::FilesystemReceipt
                )
            })
            .count();
        let subject_count = journal
            .records()
            .iter()
            .filter(|record| !record.subject_ref.is_empty() && record.subject_ref != "subject:none")
            .map(|record| record.subject_ref.as_str())
            .collect::<BTreeSet<_>>()
            .len();
        Self {
            summary: format!(
                "projection:{consumer} records:{source_record_count} decisions:{decision_count} rules:{policy_rule_count} gates:{gate_count} obligations:{obligation_count} receipt_requirements:{receipt_requirement_count} filesystem_receipts:{filesystem_receipt_count} subject_states:{subject_state_count} effects:{effect_count}"
            ),
            consumer,
            case_ref,
            source_record_count,
            receipt_count,
            subject_count,
            decision_count,
            policy_rule_count,
            gate_count,
            obligation_count,
            receipt_requirement_count,
            filesystem_receipt_count,
            subject_state_count,
            effect_count,
        }
    }
}
