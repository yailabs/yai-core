#ifndef YAI_CONTROL_AUTHORITY_SCOPE_H
#define YAI_CONTROL_AUTHORITY_SCOPE_H

#include "yai/base/error.h"

typedef struct yai_authority_scope {
    char authority_scope_ref[128];
    int can_claim;
    int can_propose;
    int can_dispatch;
    int can_execute;
    int can_review;
    int can_approve;
    int can_deny;
    int can_defer;
    int can_quarantine;
    int can_mutate_decision;
    int can_create_receipt;
    int can_display;
    char source[64];
} yai_authority_scope_t;

yai_status_t yai_authority_scope_init_model(yai_authority_scope_t *scope);
yai_status_t yai_authority_scope_init_filesystem(yai_authority_scope_t *scope);
yai_status_t yai_authority_scope_init_reviewer(yai_authority_scope_t *scope);
yai_status_t yai_authority_scope_init_terminal(yai_authority_scope_t *scope);

#endif
