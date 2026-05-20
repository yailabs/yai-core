#ifndef YAI_RECONCILE_RECOVERY_H
#define YAI_RECONCILE_RECOVERY_H

#include "yai/base/error.h"

typedef enum yai_reconcile_posture {
    YAI_RECONCILE_OBSERVED = 0,
    YAI_RECONCILE_REQUIRES_REVIEW = 1,
    YAI_RECONCILE_RECOVERABLE = 2,
    YAI_RECONCILE_UNRECOVERABLE = 3,
    YAI_RECONCILE_COMPENSATED = 4
} yai_reconcile_posture_t;

const char *yai_reconcile_posture_string(yai_reconcile_posture_t posture);
yai_status_t yai_reconcile_posture_from_string(const char *value,
                                               yai_reconcile_posture_t *posture);

#endif
