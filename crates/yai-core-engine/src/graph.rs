#[derive(Clone, Debug, Eq, PartialEq)]
pub struct GraphEdge {
    pub from_ref: String,
    pub to_ref: String,
    pub edge_kind: String,
}

impl GraphEdge {
    pub fn new(
        from_ref: impl Into<String>,
        to_ref: impl Into<String>,
        edge_kind: impl Into<String>,
    ) -> Self {
        Self {
            from_ref: from_ref.into(),
            to_ref: to_ref.into(),
            edge_kind: edge_kind.into(),
        }
    }
}
