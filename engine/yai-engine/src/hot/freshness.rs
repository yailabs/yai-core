#[derive(Clone, Debug, Eq, PartialEq)]
pub enum HotFreshness {
    Fresh,
    Stale,
    Unknown,
    Rebuilding,
}

impl HotFreshness {
    pub fn as_str(&self) -> &'static str {
        match self {
            Self::Fresh => "fresh",
            Self::Stale => "stale",
            Self::Unknown => "unknown",
            Self::Rebuilding => "rebuilding",
        }
    }
}

#[derive(Clone, Debug, Eq, PartialEq)]
pub enum HotStaleReason {
    None,
    NewReceiptAfterProjection,
    NewDecisionAfterProjection,
    NewAuthorityScopeAfterProjection,
    NewMemoryAfterProjection,
    NewDivergenceAfterProjection,
    ThreadChanged,
    ManualRefreshRequired,
    Unknown,
}

impl HotStaleReason {
    pub fn as_str(&self) -> &'static str {
        match self {
            Self::None => "none",
            Self::NewReceiptAfterProjection => "new_receipt_after_projection",
            Self::NewDecisionAfterProjection => "new_decision_after_projection",
            Self::NewAuthorityScopeAfterProjection => "new_authority_scope_after_projection",
            Self::NewMemoryAfterProjection => "new_memory_after_projection",
            Self::NewDivergenceAfterProjection => "new_divergence_after_projection",
            Self::ThreadChanged => "thread_changed",
            Self::ManualRefreshRequired => "manual_refresh_required",
            Self::Unknown => "unknown",
        }
    }
}
