#ifndef YAI_CONTROL_FAILURE_MODE_H
#define YAI_CONTROL_FAILURE_MODE_H

#include "yai/base/error.h"

typedef enum yai_failure_mode {
    YAI_FAILURE_FAIL_CLOSED = 0,
    YAI_FAILURE_FAIL_OPEN_OBSERVED = 1,
    YAI_FAILURE_REQUIRE_REVIEW = 2,
    YAI_FAILURE_REQUIRE_EVIDENCE = 3,
    YAI_FAILURE_DEFER_UNTIL_POLICY_AVAILABLE = 4,
    YAI_FAILURE_QUARANTINE_SUBJECT = 5,
    YAI_FAILURE_DENY_WITHOUT_RECEIPT_PATH = 6
} yai_failure_mode_t;

const char *yai_failure_mode_string(yai_failure_mode_t mode);
yai_status_t yai_failure_mode_from_string(const char *value,
                                          yai_failure_mode_t *mode);

#endif
