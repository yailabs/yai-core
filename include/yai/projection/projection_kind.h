#ifndef YAI_PROJECTION_PROJECTION_KIND_H
#define YAI_PROJECTION_PROJECTION_KIND_H

#include "yai/base/error.h"

typedef enum yai_projection_kind {
    YAI_PROJECTION_KIND_OPERATOR_SUMMARY = 0,
    YAI_PROJECTION_KIND_MODEL_CONTEXT = 1,
    YAI_PROJECTION_KIND_AGENT_CONTEXT = 2,
    YAI_PROJECTION_KIND_AUDIT_PACKET = 3,
    YAI_PROJECTION_KIND_DEBUG_DUMP = 4,
    YAI_PROJECTION_KIND_CONTROL_SUMMARY = 5,
    YAI_PROJECTION_KIND_MEMORY_SUMMARY = 6
} yai_projection_kind_t;

const char *yai_projection_kind_string(yai_projection_kind_t kind);
yai_status_t yai_projection_kind_from_string(const char *value,
                                             yai_projection_kind_t *kind);

#endif
