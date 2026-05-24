#include "yai/hot/hot_state.h"

#include <string.h>
#include <time.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

static unsigned long long now_ms(void) {
    return (unsigned long long)time(0) * 1000ull;
}

static void set_status(char *dst, size_t dst_size, const char *status) {
    yai_copy_string(dst, dst_size, status);
}

static yai_status_t mark_id(char *dst,
                            size_t dst_size,
                            const char *value,
                            yai_hot_state_t *state,
                            unsigned int flag) {
    if (state == 0 || value == 0 || value[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_copy_string(dst, dst_size, value);
    state->snapshot.case_version += 1;
    state->snapshot.dirty_flags |= flag;
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_init(yai_hot_state_t *state) {
    if (state == 0) {
        return YAI_ERR_INVALID;
    }
    memset(state, 0, sizeof(*state));
    yai_copy_string(state->snapshot.hot_state_id,
                    sizeof(state->snapshot.hot_state_id),
                    "hot:state-v0");
    state->snapshot.projection_freshness = YAI_HOT_FRESHNESS_UNKNOWN;
    state->snapshot.projection_stale_reason = YAI_HOT_STALE_UNKNOWN;
    set_status(state->snapshot.case_session_status,
               sizeof(state->snapshot.case_session_status),
               "unknown");
    set_status(state->snapshot.case_world_status,
               sizeof(state->snapshot.case_world_status),
               "unknown");
    set_status(state->snapshot.case_context_status,
               sizeof(state->snapshot.case_context_status),
               "unknown");
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_set_case(yai_hot_state_t *state, const char *case_ref) {
    if (state == 0 || case_ref == 0 || case_ref[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_copy_string(state->snapshot.case_ref, sizeof(state->snapshot.case_ref), case_ref);
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_set_session(yai_hot_state_t *state, const char *session_id) {
    if (state == 0 || session_id == 0 || session_id[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_copy_string(state->snapshot.case_session_id,
                    sizeof(state->snapshot.case_session_id),
                    session_id);
    set_status(state->snapshot.case_session_status,
               sizeof(state->snapshot.case_session_status),
               "active");
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_set_case_world_loaded(yai_hot_state_t *state) {
    if (state == 0) {
        return YAI_ERR_INVALID;
    }
    set_status(state->snapshot.case_world_status,
               sizeof(state->snapshot.case_world_status),
               "loaded");
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_set_context(yai_hot_state_t *state, const char *context_id) {
    if (state == 0 || context_id == 0 || context_id[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_copy_string(state->snapshot.case_context_id,
                    sizeof(state->snapshot.case_context_id),
                    context_id);
    set_status(state->snapshot.case_context_status,
               sizeof(state->snapshot.case_context_status),
               "active");
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_mark_record(yai_hot_state_t *state, const char *record_id) {
    return mark_id(state == 0 ? 0 : state->snapshot.last_record_id,
                   YAI_HOT_ID_MAX,
                   record_id,
                   state,
                   YAI_HOT_DIRTY_RECORD);
}

yai_status_t yai_hot_state_mark_decision(yai_hot_state_t *state, const char *decision_id) {
    yai_status_t status = mark_id(state == 0 ? 0 : state->snapshot.last_decision_id,
                                  YAI_HOT_ID_MAX,
                                  decision_id,
                                  state,
                                  YAI_HOT_DIRTY_RECORD);
    if (status != YAI_OK) {
        return status;
    }
    state->snapshot.projection_freshness = YAI_HOT_FRESHNESS_STALE;
    state->snapshot.projection_stale_reason = YAI_HOT_STALE_NEW_DECISION_AFTER_PROJECTION;
    state->snapshot.dirty_flags |= YAI_HOT_DIRTY_PROJECTION;
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_mark_receipt(yai_hot_state_t *state, const char *receipt_id) {
    yai_status_t status = mark_id(state == 0 ? 0 : state->snapshot.last_receipt_id,
                                  YAI_HOT_ID_MAX,
                                  receipt_id,
                                  state,
                                  YAI_HOT_DIRTY_RECORD | YAI_HOT_DIRTY_RECEIPT);
    if (status != YAI_OK) {
        return status;
    }
    state->snapshot.projection_freshness = YAI_HOT_FRESHNESS_STALE;
    state->snapshot.projection_stale_reason = YAI_HOT_STALE_NEW_RECEIPT_AFTER_PROJECTION;
    state->snapshot.dirty_flags |= YAI_HOT_DIRTY_PROJECTION;
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_mark_projection(yai_hot_state_t *state, const char *projection_id) {
    if (state == 0 || projection_id == 0 || projection_id[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_copy_string(state->snapshot.previous_projection_id,
                    sizeof(state->snapshot.previous_projection_id),
                    state->snapshot.current_projection_id);
    yai_copy_string(state->snapshot.current_projection_id,
                    sizeof(state->snapshot.current_projection_id),
                    projection_id);
    state->snapshot.projection_freshness = YAI_HOT_FRESHNESS_FRESH;
    state->snapshot.projection_stale_reason = YAI_HOT_STALE_NONE;
    state->snapshot.dirty_flags &= ~YAI_HOT_DIRTY_PROJECTION;
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_mark_thread(yai_hot_state_t *state, const char *thread_id) {
    yai_status_t status = mark_id(state == 0 ? 0 : state->snapshot.active_thread_id,
                                  YAI_HOT_ID_MAX,
                                  thread_id,
                                  state,
                                  YAI_HOT_DIRTY_THREAD | YAI_HOT_DIRTY_PROJECTION);
    if (status != YAI_OK) {
        return status;
    }
    state->snapshot.projection_freshness = YAI_HOT_FRESHNESS_STALE;
    state->snapshot.projection_stale_reason = YAI_HOT_STALE_THREAD_CHANGED;
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_mark_participant_view(yai_hot_state_t *state,
                                                 const char *frame_id) {
    return mark_id(state == 0 ? 0 : state->snapshot.participant_view_frame_id,
                   YAI_HOT_ID_MAX,
                   frame_id,
                   state,
                   YAI_HOT_DIRTY_PROJECTION);
}

yai_status_t yai_hot_state_invalidate_projection(yai_hot_state_t *state,
                                                 yai_hot_stale_reason_t reason) {
    if (state == 0) {
        return YAI_ERR_INVALID;
    }
    state->snapshot.projection_freshness = YAI_HOT_FRESHNESS_STALE;
    state->snapshot.projection_stale_reason = reason;
    state->snapshot.dirty_flags |= YAI_HOT_DIRTY_PROJECTION;
    state->snapshot.updated_at_unix_ms = now_ms();
    return YAI_OK;
}

yai_status_t yai_hot_state_snapshot(const yai_hot_state_t *state,
                                    yai_hot_snapshot_t *out) {
    if (state == 0 || out == 0) {
        return YAI_ERR_INVALID;
    }
    *out = state->snapshot;
    return YAI_OK;
}
