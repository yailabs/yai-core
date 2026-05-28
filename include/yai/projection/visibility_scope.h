#ifndef YAI_PROJECTION_VISIBILITY_SCOPE_H
#define YAI_PROJECTION_VISIBILITY_SCOPE_H

#include "yai/base/error.h"

typedef struct yai_visibility_scope {
    char visibility_scope_ref[128];
    char raw_journal_access[32];
    char filesystem_access[32];
    char record_access[32];
    char fact_access[32];
    char graph_access[32];
    char memory_access[32];
    char policy_access[32];
    char projection_kind[64];
    char redaction[64];
    char consumer_kind[64];
    char source[64];
} yai_visibility_scope_t;

yai_status_t yai_visibility_scope_init_model(yai_visibility_scope_t *scope);
yai_status_t yai_visibility_scope_init_summary(yai_visibility_scope_t *scope,
                                               const char *consumer_kind);

#endif
