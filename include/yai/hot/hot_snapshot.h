#ifndef YAI_HOT_HOT_SNAPSHOT_H
#define YAI_HOT_HOT_SNAPSHOT_H

#include <stddef.h>

#include "yai/base/error.h"

#define YAI_HOT_ID_MAX 64
#define YAI_HOT_REF_MAX 96
#define YAI_HOT_REASON_MAX 64
#define YAI_HOT_FRESHNESS_MAX 16

typedef enum yai_hot_freshness {
    YAI_HOT_FRESHNESS_FRESH = 0,
    YAI_HOT_FRESHNESS_STALE = 1,
    YAI_HOT_FRESHNESS_UNKNOWN = 2,
    YAI_HOT_FRESHNESS_REBUILDING = 3
} yai_hot_freshness_t;

typedef enum yai_hot_stale_reason {
    YAI_HOT_STALE_NONE = 0,
    YAI_HOT_STALE_NEW_RECEIPT_AFTER_PROJECTION = 1,
    YAI_HOT_STALE_NEW_DECISION_AFTER_PROJECTION = 2,
    YAI_HOT_STALE_NEW_AUTHORITY_SCOPE_AFTER_PROJECTION = 3,
    YAI_HOT_STALE_NEW_MEMORY_AFTER_PROJECTION = 4,
    YAI_HOT_STALE_NEW_DIVERGENCE_AFTER_PROJECTION = 5,
    YAI_HOT_STALE_THREAD_CHANGED = 6,
    YAI_HOT_STALE_MANUAL_REFRESH_REQUIRED = 7,
    YAI_HOT_STALE_UNKNOWN = 8
} yai_hot_stale_reason_t;

typedef struct yai_hot_snapshot {
    char hot_state_id[YAI_HOT_ID_MAX];
    char case_ref[YAI_HOT_REF_MAX];
    char case_session_id[YAI_HOT_ID_MAX];
    char case_context_id[YAI_HOT_ID_MAX];
    unsigned long case_version;
    char active_thread_id[YAI_HOT_ID_MAX];
    char current_projection_id[YAI_HOT_ID_MAX];
    char previous_projection_id[YAI_HOT_ID_MAX];
    char last_record_id[YAI_HOT_ID_MAX];
    char last_decision_id[YAI_HOT_ID_MAX];
    char last_receipt_id[YAI_HOT_ID_MAX];
    char last_model_interpretation_id[YAI_HOT_ID_MAX];
    char last_divergence_id[YAI_HOT_ID_MAX];
    unsigned long pending_op_count;
    unsigned long pending_obligation_count;
    unsigned long carrier_lock_count;
    yai_hot_freshness_t projection_freshness;
    yai_hot_stale_reason_t projection_stale_reason;
    unsigned int dirty_flags;
    unsigned long long updated_at_unix_ms;
} yai_hot_snapshot_t;

const char *yai_hot_freshness_string(yai_hot_freshness_t freshness);
const char *yai_hot_stale_reason_string(yai_hot_stale_reason_t reason);
yai_status_t yai_hot_snapshot_json(const yai_hot_snapshot_t *snapshot,
                                   char *buffer,
                                   size_t buffer_size);
yai_status_t yai_hot_snapshot_write_json(const yai_hot_snapshot_t *snapshot,
                                         const char *path);

#endif
