#[derive(Clone, Debug, Eq, PartialEq)]
pub struct QueryPlan {
    pub query: String,
}

impl QueryPlan {
    pub fn new(query: impl Into<String>) -> Self {
        Self {
            query: query.into(),
        }
    }
}
