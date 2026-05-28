#ifndef YAI_EFFECT_RESOURCE_SCOPE_H
#define YAI_EFFECT_RESOURCE_SCOPE_H

#include "yai/base/error.h"
#include "yai/case/case_ref.h"

typedef struct yai_resource_scope {
    char resource_scope_id[128];
    yai_case_ref_t case_ref;
    char resource_kind[64];
    char resource_ref[256];
    char scope_status[64];
    int read_allowed;
    int write_allowed;
    int execute_allowed;
    char allowed_prefixes[256];
    char denied_prefixes[256];
    char source[64];
} yai_resource_scope_t;

yai_status_t yai_resource_scope_init_filesystem(yai_resource_scope_t *scope,
                                                const yai_case_ref_t *case_ref,
                                                const char *resource_ref);

#endif
