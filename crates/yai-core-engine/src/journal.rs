use crate::record::Record;

#[derive(Default)]
pub struct Journal {
    records: Vec<Record>,
}

impl Journal {
    pub fn new() -> Self {
        Self {
            records: Vec::new(),
        }
    }

    pub fn append(&mut self, record: Record) {
        self.records.push(record);
    }

    pub fn count(&self) -> usize {
        self.records.len()
    }

    pub fn records(&self) -> &[Record] {
        &self.records
    }
}
