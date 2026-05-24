#ifndef YAI_HOT_HOT_STATE_H
#define YAI_HOT_HOT_STATE_H

#include "yai/base/error.h"
#include "yai/hot/hot_flags.h"
#include "yai/hot/hot_snapshot.h"

typedef struct yai_hot_state {
    yai_hot_snapshot_t snapshot;
} yai_hot_state_t;

yai_status_t yai_hot_state_init(yai_hot_state_t *state);
yai_status_t yai_hot_state_set_case(yai_hot_state_t *state, const char *case_ref);
yai_status_t yai_hot_state_set_session(yai_hot_state_t *state, const char *session_id);
yai_status_t yai_hot_state_set_context(yai_hot_state_t *state, const char *context_id);
yai_status_t yai_hot_state_mark_record(yai_hot_state_t *state, const char *record_id);
yai_status_t yai_hot_state_mark_decision(yai_hot_state_t *state, const char *decision_id);
yai_status_t yai_hot_state_mark_receipt(yai_hot_state_t *state, const char *receipt_id);
yai_status_t yai_hot_state_mark_projection(yai_hot_state_t *state, const char *projection_id);
yai_status_t yai_hot_state_mark_thread(yai_hot_state_t *state, const char *thread_id);
yai_status_t yai_hot_state_invalidate_projection(yai_hot_state_t *state,
                                                 yai_hot_stale_reason_t reason);
yai_status_t yai_hot_state_snapshot(const yai_hot_state_t *state,
                                    yai_hot_snapshot_t *out);

#endif
