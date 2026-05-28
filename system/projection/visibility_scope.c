#include "yai/projection/visibility_scope.h"

#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

static yai_status_t init_visibility(yai_visibility_scope_t *scope,
                                    const char *scope_ref,
                                    const char *consumer_kind,
                                    const char *projection_kind) {
    if (scope == 0) {
        return YAI_ERR_INVALID;
    }
    memset(scope, 0, sizeof(*scope));
    yai_copy_string(scope->visibility_scope_ref, sizeof(scope->visibility_scope_ref), scope_ref);
    yai_copy_string(scope->raw_journal_access, sizeof(scope->raw_journal_access), "not_provided");
    yai_copy_string(scope->filesystem_access, sizeof(scope->filesystem_access), "not_provided");
    yai_copy_string(scope->record_access, sizeof(scope->record_access), "scoped_summary");
    yai_copy_string(scope->fact_access, sizeof(scope->fact_access), "scoped_summary");
    yai_copy_string(scope->graph_access, sizeof(scope->graph_access), "scoped_summary");
    yai_copy_string(scope->memory_access, sizeof(scope->memory_access), "scoped_summary");
    yai_copy_string(scope->policy_access, sizeof(scope->policy_access), "scoped_summary");
    yai_copy_string(scope->projection_kind, sizeof(scope->projection_kind), projection_kind);
    yai_copy_string(scope->redaction, sizeof(scope->redaction), "summary_only");
    yai_copy_string(scope->consumer_kind, sizeof(scope->consumer_kind), consumer_kind);
    yai_copy_string(scope->source, sizeof(scope->source), "derived_v0");
    return YAI_OK;
}

yai_status_t yai_visibility_scope_init_model(yai_visibility_scope_t *scope) {
    return init_visibility(scope, "visibility-scope:llm-provider:model-context:v0", "model", "model_context");
}

yai_status_t yai_visibility_scope_init_summary(yai_visibility_scope_t *scope,
                                               const char *consumer_kind) {
    return init_visibility(scope, "visibility-scope:summary:v0", consumer_kind, "case_summary");
}
