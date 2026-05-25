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

use crate::record::{Record, RecordKind};
use std::fs;
use std::io;
use std::path::Path;

pub const JOURNAL_RECORD_SCHEMA: &str = "yai.store.record.v0";

#[derive(Default)]
pub struct Journal {
    records: Vec<Record>,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum JournalEntryStatus {
    Valid,
    InvalidJson,
    InvalidSchema,
    UnsupportedKind,
    Duplicate,
    Replayed,
    SkippedWithReason,
    Unknown,
}

impl JournalEntryStatus {
    pub fn as_str(&self) -> &'static str {
        match self {
            Self::Valid => "valid",
            Self::InvalidJson => "invalid_json",
            Self::InvalidSchema => "invalid_schema",
            Self::UnsupportedKind => "unsupported_kind",
            Self::Duplicate => "duplicate",
            Self::Replayed => "replayed",
            Self::SkippedWithReason => "skipped_with_reason",
            Self::Unknown => "unknown",
        }
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct JournalReplayDiagnostic {
    pub line_number: usize,
    pub entry_status: JournalEntryStatus,
    pub record_id: String,
    pub record_kind: String,
    pub schema: String,
    pub error_code: String,
    pub error_message: String,
    pub action: String,
}

#[derive(Clone, Debug, Default, Eq, PartialEq)]
pub struct JournalInspection {
    pub lines_total: usize,
    pub valid_entries: usize,
    pub invalid_entries: usize,
    pub unsupported_entries: usize,
    pub duplicate_entries: usize,
    pub diagnostics: Vec<JournalReplayDiagnostic>,
}

impl JournalInspection {
    pub fn replay_ready(&self) -> bool {
        self.lines_total > 0
            && self.invalid_entries == 0
            && self.unsupported_entries == 0
            && self.duplicate_entries == 0
    }
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

    pub fn inspect_jsonl_str(contents: &str) -> JournalInspection {
        let mut inspection = JournalInspection::default();
        let mut seen_record_ids: Vec<String> = Vec::new();
        for (index, line) in contents.lines().enumerate() {
            inspection.lines_total += 1;
            let line_number = index + 1;
            let trimmed = line.trim();
            if trimmed.is_empty() {
                inspection.unsupported_entries += 1;
                inspection.diagnostics.push(diagnostic(
                    line_number,
                    JournalEntryStatus::SkippedWithReason,
                    "",
                    "",
                    "",
                    "blank_line",
                    "blank journal line",
                    "skip",
                ));
                continue;
            }
            let fields = match parse_json_string_fields(trimmed) {
                Ok(fields) => fields,
                Err(()) => {
                    inspection.invalid_entries += 1;
                    inspection.diagnostics.push(diagnostic(
                        line_number,
                        JournalEntryStatus::InvalidJson,
                        "",
                        "",
                        "",
                        "invalid_json",
                        "line is not a complete JSON object",
                        "report",
                    ));
                    continue;
                }
            };

            let schema = field_value(&fields, "schema").unwrap_or_default();
            let record_id = field_value(&fields, "record_id").unwrap_or_default();
            let record_kind = field_value(&fields, "record_kind").unwrap_or_default();

            if schema != JOURNAL_RECORD_SCHEMA {
                inspection.unsupported_entries += 1;
                inspection.diagnostics.push(diagnostic(
                    line_number,
                    JournalEntryStatus::InvalidSchema,
                    &record_id,
                    &record_kind,
                    &schema,
                    "invalid_schema",
                    "expected schema yai.store.record.v0",
                    "report",
                ));
                continue;
            }
            if record_id.is_empty() {
                inspection.unsupported_entries += 1;
                inspection.diagnostics.push(diagnostic(
                    line_number,
                    JournalEntryStatus::InvalidSchema,
                    "",
                    &record_kind,
                    &schema,
                    "missing_record_id",
                    "record_id is required for replay",
                    "report",
                ));
                continue;
            }
            if RecordKind::from_str(&record_kind).is_none() {
                inspection.unsupported_entries += 1;
                inspection.diagnostics.push(diagnostic(
                    line_number,
                    JournalEntryStatus::UnsupportedKind,
                    &record_id,
                    &record_kind,
                    &schema,
                    "unsupported_kind",
                    "record_kind is not supported by this parser",
                    "report",
                ));
                continue;
            }
            if seen_record_ids.iter().any(|seen| seen == &record_id) {
                inspection.duplicate_entries += 1;
                inspection.diagnostics.push(diagnostic(
                    line_number,
                    JournalEntryStatus::Duplicate,
                    &record_id,
                    &record_kind,
                    &schema,
                    "duplicate_record_id",
                    "record_id already appeared earlier in this journal",
                    "report",
                ));
                continue;
            }

            seen_record_ids.push(record_id);
            inspection.valid_entries += 1;
        }
        inspection
    }

