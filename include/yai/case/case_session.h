#ifndef YAI_CASE_CASE_SESSION_H
#define YAI_CASE_CASE_SESSION_H

#include "yai/base/error.h"
#include "yai/case/case_context.h"
#include "yai/hot/hot_state.h"

#define YAI_CASE_SESSION_PATH_MAX 256
#define YAI_CASE_SESSION_STATE_MAX 32

typedef struct yai_case_session {
    char session_id[64];
    char journal_path[YAI_CASE_SESSION_PATH_MAX];
    char lifecycle_state[YAI_CASE_SESSION_STATE_MAX];
    yai_case_context_t context;
    yai_hot_state_t hot_state;
} yai_case_session_t;

yai_status_t yai_case_session_open(yai_case_session_t *session,
                                   const char *session_id,
                                   const char *journal_path,
                                   const yai_case_ref_t *case_ref,
                                   const yai_subject_ref_t *active_subject_ref,
                                   const yai_journal_t *journal);

int yai_case_session_is_active(const yai_case_session_t *session);

#endif
