#[derive(Clone, Debug, Eq, PartialEq)]
pub enum RecordKind {
    Case,
    SubjectBinding,
    Attempt,
    Decision,
    Receipt,
    Projection,
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct Record {
    pub id: String,
    pub kind: RecordKind,
    pub summary: String,
}

impl Record {
    pub fn new(id: impl Into<String>, kind: RecordKind, summary: impl Into<String>) -> Self {
        Self {
            id: id.into(),
            kind,
            summary: summary.into(),
        }
    }
}
