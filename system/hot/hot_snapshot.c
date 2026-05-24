#include "yai/hot/hot_snapshot.h"

#include <stdio.h>

const char *yai_hot_freshness_string(yai_hot_freshness_t freshness) {
    switch (freshness) {
    case YAI_HOT_FRESHNESS_FRESH:
        return "fresh";
    case YAI_HOT_FRESHNESS_STALE:
        return "stale";
    case YAI_HOT_FRESHNESS_REBUILDING:
        return "rebuilding";
    case YAI_HOT_FRESHNESS_UNKNOWN:
    default:
        return "unknown";
    }
}

const char *yai_hot_stale_reason_string(yai_hot_stale_reason_t reason) {
    switch (reason) {
    case YAI_HOT_STALE_NONE:
        return "none";
    case YAI_HOT_STALE_NEW_RECEIPT_AFTER_PROJECTION:
        return "new_receipt_after_projection";
    case YAI_HOT_STALE_NEW_DECISION_AFTER_PROJECTION:
        return "new_decision_after_projection";
    case YAI_HOT_STALE_NEW_AUTHORITY_SCOPE_AFTER_PROJECTION:
        return "new_authority_scope_after_projection";
    case YAI_HOT_STALE_NEW_MEMORY_AFTER_PROJECTION:
        return "new_memory_after_projection";
    case YAI_HOT_STALE_NEW_DIVERGENCE_AFTER_PROJECTION:
        return "new_divergence_after_projection";
    case YAI_HOT_STALE_THREAD_CHANGED:
        return "thread_changed";
    case YAI_HOT_STALE_MANUAL_REFRESH_REQUIRED:
        return "manual_refresh_required";
    case YAI_HOT_STALE_UNKNOWN:
    default:
        return "unknown";
    }
}

yai_status_t yai_hot_snapshot_json(const yai_hot_snapshot_t *snapshot,
                                   char *buffer,
                                   size_t buffer_size) {
    int written = 0;
    if (snapshot == 0 || buffer == 0 || buffer_size == 0) {
        return YAI_ERR_INVALID;
    }
    written = snprintf(buffer,
                       buffer_size,
                       "{\"schema\":\"yai.hot_state.v0\",\"hot_state_id\":\"%s\",\"case_ref\":\"%s\",\"case_session_id\":\"%s\",\"case_context_id\":\"%s\",\"case_version\":%lu,\"active_thread_id\":\"%s\",\"current_projection_id\":\"%s\",\"previous_projection_id\":\"%s\",\"last_record_id\":\"%s\",\"last_decision_id\":\"%s\",\"last_receipt_id\":\"%s\",\"last_model_interpretation_id\":\"%s\",\"last_divergence_id\":\"%s\",\"pending_op_count\":%lu,\"pending_obligation_count\":%lu,\"carrier_lock_count\":%lu,\"projection_freshness\":\"%s\",\"projection_stale_reason\":\"%s\",\"dirty_flags\":%u,\"updated_at_unix_ms\":%llu}\n",
                       snapshot->hot_state_id,
                       snapshot->case_ref,
                       snapshot->case_session_id,
                       snapshot->case_context_id,
                       snapshot->case_version,
                       snapshot->active_thread_id,
                       snapshot->current_projection_id,
                       snapshot->previous_projection_id,
                       snapshot->last_record_id,
                       snapshot->last_decision_id,
                       snapshot->last_receipt_id,
                       snapshot->last_model_interpretation_id,
                       snapshot->last_divergence_id,
                       snapshot->pending_op_count,
                       snapshot->pending_obligation_count,
                       snapshot->carrier_lock_count,
                       yai_hot_freshness_string(snapshot->projection_freshness),
                       yai_hot_stale_reason_string(snapshot->projection_stale_reason),
                       snapshot->dirty_flags,
                       snapshot->updated_at_unix_ms);
    if (written < 0 || (size_t)written >= buffer_size) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    return YAI_OK;
}

yai_status_t yai_hot_snapshot_write_json(const yai_hot_snapshot_t *snapshot,
                                         const char *path) {
    char buffer[2048];
    FILE *file = 0;
    if (path == 0 || path[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    if (yai_hot_snapshot_json(snapshot, buffer, sizeof(buffer)) != YAI_OK) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    file = fopen(path, "w");
    if (file == 0) {
        return YAI_ERR_INVALID;
    }
    fputs(buffer, file);
    fclose(file);
    return YAI_OK;
}
