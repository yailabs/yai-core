#ifndef YAI_PROJECTION_FRESHNESS_H
#define YAI_PROJECTION_FRESHNESS_H

#include "yai/base/error.h"
#include "yai/hot/hot_snapshot.h"

typedef enum yai_projection_freshness {
    YAI_PROJECTION_FRESH = 0,
    YAI_PROJECTION_STALE = 1,
    YAI_PROJECTION_FRESHNESS_UNKNOWN = 2
} yai_projection_freshness_t;

typedef enum yai_projection_freshness_policy {
    YAI_PROJECTION_POLICY_USABLE = 0,
    YAI_PROJECTION_POLICY_REFRESH_RECOMMENDED = 1,
    YAI_PROJECTION_POLICY_REFRESH_REQUIRED = 2,
    YAI_PROJECTION_POLICY_BLOCKED_FOR_MODEL = 3,
    YAI_PROJECTION_POLICY_UNKNOWN = 4
} yai_projection_freshness_policy_t;

const char *yai_projection_freshness_string(yai_projection_freshness_t freshness);
yai_status_t yai_projection_freshness_from_string(const char *value,
                                                  yai_projection_freshness_t *freshness);
const char *yai_projection_freshness_policy_string(yai_projection_freshness_policy_t policy);
yai_projection_freshness_policy_t yai_projection_freshness_policy_for_consumer(
    const char *consumer,
    yai_hot_freshness_t freshness,
    yai_hot_stale_reason_t stale_reason);

#endif
