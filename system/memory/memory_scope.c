#include "yai/memory/memory_scope.h"

#include <string.h>

const char *yai_memory_scope_kind_string(yai_memory_scope_kind_t scope_kind) {
    switch (scope_kind) {
    case YAI_MEMORY_SCOPE_CASE:
        return "case";
    case YAI_MEMORY_SCOPE_SUBJECT:
        return "subject";
    case YAI_MEMORY_SCOPE_LOCAL:
        return "local";
    default:
        return "unknown";
    }
}

yai_status_t yai_memory_scope_kind_from_string(const char *value,
                                               yai_memory_scope_kind_t *scope_kind) {
    if (value == 0 || scope_kind == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "case") == 0) {
        *scope_kind = YAI_MEMORY_SCOPE_CASE;
    } else if (strcmp(value, "subject") == 0) {
        *scope_kind = YAI_MEMORY_SCOPE_SUBJECT;
    } else if (strcmp(value, "local") == 0) {
        *scope_kind = YAI_MEMORY_SCOPE_LOCAL;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
