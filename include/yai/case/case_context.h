#ifndef YAI_CASE_CASE_CONTEXT_H
#define YAI_CASE_CASE_CONTEXT_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/case/case_ref.h"
#include "yai/store/journal.h"
#include "yai/subject/subject_ref.h"

typedef enum yai_case_context_state {
    YAI_CASE_CONTEXT_EMPTY = 0,
    YAI_CASE_CONTEXT_ACTIVE = 1,
    YAI_CASE_CONTEXT_STALE = 2
} yai_case_context_state_t;

typedef struct yai_case_context {
    yai_case_ref_t case_ref;
    yai_subject_ref_t active_subject_ref;
    yai_case_context_state_t state;
    size_t source_record_count;
    size_t case_domain_count;
    size_t case_attachment_count;
    size_t case_binding_count;
    size_t interaction_thread_count;
    size_t interaction_turn_count;
    size_t participant_view_frame_count;
    size_t subject_binding_count;
    size_t projection_rule_count;
    size_t authority_scope_count;
    size_t projection_result_count;
    size_t receipt_count;
    size_t memory_candidate_count;
    size_t graph_edge_count;
    char authority_summary[128];
    char projection_summary[128];
} yai_case_context_t;

const char *yai_case_context_state_string(yai_case_context_state_t state);

yai_status_t yai_case_context_init(yai_case_context_t *context,
                                   const yai_case_ref_t *case_ref,
                                   const yai_subject_ref_t *active_subject_ref);

yai_status_t yai_case_context_materialize(yai_case_context_t *context,
                                          const yai_journal_t *journal);

int yai_case_context_is_active(const yai_case_context_t *context);

#endif
