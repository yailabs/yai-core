#[derive(Default)]
pub struct IndexPlan {
    pub record_count: usize,
}

impl IndexPlan {
    pub fn new(record_count: usize) -> Self {
        Self { record_count }
    }
}
