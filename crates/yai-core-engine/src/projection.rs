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
        let subject_count = journal
            .records()
            .iter()
            .filter(|record| !record.subject_ref.is_empty() && record.subject_ref != "subject:none")
            .map(|record| record.subject_ref.as_str())
            .collect::<BTreeSet<_>>()
            .len();
        Self {
            summary: format!(
                "projection:{consumer} records:{source_record_count} receipts:{receipt_count} subjects:{subject_count}"
            ),
            consumer,
            case_ref,
            source_record_count,
            receipt_count,
            subject_count,
        }
    }
}