    pub fn inspect_jsonl(path: impl AsRef<Path>) -> io::Result<JournalInspection> {
        let contents = fs::read_to_string(path)?;
        Ok(Self::inspect_jsonl_str(&contents))
    }
}

fn diagnostic(
    line_number: usize,
    entry_status: JournalEntryStatus,
    record_id: &str,
    record_kind: &str,
    schema: &str,
    error_code: &str,
    error_message: &str,
    action: &str,
) -> JournalReplayDiagnostic {
    JournalReplayDiagnostic {
        line_number,
        entry_status,
        record_id: empty_as_none(record_id),
        record_kind: empty_as_none(record_kind),
        schema: empty_as_none(schema),
        error_code: error_code.to_string(),
        error_message: error_message.to_string(),
        action: action.to_string(),
    }
}

fn empty_as_none(value: &str) -> String {
    if value.is_empty() {
        "none".to_string()
    } else {
        value.to_string()
    }
}

fn field_value(fields: &[(String, String)], key: &str) -> Option<String> {
    fields
        .iter()
        .find(|(field_key, _)| field_key == key)
        .map(|(_, value)| value.clone())
}

fn parse_json_string_fields(line: &str) -> Result<Vec<(String, String)>, ()> {
    let mut parser = JsonStringFieldParser::new(line);
    parser.parse_object()
}

struct JsonStringFieldParser<'a> {
    chars: Vec<char>,
    index: usize,
    _source: &'a str,
}

impl<'a> JsonStringFieldParser<'a> {
    fn new(source: &'a str) -> Self {
        Self {
            chars: source.chars().collect(),
            index: 0,
            _source: source,
        }
    }

    fn parse_object(&mut self) -> Result<Vec<(String, String)>, ()> {
        let mut fields = Vec::new();
        self.skip_ws();
        self.expect('{')?;
        self.skip_ws();
        if self.consume('}') {
            self.skip_ws();
            return if self.at_end() { Ok(fields) } else { Err(()) };
        }
        loop {
            self.skip_ws();
            let key = self.parse_string()?;
            self.skip_ws();
            self.expect(':')?;
            self.skip_ws();
            if self.peek() == Some('"') {
                let value = self.parse_string()?;
                fields.push((key, value));
            } else {
                self.skip_value()?;
            }
            self.skip_ws();
            if self.consume('}') {
                self.skip_ws();
                return if self.at_end() { Ok(fields) } else { Err(()) };
            }
            self.expect(',')?;
        }
    }

    fn parse_string(&mut self) -> Result<String, ()> {
        self.expect('"')?;
        let mut result = String::new();
        while let Some(ch) = self.next() {
            match ch {
                '"' => return Ok(result),
                '\\' => {
                    let escaped = self.next().ok_or(())?;
                    match escaped {
                        '"' => result.push('"'),
                        '\\' => result.push('\\'),
                        '/' => result.push('/'),
                        'b' => result.push('\u{0008}'),
                        'f' => result.push('\u{000c}'),
                        'n' => result.push('\n'),
                        'r' => result.push('\r'),
                        't' => result.push('\t'),
                        _ => return Err(()),
                    }
                }
                other => result.push(other),
            }
        }
        Err(())
    }

    fn skip_value(&mut self) -> Result<(), ()> {
        self.skip_ws();
        match self.peek() {
            Some('"') => self.parse_string().map(|_| ()),
            Some('{') => self.skip_compound('{', '}'),
            Some('[') => self.skip_compound('[', ']'),
            Some(_) => {
                while let Some(ch) = self.peek() {
                    if matches!(ch, ',' | '}' | ']') {
                        break;
                    }
                    self.index += 1;
                }
                Ok(())
            }
            None => Err(()),
        }
    }

    fn skip_compound(&mut self, open: char, close: char) -> Result<(), ()> {
        self.expect(open)?;
        let mut depth = 1usize;
        while let Some(ch) = self.next() {
            match ch {
                '"' => {
                    self.index -= 1;
                    self.parse_string()?;
                }
                current if current == open => depth += 1,
                current if current == close => {
                    depth -= 1;
                    if depth == 0 {
                        return Ok(());
                    }
                }
                _ => {}
            }
        }
        Err(())
    }

    fn skip_ws(&mut self) {
        while matches!(self.peek(), Some(' ' | '\n' | '\r' | '\t')) {
            self.index += 1;
        }
    }

    fn expect(&mut self, expected: char) -> Result<(), ()> {
        if self.consume(expected) {
            Ok(())
        } else {
            Err(())
        }
    }

    fn consume(&mut self, expected: char) -> bool {
        if self.peek() == Some(expected) {
            self.index += 1;
            true
        } else {
            false
        }
    }

    fn peek(&self) -> Option<char> {
        self.chars.get(self.index).copied()
    }

    fn next(&mut self) -> Option<char> {
        let ch = self.peek()?;
        self.index += 1;
        Some(ch)
    }

    fn at_end(&self) -> bool {
        self.index == self.chars.len()
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::record::{Record, RecordKind};

    #[test]
    fn inspect_reports_valid_duplicate_and_invalid_lines() {
        let record = Record::from_parts(
            "rec:journal-inspect",
            "case:journal-inspect",
            RecordKind::Receipt,
            "subject:repo-test",
            "op:journal-inspect",
            "decision:journal-inspect",
            "receipt:journal-inspect",
            "receipt:blocked",
        )
        .to_jsonl();
        let content = format!(
            "{}{}{{\"schema\":\"yai.store.record.v0\",\"record_id\":\"rec:bad-kind\",\"record_kind\":\"future_kind\"}}\n{{broken\n",
            record, record
        );
        let inspection = Journal::inspect_jsonl_str(&content);

        assert_eq!(inspection.lines_total, 4);
        assert_eq!(inspection.valid_entries, 1);
        assert_eq!(inspection.duplicate_entries, 1);
        assert_eq!(inspection.unsupported_entries, 1);
        assert_eq!(inspection.invalid_entries, 1);
        assert!(!inspection.replay_ready());
        assert!(inspection
            .diagnostics
            .iter()
            .any(|diagnostic| diagnostic.entry_status == JournalEntryStatus::Duplicate));
        assert!(inspection
            .diagnostics
            .iter()
            .any(|diagnostic| diagnostic.entry_status == JournalEntryStatus::InvalidJson));
    }
}
