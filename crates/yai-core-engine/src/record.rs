#[derive(Clone, Debug, Eq, PartialEq)]
pub enum RecordKind {
    Case,
    SubjectBinding,
    Attempt,
    Decision,
    Receipt,
    Projection,
}

impl RecordKind {
    pub fn as_str(&self) -> &'static str {
        match self {
            Self::Case => "case",
            Self::SubjectBinding => "subject_binding",
            Self::Attempt => "attempt",
            Self::Decision => "decision",
            Self::Receipt => "receipt",
            Self::Projection => "projection",
        }
    }

    pub fn from_str(value: &str) -> Option<Self> {
        match value {
            "case" => Some(Self::Case),
            "subject_binding" => Some(Self::SubjectBinding),
            "attempt" => Some(Self::Attempt),
            "decision" => Some(Self::Decision),
            "receipt" => Some(Self::Receipt),
            "projection" => Some(Self::Projection),
            _ => None,
        }
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Record {
    pub id: String,
    pub case_ref: String,
    pub kind: RecordKind,
    pub subject_ref: String,
    pub attempt_id: String,
    pub decision_id: String,
    pub receipt_id: String,
    pub summary: String,
}

impl Record {
    pub fn new(id: impl Into<String>, kind: RecordKind, summary: impl Into<String>) -> Self {
        Self {
            id: id.into(),
            case_ref: String::new(),
            kind,
            subject_ref: String::new(),
            attempt_id: String::new(),
            decision_id: String::new(),
            receipt_id: String::new(),
            summary: summary.into(),
        }
    }

    pub fn from_parts(
        id: impl Into<String>,
        case_ref: impl Into<String>,
        kind: RecordKind,
        subject_ref: impl Into<String>,
        attempt_id: impl Into<String>,
        decision_id: impl Into<String>,
        receipt_id: impl Into<String>,
        summary: impl Into<String>,
    ) -> Self {
        Self {
            id: id.into(),
            case_ref: case_ref.into(),
            kind,
            subject_ref: subject_ref.into(),
            attempt_id: attempt_id.into(),
            decision_id: decision_id.into(),
            receipt_id: receipt_id.into(),
            summary: summary.into(),
        }
    }

    pub fn to_jsonl(&self) -> String {
        format!(
            "{{\"schema\":\"yai.store.record.v0\",\"record_id\":\"{}\",\"case_ref\":\"{}\",\"record_kind\":\"{}\",\"subject_ref\":\"{}\",\"attempt_id\":\"{}\",\"decision_id\":\"{}\",\"receipt_id\":\"{}\",\"summary\":\"{}\"}}\n",
            escape_json(&self.id),
            escape_json(&self.case_ref),
            self.kind.as_str(),
            escape_json(&self.subject_ref),
            escape_json(&self.attempt_id),
            escape_json(&self.decision_id),
            escape_json(&self.receipt_id),
            escape_json(&self.summary)
        )
    }

    pub fn from_jsonl(line: &str) -> Option<Self> {
        let schema = extract_json_string(line, "schema")?;
        if schema != "yai.store.record.v0" {
            return None;
        }
        let kind = RecordKind::from_str(&extract_json_string(line, "record_kind")?)?;
        Some(Self::from_parts(
            extract_json_string(line, "record_id")?,
            extract_json_string(line, "case_ref").unwrap_or_default(),
            kind,
            extract_json_string(line, "subject_ref").unwrap_or_default(),
            extract_json_string(line, "attempt_id").unwrap_or_default(),
            extract_json_string(line, "decision_id").unwrap_or_default(),
            extract_json_string(line, "receipt_id").unwrap_or_default(),
            extract_json_string(line, "summary").unwrap_or_default(),
        ))
    }
}

fn escape_json(value: &str) -> String {
    value
        .replace('\\', "\\\\")
        .replace('"', "\\\"")
        .replace('\n', "\\n")
        .replace('\r', "\\r")
}

fn extract_json_string(line: &str, key: &str) -> Option<String> {
    let pattern = format!("\"{key}\":\"");
    let start = line.find(&pattern)? + pattern.len();
    let mut result = String::new();
    let mut chars = line[start..].chars();
    while let Some(ch) = chars.next() {
        match ch {
            '"' => return Some(result),
            '\\' => {
                let escaped = chars.next()?;
                match escaped {
                    'n' => result.push('\n'),
                    'r' => result.push('\r'),
                    other => result.push(other),
                }
            }
            other => result.push(other),
        }
    }
    None
}
