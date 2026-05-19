#include "yai/control/decision.h"

#include <stddef.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);
int yai_string_contains(const char *haystack, const char *needle);

static int yai_attempt_is_read_like(const yai_op_attempt_t *attempt) {
    return yai_string_contains(attempt->effect_class, "read") ||
           yai_string_contains(attempt->effect_class, "observe") ||
           yai_string_contains(attempt->mutability, "read") ||
           yai_string_contains(attempt->mutability, "immutable") ||
           yai_string_contains(attempt->mutability, "none");
}

yai_status_t yai_control_decide(const yai_op_attempt_t *attempt,
                                yai_control_decision_t *decision) {
    if (attempt == 0 || decision == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&decision->decision_id, "decision:new1");
    decision->case_ref = attempt->case_ref;
    decision->attempt_id = attempt->attempt_id;
    decision->receipt_required = 1;

    if (yai_case_ref_is_empty(&attempt->case_ref)) {
        decision->outcome = YAI_DECISION_DENY;
        yai_copy_string(decision->reason, sizeof(decision->reason), "case is empty");
        return YAI_OK;
    }

    if (yai_subject_ref_is_empty(&attempt->target_subject_ref)) {
        decision->outcome = YAI_DECISION_DENY;
        yai_copy_string(decision->reason, sizeof(decision->reason), "target subject is empty");
        return YAI_OK;
    }

    if (yai_attempt_is_read_like(attempt)) {
        decision->outcome = YAI_DECISION_ALLOW;
        yai_copy_string(decision->reason, sizeof(decision->reason), "observed/read-like attempt allowed");
        return YAI_OK;
    }

    decision->outcome = YAI_DECISION_REQUIRE_REVIEW;
    yai_copy_string(decision->reason, sizeof(decision->reason), "mutative attempt requires review in NEW.1");
    return YAI_OK;
}

const char *yai_decision_outcome_string(yai_decision_outcome_t outcome) {
    switch (outcome) {
    case YAI_DECISION_ALLOW:
        return "allow";
    case YAI_DECISION_DENY:
        return "deny";
    case YAI_DECISION_DEFER:
        return "defer";
    case YAI_DECISION_OBSERVE_ONLY:
        return "observe_only";
    case YAI_DECISION_REQUIRE_REVIEW:
        return "require_review";
    case YAI_DECISION_REQUIRE_EVIDENCE:
        return "require_evidence";
    case YAI_DECISION_REQUIRE_REDACTION:
        return "require_redaction";
    case YAI_DECISION_ALLOW_WITH_CONSTRAINTS:
        return "allow_with_constraints";
    default:
        return "unknown";
    }
}
