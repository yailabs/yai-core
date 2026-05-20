use crate::journal::Journal;
use crate::record::RecordKind;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct MemorySummary {
    pub records: usize,
    pub memory_candidates: usize,
    pub operational: usize,
    pub decision: usize,
    pub subject: usize,
    pub error: usize,
    pub recovery: usize,
}

impl MemorySummary {
    pub fn from_journal(journal: &Journal) -> Self {
        let mut summary = Self {
            records: journal.count(),
            memory_candidates: 0,
            operational: 0,
            decision: 0,
            subject: 0,
            error: 0,
            recovery: 0,
        };

        for record in journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::MemoryCandidate)
        {
            summary.memory_candidates += 1;
            if record.summary.contains("memory:operational") {
                summary.operational += 1;
            } else if record.summary.contains("memory:decision") {
                summary.decision += 1;
            } else if record.summary.contains("memory:subject") {
                summary.subject += 1;
            } else if record.summary.contains("memory:error") {
                summary.error += 1;
            } else if record.summary.contains("memory:recovery") {
                summary.recovery += 1;
            }
        }

        summary
    }
}

pub fn derive_memory_note(journal: &Journal) -> String {
    format!("memory:candidate records:{}", journal.count())
}
