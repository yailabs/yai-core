#ifndef YAI_MEMORY_MEMORY_SCOPE_H
#define YAI_MEMORY_MEMORY_SCOPE_H

#include "yai/base/error.h"

typedef enum yai_memory_scope_kind {
    YAI_MEMORY_SCOPE_CASE = 0,
    YAI_MEMORY_SCOPE_SUBJECT = 1,
    YAI_MEMORY_SCOPE_LOCAL = 2
} yai_memory_scope_kind_t;

const char *yai_memory_scope_kind_string(yai_memory_scope_kind_t scope_kind);
yai_status_t yai_memory_scope_kind_from_string(const char *value,
                                               yai_memory_scope_kind_t *scope_kind);

#endif
