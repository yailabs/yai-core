#include "yai/projection/redaction.h"

#include <string.h>

const char *yai_redaction_posture_string(yai_redaction_posture_t posture) {
    switch (posture) {
    case YAI_REDACTION_NONE:
        return "none";
    case YAI_REDACTION_SUMMARY_ONLY:
        return "summary_only";
    case YAI_REDACTION_REFS_ONLY:
        return "refs_only";
    case YAI_REDACTION_REDACTED:
        return "redacted";
    case YAI_REDACTION_BLOCKED:
        return "blocked";
    default:
        return "unknown";
    }
}

yai_status_t yai_redaction_posture_from_string(const char *value,
                                               yai_redaction_posture_t *posture) {
    if (value == 0 || posture == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "none") == 0) {
        *posture = YAI_REDACTION_NONE;
    } else if (strcmp(value, "summary_only") == 0) {
        *posture = YAI_REDACTION_SUMMARY_ONLY;
    } else if (strcmp(value, "refs_only") == 0) {
        *posture = YAI_REDACTION_REFS_ONLY;
    } else if (strcmp(value, "redacted") == 0) {
        *posture = YAI_REDACTION_REDACTED;
    } else if (strcmp(value, "blocked") == 0) {
        *posture = YAI_REDACTION_BLOCKED;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
