#include "yai/case/case_context.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

const char *yai_case_context_state_string(yai_case_context_state_t state) {
    switch (state) {
    case YAI_CASE_CONTEXT_EMPTY:
        return "empty";
    case YAI_CASE_CONTEXT_ACTIVE:
        return "active";
    case YAI_CASE_CONTEXT_STALE:
        return "stale";
    default:
        return "unknown";
    }
}

yai_status_t yai_case_context_init(yai_case_context_t *context,
                                   const yai_case_ref_t *case_ref,
                                   const yai_subject_ref_t *active_subject_ref) {
    if (context == 0 || yai_case_ref_is_empty(case_ref)) {
        return YAI_ERR_INVALID;
    }
    memset(context, 0, sizeof(*context));
    context->case_ref = *case_ref;
    if (active_subject_ref != 0) {
        context->active_subject_ref = *active_subject_ref;
    }
    context->state = YAI_CASE_CONTEXT_EMPTY;
    yai_copy_string(context->authority_summary,
                    sizeof(context->authority_summary),
                    "authority:unmaterialized");
    yai_copy_string(context->projection_summary,
                    sizeof(context->projection_summary),
                    "projection:unmaterialized");
    return YAI_OK;
}

static int record_matches_case(const yai_store_record_t *record,
                               const yai_case_ref_t *case_ref) {
    if (record == 0 || case_ref == 0) {
        return 0;
    }
    return strcmp(record->case_ref.case_id.value, case_ref->case_id.value) == 0;
}

yai_status_t yai_case_context_materialize(yai_case_context_t *context,
                                          const yai_journal_t *journal) {
    size_t index = 0;

    if (context == 0 || journal == 0 || yai_case_ref_is_empty(&context->case_ref)) {
        return YAI_ERR_INVALID;
    }

    context->source_record_count = 0;
    context->case_domain_count = 0;
    context->case_attachment_count = 0;
    context->case_binding_count = 0;
    context->subject_binding_count = 0;
    context->projection_rule_count = 0;
    context->authority_scope_count = 0;
    context->projection_result_count = 0;
    context->receipt_count = 0;
    context->memory_candidate_count = 0;
    context->graph_edge_count = 0;

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        if (!record_matches_case(record, &context->case_ref)) {
            continue;
        }
        context->source_record_count += 1;
        switch (record->record_kind) {
        case YAI_RECORD_CASE_DOMAIN:
            context->case_domain_count += 1;
            break;
        case YAI_RECORD_CASE_ATTACHMENT:
            context->case_attachment_count += 1;
            break;
        case YAI_RECORD_CASE_BINDING:
            context->case_binding_count += 1;
            break;
        case YAI_RECORD_SUBJECT_BINDING:
            context->subject_binding_count += 1;
            break;
        case YAI_RECORD_PROJECTION_RULE:
            context->projection_rule_count += 1;
            break;
        case YAI_RECORD_AUTHORITY_SCOPE:
            context->authority_scope_count += 1;
            break;
        case YAI_RECORD_PROJECTION_RESULT:
            context->projection_result_count += 1;
            break;
        case YAI_RECORD_RECEIPT:
        case YAI_RECORD_EFFECT_RECEIPT:
        case YAI_RECORD_FILESYSTEM_RECEIPT:
            context->receipt_count += 1;
            break;
        case YAI_RECORD_MEMORY_CANDIDATE:
            context->memory_candidate_count += 1;
            break;
        case YAI_RECORD_GRAPH_EDGE:
            context->graph_edge_count += 1;
            break;
        default:
            break;
        }
    }

    context->state = (context->case_domain_count > 0 &&
                      context->case_attachment_count > 0 &&
                      context->case_binding_count > 0)
                         ? YAI_CASE_CONTEXT_ACTIVE
                         : YAI_CASE_CONTEXT_STALE;
    (void)snprintf(context->authority_summary,
                   sizeof(context->authority_summary),
                   "authority:scopes:%zu",
                   context->authority_scope_count);
    (void)snprintf(context->projection_summary,
                   sizeof(context->projection_summary),
                   "projection:results:%zu rules:%zu",
                   context->projection_result_count,
                   context->projection_rule_count);
    return YAI_OK;
}

int yai_case_context_is_active(const yai_case_context_t *context) {
    return context != 0 && context->state == YAI_CASE_CONTEXT_ACTIVE;
}
