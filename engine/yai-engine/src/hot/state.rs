use super::freshness::{HotFreshness, HotStaleReason};
use super::snapshot::HotSnapshot;

pub const RECORD_DIRTY: u32 = 1 << 0;
pub const RECEIPT_DIRTY: u32 = 1 << 1;
pub const PROJECTION_DIRTY: u32 = 1 << 4;
pub const THREAD_DIRTY: u32 = 1 << 6;

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct HotState {
    snapshot: HotSnapshot,
}

impl HotState {
    pub fn new() -> Self {
        Self {
            snapshot: HotSnapshot::default(),
        }
    }

    pub fn set_case(&mut self, case_ref: impl Into<String>) {
        self.snapshot.case_ref = case_ref.into();
    }

    pub fn mark_receipt(&mut self, receipt_id: impl Into<String>) {
        self.snapshot.last_receipt_id = receipt_id.into();
        self.snapshot.case_version += 1;
        self.snapshot.dirty_flags |= RECORD_DIRTY | RECEIPT_DIRTY | PROJECTION_DIRTY;
        self.snapshot.projection_freshness = HotFreshness::Stale;
        self.snapshot.projection_stale_reason = HotStaleReason::NewReceiptAfterProjection;
    }

    pub fn mark_projection(&mut self, projection_id: impl Into<String>) {
        self.snapshot.previous_projection_id = self.snapshot.current_projection_id.clone();
        self.snapshot.current_projection_id = projection_id.into();
        self.snapshot.projection_freshness = HotFreshness::Fresh;
        self.snapshot.projection_stale_reason = HotStaleReason::None;
        self.snapshot.dirty_flags &= !PROJECTION_DIRTY;
    }

    pub fn switch_thread(&mut self, thread_id: impl Into<String>) {
        self.snapshot.active_thread_id = thread_id.into();
        self.snapshot.case_version += 1;
        self.snapshot.dirty_flags |= THREAD_DIRTY | PROJECTION_DIRTY;
        self.snapshot.projection_freshness = HotFreshness::Stale;
        self.snapshot.projection_stale_reason = HotStaleReason::ThreadChanged;
    }

    pub fn snapshot(&self) -> &HotSnapshot {
        &self.snapshot
    }
}

impl Default for HotState {
    fn default() -> Self {
        Self::new()
    }
}
