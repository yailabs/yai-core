#ifndef YAI_MEMORY_MEMORY_KIND_H
#define YAI_MEMORY_MEMORY_KIND_H

#include "yai/base/error.h"

typedef enum yai_memory_kind {
    YAI_MEMORY_OPERATIONAL = 0,
    YAI_MEMORY_DECISION = 1,
    YAI_MEMORY_SUBJECT = 2,
    YAI_MEMORY_ERROR = 3,
    YAI_MEMORY_RECOVERY = 4
} yai_memory_kind_t;

typedef enum yai_memory_freshness {
    YAI_MEMORY_FRESH = 0,
    YAI_MEMORY_STALE = 1,
    YAI_MEMORY_FRESHNESS_UNKNOWN = 2
} yai_memory_freshness_t;

typedef enum yai_memory_confidence {
    YAI_MEMORY_CONFIDENCE_HIGH = 0,
    YAI_MEMORY_CONFIDENCE_MEDIUM = 1,
    YAI_MEMORY_CONFIDENCE_LOW = 2,
    YAI_MEMORY_CONFIDENCE_UNKNOWN = 3
} yai_memory_confidence_t;

const char *yai_memory_kind_string(yai_memory_kind_t kind);
yai_status_t yai_memory_kind_from_string(const char *value,
                                         yai_memory_kind_t *kind);
const char *yai_memory_freshness_string(yai_memory_freshness_t freshness);
yai_status_t yai_memory_freshness_from_string(const char *value,
                                              yai_memory_freshness_t *freshness);
const char *yai_memory_confidence_string(yai_memory_confidence_t confidence);
yai_status_t yai_memory_confidence_from_string(const char *value,
                                               yai_memory_confidence_t *confidence);

#endif
