#ifndef YAI_SUBJECT_SUBJECT_STATE_H
#define YAI_SUBJECT_SUBJECT_STATE_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/store/journal.h"
#include "yai/store/record.h"
#include "yai/subject/subject_ref.h"

typedef enum yai_subject_state_kind {
    YAI_SUBJECT_STATE_BOUND = 0,
    YAI_SUBJECT_STATE_OBSERVED = 1,
    YAI_SUBJECT_STATE_CONTROLLED = 2,
    YAI_SUBJECT_STATE_LOST = 3,
    YAI_SUBJECT_STATE_CONFLICTED = 4,
    YAI_SUBJECT_STATE_UNKNOWN = 5
} yai_subject_state_kind_t;

typedef struct yai_subject_state {
    yai_subject_ref_t subject_ref;
    yai_case_ref_t case_ref;
    yai_subject_state_kind_t state_kind;
    yai_id_t last_receipt_id;
    yai_id_t last_decision_id;
    char last_observed_at[32];
    char content_or_status_hash[32];
    char summary[128];
} yai_subject_state_t;

yai_status_t yai_subject_state_init(yai_subject_state_t *state,
                                    const yai_case_ref_t *case_ref,
                                    const yai_subject_ref_t *subject_ref);
yai_status_t yai_subject_state_apply_record(yai_subject_state_t *state,
                                            const yai_store_record_t *record);
yai_status_t yai_subject_state_from_journal(const yai_case_ref_t *case_ref,
                                            const yai_subject_ref_t *subject_ref,
                                            const yai_journal_t *journal,
                                            yai_subject_state_t *state);
const char *yai_subject_state_kind_string(yai_subject_state_kind_t kind);

#endif
