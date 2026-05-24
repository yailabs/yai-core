use super::freshness::{HotFreshness, HotStaleReason};

#[derive(Clone, Debug, Eq, PartialEq)]
pub struct HotSnapshot {
    pub hot_state_id: String,
    pub case_ref: String,
    pub case_session_id: String,
    pub case_context_id: String,
    pub case_session_status: String,
    pub case_world_status: String,
    pub case_context_status: String,
    pub case_version: u64,
    pub active_thread_id: String,
    pub participant_view_frame_id: String,
    pub current_projection_id: String,
    pub previous_projection_id: String,
    pub last_record_id: String,
    pub last_decision_id: String,
    pub last_receipt_id: String,
    pub last_model_interpretation_id: String,
    pub last_divergence_id: String,
    pub pending_op_count: u64,
    pub pending_obligation_count: u64,
    pub carrier_lock_count: u64,
    pub projection_freshness: HotFreshness,
    pub projection_stale_reason: HotStaleReason,
    pub dirty_flags: u32,
    pub updated_at_unix_ms: u64,
}

impl Default for HotSnapshot {
    fn default() -> Self {
        Self {
            hot_state_id: "hot:state-v0".to_string(),
            case_ref: String::new(),
            case_session_id: String::new(),
            case_context_id: String::new(),
            case_session_status: "unknown".to_string(),
            case_world_status: "unknown".to_string(),
            case_context_status: "unknown".to_string(),
            case_version: 0,
            active_thread_id: String::new(),
            participant_view_frame_id: String::new(),
            current_projection_id: String::new(),
            previous_projection_id: String::new(),
            last_record_id: String::new(),
            last_decision_id: String::new(),
            last_receipt_id: String::new(),
            last_model_interpretation_id: String::new(),
            last_divergence_id: String::new(),
            pending_op_count: 0,
            pending_obligation_count: 0,
            carrier_lock_count: 0,
            projection_freshness: HotFreshness::Unknown,
            projection_stale_reason: HotStaleReason::Unknown,
            dirty_flags: 0,
            updated_at_unix_ms: 0,
        }
    }
}
