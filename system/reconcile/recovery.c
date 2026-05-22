#include "yai/reconcile/recovery.h"

#include <string.h>

const char *yai_reconcile_posture_string(yai_reconcile_posture_t posture) {
    switch (posture) {
    case YAI_RECONCILE_OBSERVED:
        return "observed";
    case YAI_RECONCILE_REQUIRES_REVIEW:
        return "requires_review";
    case YAI_RECONCILE_RECOVERABLE:
        return "recoverable";
    case YAI_RECONCILE_UNRECOVERABLE:
        return "unrecoverable";
    case YAI_RECONCILE_COMPENSATED:
        return "compensated";
    default:
        return "unknown";
    }
}

yai_status_t yai_reconcile_posture_from_string(const char *value,
                                               yai_reconcile_posture_t *posture) {
    if (value == 0 || posture == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "observed") == 0) {
        *posture = YAI_RECONCILE_OBSERVED;
    } else if (strcmp(value, "requires_review") == 0) {
        *posture = YAI_RECONCILE_REQUIRES_REVIEW;
    } else if (strcmp(value, "recoverable") == 0) {
        *posture = YAI_RECONCILE_RECOVERABLE;
    } else if (strcmp(value, "unrecoverable") == 0) {
        *posture = YAI_RECONCILE_UNRECOVERABLE;
    } else if (strcmp(value, "compensated") == 0) {
        *posture = YAI_RECONCILE_COMPENSATED;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
