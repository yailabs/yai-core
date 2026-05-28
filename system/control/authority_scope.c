#include "yai/control/authority_scope.h"

#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

static yai_status_t init_scope(yai_authority_scope_t *scope, const char *scope_ref) {
    if (scope == 0) {
        return YAI_ERR_INVALID;
    }
    memset(scope, 0, sizeof(*scope));
    yai_copy_string(scope->authority_scope_ref, sizeof(scope->authority_scope_ref), scope_ref);
    yai_copy_string(scope->source, sizeof(scope->source), "derived_v0");
    return YAI_OK;
}

yai_status_t yai_authority_scope_init_model(yai_authority_scope_t *scope) {
    yai_status_t status = init_scope(scope, "authority-scope:llm-provider:v0");
    if (status != YAI_OK) return status;
    scope->can_claim = 1;
    scope->can_propose = 1;
    return YAI_OK;
}

yai_status_t yai_authority_scope_init_filesystem(yai_authority_scope_t *scope) {
    yai_status_t status = init_scope(scope, "authority-scope:filesystem-sandbox:v0");
    if (status != YAI_OK) return status;
    scope->can_dispatch = 1;
    scope->can_execute = 1;
    scope->can_create_receipt = 1;
    return YAI_OK;
}

yai_status_t yai_authority_scope_init_reviewer(yai_authority_scope_t *scope) {
    yai_status_t status = init_scope(scope, "authority-scope:operator-reviewer:v0");
    if (status != YAI_OK) return status;
    scope->can_review = 1;
    scope->can_approve = 1;
    scope->can_deny = 1;
    scope->can_defer = 1;
    scope->can_quarantine = 1;
    scope->can_display = 1;
    return YAI_OK;
}

yai_status_t yai_authority_scope_init_terminal(yai_authority_scope_t *scope) {
    yai_status_t status = init_scope(scope, "authority-scope:linenoise-terminal:v0");
    if (status != YAI_OK) return status;
    scope->can_display = 1;
    return YAI_OK;
}
