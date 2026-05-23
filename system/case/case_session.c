#include "yai/case/case_session.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_case_session_open(yai_case_session_t *session,
                                   const char *session_id,
                                   const char *journal_path,
                                   const yai_case_ref_t *case_ref,
                                   const yai_subject_ref_t *active_subject_ref,
                                   const yai_journal_t *journal) {
    if (session == 0 || session_id == 0 || session_id[0] == '\0' ||
        journal_path == 0 || journal_path[0] == '\0' || journal == 0) {
        return YAI_ERR_INVALID;
    }
    memset(session, 0, sizeof(*session));
    yai_copy_string(session->session_id, sizeof(session->session_id), session_id);
    yai_copy_string(session->journal_path, sizeof(session->journal_path), journal_path);
    if (yai_case_context_init(&session->context, case_ref, active_subject_ref) != YAI_OK ||
        yai_case_context_materialize(&session->context, journal) != YAI_OK) {
        return YAI_ERR_INVALID;
    }
    yai_copy_string(session->lifecycle_state,
                    sizeof(session->lifecycle_state),
                    yai_case_context_is_active(&session->context) ? "active" : "stale");
    return YAI_OK;
}

int yai_case_session_is_active(const yai_case_session_t *session) {
    return session != 0 && yai_case_context_is_active(&session->context) &&
           strcmp(session->lifecycle_state, "active") == 0;
}
