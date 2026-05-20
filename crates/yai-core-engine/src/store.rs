use crate::journal::Journal;
use crate::projection::ProjectionSummary;
use crate::record::Record;

pub struct Store {
    journal: Journal,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct EngineSummary {
    pub records: usize,
    pub receipts: usize,
    pub graph_edges: usize,
    pub memory_candidates: usize,
    pub projections: usize,
    pub divergences: usize,
}

impl Store {
    pub fn new() -> Self {
        Self {
            journal: Journal::new(),
        }
    }

    pub fn append(&mut self, record: Record) {
        self.journal.append(record);
    }

    pub fn journal(&self) -> &Journal {
        &self.journal
    }

    pub fn from_journal(journal: Journal) -> Self {
        Self { journal }
    }

    pub fn engine_summary(&self) -> EngineSummary {
        let projection = ProjectionSummary::from_journal("engine", &self.journal);
        EngineSummary {
            records: projection.source_record_count,
            receipts: projection.receipt_count,
            graph_edges: projection.graph_edge_count,
            memory_candidates: projection.memory_candidate_count,
            projections: projection.projection_result_count,
            divergences: projection.divergence_count,
        }
    }
}

impl Default for Store {
    fn default() -> Self {
        Self::new()
    }
}
