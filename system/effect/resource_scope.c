#include "yai/effect/resource_scope.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_resource_scope_init_filesystem(yai_resource_scope_t *scope,
                                                const yai_case_ref_t *case_ref,
                                                const char *resource_ref) {
    int inside = 0;
    if (scope == 0 || yai_case_ref_is_empty(case_ref) || resource_ref == 0) {
        return YAI_ERR_INVALID;
    }
    memset(scope, 0, sizeof(*scope));
    scope->case_ref = *case_ref;
    inside = strncmp(resource_ref, "sandbox/", 8) == 0 && strstr(resource_ref, "..") == 0;
    (void)snprintf(scope->resource_scope_id,
                   sizeof(scope->resource_scope_id),
                   "resource-scope:%.64s:filesystem",
                   case_ref->case_id.value);
    yai_copy_string(scope->resource_kind, sizeof(scope->resource_kind), "filesystem");
    yai_copy_string(scope->resource_ref, sizeof(scope->resource_ref), resource_ref);
    yai_copy_string(scope->scope_status, sizeof(scope->scope_status), inside ? "inside_sandbox" : "outside_sandbox");
    scope->read_allowed = inside;
    scope->write_allowed = inside;
    scope->execute_allowed = 0;
    yai_copy_string(scope->allowed_prefixes, sizeof(scope->allowed_prefixes), "sandbox/");
    yai_copy_string(scope->denied_prefixes, sizeof(scope->denied_prefixes), "../,/,outside-sandbox/");
    yai_copy_string(scope->source, sizeof(scope->source), "derived_v0");
    return YAI_OK;
}
