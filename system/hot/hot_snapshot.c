#include "yai/hot/hot_snapshot.h"

#include <stdio.h>
#include <string.h>

#include "yai/hot/hot_flags.h"

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

static void append_dirty_flag(char *buffer,
                              size_t buffer_size,
                              unsigned int *written_count,
                              const char *name) {
    size_t length = 0;
    if (buffer == 0 || buffer_size == 0 || written_count == 0 || name == 0) {
        return;
    }
    length = strlen(buffer);
    if (*written_count > 0 && length + 1 < buffer_size) {
        (void)snprintf(buffer + length, buffer_size - length, ",");
        length = strlen(buffer);
    }
    if (length + strlen(name) + 3 < buffer_size) {
        (void)snprintf(buffer + length, buffer_size - length, "\"%s\"", name);
        *written_count += 1;
    }
}

static void dirty_flags_json(unsigned int flags, char *buffer, size_t buffer_size) {
    unsigned int written_count = 0;
    if (buffer == 0 || buffer_size == 0) {
        return;
    }
    buffer[0] = '\0';
    if ((flags & YAI_HOT_DIRTY_RECORD) != 0u) {
        append_dirty_flag(buffer, buffer_size, &written_count, "record_dirty");
    }
    if ((flags & YAI_HOT_DIRTY_RECEIPT) != 0u) {
        append_dirty_flag(buffer, buffer_size, &written_count, "receipt_dirty");
    }
    if ((flags & YAI_HOT_DIRTY_GRAPH) != 0u) {
        append_dirty_flag(buffer, buffer_size, &written_count, "graph_dirty");
    }
    if ((flags & YAI_HOT_DIRTY_MEMORY) != 0u) {
        append_dirty_flag(buffer, buffer_size, &written_count, "memory_dirty");
    }
    if ((flags & YAI_HOT_DIRTY_PROJECTION) != 0u) {
        append_dirty_flag(buffer, buffer_size, &written_count, "projection_dirty");
    }
    if ((flags & YAI_HOT_DIRTY_AUTHORITY) != 0u) {
        append_dirty_flag(buffer, buffer_size, &written_count, "authority_dirty");
    }
    if ((flags & YAI_HOT_DIRTY_THREAD) != 0u) {
        append_dirty_flag(buffer, buffer_size, &written_count, "thread_dirty");
    }
    if ((flags & YAI_HOT_DIRTY_OBLIGATION) != 0u) {
        append_dirty_flag(buffer, buffer_size, &written_count, "obligation_dirty");
    }
    if ((flags & YAI_HOT_DIRTY_CARRIER) != 0u) {
        append_dirty_flag(buffer, buffer_size, &written_count, "carrier_dirty");
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
    char dirty_flags[256];
    if (snapshot == 0 || buffer == 0 || buffer_size == 0) {
        return YAI_ERR_INVALID;
    }
    dirty_flags_json(snapshot->dirty_flags, dirty_flags, sizeof(dirty_flags));
    written = snprintf(buffer,
                       buffer_size,
                       "{\"schema\":\"%s\",\"hot_state_id\":\"%s\",\"case_ref\":\"%s\",\"case_session_id\":\"%s\",\"case_context_id\":\"%s\",\"case_session_status\":\"%s\",\"case_world_status\":\"%s\",\"case_context_status\":\"%s\",\"case_version\":%lu,\"active_thread_id\":\"%s\",\"participant_view_frame_id\":\"%s\",\"current_projection_id\":\"%s\",\"previous_projection_id\":\"%s\",\"last_record_id\":\"%s\",\"last_decision_id\":\"%s\",\"last_receipt_id\":\"%s\",\"last_model_interpretation_id\":\"%s\",\"last_divergence_id\":\"%s\",\"pending_op_count\":%lu,\"pending_obligation_count\":%lu,\"carrier_lock_count\":%lu,\"projection_freshness\":\"%s\",\"projection_stale_reason\":\"%s\",\"dirty_flags\":[%s],\"updated_at_unix_ms\":%llu}\n",
                       YAI_HOT_SNAPSHOT_SCHEMA,
                       snapshot->hot_state_id,
                       snapshot->case_ref,
                       snapshot->case_session_id,
                       snapshot->case_context_id,
                       snapshot->case_session_status,
                       snapshot->case_world_status,
                       snapshot->case_context_status,
                       snapshot->case_version,
                       snapshot->active_thread_id,
                       snapshot->participant_view_frame_id,
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
                       dirty_flags,
                       snapshot->updated_at_unix_ms);
    if (written < 0 || (size_t)written >= buffer_size) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    return YAI_OK;
}

yai_status_t yai_hot_snapshot_write_json(const yai_hot_snapshot_t *snapshot,
                                         const char *path) {
    char buffer[3072];
    char tmp_path[512];
    FILE *file = 0;
    if (path == 0 || path[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    if (yai_hot_snapshot_json(snapshot, buffer, sizeof(buffer)) != YAI_OK) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    if (snprintf(tmp_path, sizeof(tmp_path), "%s.tmp", path) >= (int)sizeof(tmp_path)) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    file = fopen(tmp_path, "w");
    if (file == 0) {
        return YAI_ERR_INVALID;
    }
    if (fputs(buffer, file) < 0 || fflush(file) != 0) {
        (void)fclose(file);
        (void)remove(tmp_path);
        return YAI_ERR_INVALID;
    }
    if (fclose(file) != 0) {
        (void)remove(tmp_path);
        return YAI_ERR_INVALID;
    }
    if (rename(tmp_path, path) != 0) {
        (void)remove(tmp_path);
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
