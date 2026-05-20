#ifndef YAI_PROJECTION_REDACTION_H
#define YAI_PROJECTION_REDACTION_H

#include "yai/base/error.h"

typedef enum yai_redaction_posture {
    YAI_REDACTION_NONE = 0,
    YAI_REDACTION_SUMMARY_ONLY = 1,
    YAI_REDACTION_REFS_ONLY = 2,
    YAI_REDACTION_REDACTED = 3,
    YAI_REDACTION_BLOCKED = 4
} yai_redaction_posture_t;

const char *yai_redaction_posture_string(yai_redaction_posture_t posture);
yai_status_t yai_redaction_posture_from_string(const char *value,
                                               yai_redaction_posture_t *posture);

#endif
