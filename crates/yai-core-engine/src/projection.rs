use crate::journal::Journal;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct ProjectionSummary {
    pub consumer: String,
    pub summary: String,
    pub source_record_count: usize,
}

impl ProjectionSummary {
    pub fn from_journal(consumer: impl Into<String>, journal: &Journal) -> Self {
        let consumer = consumer.into();
        let source_record_count = journal.count();
        Self {
            summary: format!("projection:{consumer} records:{source_record_count}"),
            consumer,
            source_record_count,
        }
    }
}
