//! YAI - journal model
//!
//! Purpose:
//!   Keep append/load helpers for JSONL operational residue.
//!
//! Ownership:
//!   In-engine journal collection and JSONL persistence helpers.
//!
//! Boundary:
//!   Does not own LMDB indexes, hot-state freshness or replay rebuild policy.
//!
//! Status:
//!   active

use crate::record::Record;
use std::fs;
use std::io;
use std::path::Path;

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

    pub fn from_jsonl_str(contents: &str) -> Self {
        let mut journal = Self::new();
        for line in contents.lines() {
            if let Some(record) = Record::from_jsonl(line) {
                journal.append(record);
            }
        }
        journal
    }

    pub fn load_jsonl(path: impl AsRef<Path>) -> io::Result<Self> {
        let contents = fs::read_to_string(path)?;
        Ok(Self::from_jsonl_str(&contents))
    }
}
