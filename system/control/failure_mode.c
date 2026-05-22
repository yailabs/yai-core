#include "yai/control/failure_mode.h"

#include <string.h>

const char *yai_failure_mode_string(yai_failure_mode_t mode) {
    switch (mode) {
    case YAI_FAILURE_FAIL_CLOSED:
        return "fail_closed";
    case YAI_FAILURE_FAIL_OPEN_OBSERVED:
        return "fail_open_observed";
    case YAI_FAILURE_REQUIRE_REVIEW:
        return "require_review";
    case YAI_FAILURE_REQUIRE_EVIDENCE:
        return "require_evidence";
    case YAI_FAILURE_DEFER_UNTIL_POLICY_AVAILABLE:
        return "defer_until_policy_available";
    case YAI_FAILURE_QUARANTINE_SUBJECT:
        return "quarantine_subject";
    case YAI_FAILURE_DENY_WITHOUT_RECEIPT_PATH:
        return "deny_without_receipt_path";
    default:
        return "unknown";
    }
}

yai_status_t yai_failure_mode_from_string(const char *value,
                                          yai_failure_mode_t *mode) {
    if (value == 0 || mode == 0) {
        return YAI_ERR_INVALID;
    }

    if (strcmp(value, "fail_closed") == 0) {
        *mode = YAI_FAILURE_FAIL_CLOSED;
    } else if (strcmp(value, "fail_open_observed") == 0) {
        *mode = YAI_FAILURE_FAIL_OPEN_OBSERVED;
    } else if (strcmp(value, "require_review") == 0) {
        *mode = YAI_FAILURE_REQUIRE_REVIEW;
    } else if (strcmp(value, "require_evidence") == 0) {
        *mode = YAI_FAILURE_REQUIRE_EVIDENCE;
    } else if (strcmp(value, "defer_until_policy_available") == 0) {
        *mode = YAI_FAILURE_DEFER_UNTIL_POLICY_AVAILABLE;
    } else if (strcmp(value, "quarantine_subject") == 0) {
        *mode = YAI_FAILURE_QUARANTINE_SUBJECT;
    } else if (strcmp(value, "deny_without_receipt_path") == 0) {
        *mode = YAI_FAILURE_DENY_WITHOUT_RECEIPT_PATH;
    } else {
        return YAI_ERR_INVALID;
    }

    return YAI_OK;
}
