#ifndef YAI_PROJECTION_FRESHNESS_H
#define YAI_PROJECTION_FRESHNESS_H

#include "yai/base/error.h"

typedef enum yai_projection_freshness {
    YAI_PROJECTION_FRESH = 0,
    YAI_PROJECTION_STALE = 1,
    YAI_PROJECTION_FRESHNESS_UNKNOWN = 2
} yai_projection_freshness_t;

const char *yai_projection_freshness_string(yai_projection_freshness_t freshness);
yai_status_t yai_projection_freshness_from_string(const char *value,
                                                  yai_projection_freshness_t *freshness);

#endif
