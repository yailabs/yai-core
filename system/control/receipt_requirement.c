#include "yai/control/receipt_requirement.h"

#include <stdio.h>

yai_status_t yai_receipt_requirement_from_decision(const char *requirement_id,
                                                   const yai_control_decision_t *decision,
                                                   yai_receipt_requirement_t *requirement) {
    if (requirement == 0 || requirement_id == 0 || requirement_id[0] == '\0' ||
        decision == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&requirement->requirement_id, requirement_id);
    requirement->case_ref = decision->case_ref;
    requirement->decision_id = decision->decision_id;
    requirement->required = decision->receipt_required;
    if (decision->outcome == YAI_DECISION_REQUIRE_REVIEW ||
        decision->outcome == YAI_DECISION_DENY) {
        requirement->requirement_kind = YAI_RECEIPT_REQUIREMENT_BLOCKED_RECEIPT;
    } else {
        requirement->requirement_kind = YAI_RECEIPT_REQUIREMENT_CARRIER_RECEIPT;
    }

    (void)snprintf(requirement->summary,
                   sizeof(requirement->summary),
                   "%s required:%d",
                   yai_receipt_requirement_kind_string(requirement->requirement_kind),
                   requirement->required);
    return YAI_OK;
}

const char *yai_receipt_requirement_kind_string(yai_receipt_requirement_kind_t kind) {
    switch (kind) {
    case YAI_RECEIPT_REQUIREMENT_CARRIER_RECEIPT:
        return "carrier_receipt";
    case YAI_RECEIPT_REQUIREMENT_EXTERNAL_RECEIPT:
        return "external_receipt";
    case YAI_RECEIPT_REQUIREMENT_BLOCKED_RECEIPT:
        return "blocked_receipt";
    case YAI_RECEIPT_REQUIREMENT_POST_STATE_OBSERVATION:
        return "post_state_observation";
    case YAI_RECEIPT_REQUIREMENT_HASH_DELTA:
        return "hash_delta";
    case YAI_RECEIPT_REQUIREMENT_EXIT_STATUS:
        return "exit_status";
    case YAI_RECEIPT_REQUIREMENT_MODEL_INVOCATION_METADATA:
        return "model_invocation_metadata";
    case YAI_RECEIPT_REQUIREMENT_OPERATOR_ACK:
        return "operator_ack";
    default:
        return "unknown";
    }
}
