#include "yai/effect/receipt.h"

#include <stddef.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_effect_receipt_from_decision(const char *receipt_id,
                                              const yai_op_attempt_t *attempt,
                                              const yai_control_decision_t *decision,
                                              yai_effect_receipt_t *receipt) {
    if (receipt == 0 || receipt_id == 0 || receipt_id[0] == '\0' ||
        attempt == 0 || decision == 0) {
        return YAI_ERR_INVALID;
    }
    yai_id_set(&receipt->receipt_id, receipt_id);
    receipt->case_ref = attempt->case_ref;
    receipt->attempt_id = attempt->attempt_id;
    receipt->decision_id = decision->decision_id;
    receipt->target_subject_ref = attempt->target_subject_ref;

    if (decision->outcome == YAI_DECISION_ALLOW ||
        decision->outcome == YAI_DECISION_OBSERVE_ONLY ||
        decision->outcome == YAI_DECISION_ALLOW_WITH_CONSTRAINTS) {
        receipt->status = YAI_RECEIPT_OBSERVED;
        yai_copy_string(receipt->summary, sizeof(receipt->summary), "effect observed in NEW.1 loop");
    } else {
        receipt->status = YAI_RECEIPT_BLOCKED;
        yai_copy_string(receipt->summary, sizeof(receipt->summary), "effect blocked or held for review in NEW.1 loop");
    }
    return YAI_OK;
}

const char *yai_receipt_status_string(yai_receipt_status_t status) {
    switch (status) {
    case YAI_RECEIPT_EXECUTED:
        return "executed";
    case YAI_RECEIPT_OBSERVED:
        return "observed";
    case YAI_RECEIPT_BLOCKED:
        return "blocked";
    case YAI_RECEIPT_FAILED:
        return "failed";
    case YAI_RECEIPT_IMPORTED:
        return "imported";
    case YAI_RECEIPT_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}
