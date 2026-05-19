use crate::journal::Journal;
use crate::record::Record;

pub struct Store {
    journal: Journal,
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
}

impl Default for Store {
    fn default() -> Self {
        Self::new()
    }
}
