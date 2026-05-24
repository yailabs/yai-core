#include "yai/projection/freshness.h"

#include <string.h>

const char *yai_projection_freshness_string(yai_projection_freshness_t freshness) {
    switch (freshness) {
    case YAI_PROJECTION_FRESH:
        return "fresh";
    case YAI_PROJECTION_STALE:
        return "stale";
    case YAI_PROJECTION_FRESHNESS_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_status_t yai_projection_freshness_from_string(const char *value,
                                                  yai_projection_freshness_t *freshness) {
    if (value == 0 || freshness == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "fresh") == 0) {
        *freshness = YAI_PROJECTION_FRESH;
    } else if (strcmp(value, "stale") == 0) {
        *freshness = YAI_PROJECTION_STALE;
    } else if (strcmp(value, "unknown") == 0) {
        *freshness = YAI_PROJECTION_FRESHNESS_UNKNOWN;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}

const char *yai_projection_freshness_policy_string(yai_projection_freshness_policy_t policy) {
    switch (policy) {
    case YAI_PROJECTION_POLICY_USABLE:
        return "usable";
    case YAI_PROJECTION_POLICY_REFRESH_RECOMMENDED:
        return "refresh_recommended";
    case YAI_PROJECTION_POLICY_REFRESH_REQUIRED:
        return "refresh_required";
    case YAI_PROJECTION_POLICY_BLOCKED_FOR_MODEL:
        return "blocked_for_model";
    case YAI_PROJECTION_POLICY_UNKNOWN:
    default:
        return "unknown";
    }
}

static int strict_consumer(const char *consumer) {
    return consumer != 0 &&
           (strcmp(consumer, "model") == 0 || strcmp(consumer, "agent") == 0);
}

static int warning_consumer(const char *consumer) {
    return consumer != 0 &&
           (strcmp(consumer, "operator") == 0 ||
            strcmp(consumer, "audit") == 0 ||
            strcmp(consumer, "debug") == 0);
}

yai_projection_freshness_policy_t yai_projection_freshness_policy_for_consumer(
    const char *consumer,
    yai_hot_freshness_t freshness,
    yai_hot_stale_reason_t stale_reason) {
    if (freshness == YAI_HOT_FRESHNESS_FRESH &&
        stale_reason == YAI_HOT_STALE_NONE) {
        return YAI_PROJECTION_POLICY_USABLE;
    }
    if (warning_consumer(consumer)) {
        return YAI_PROJECTION_POLICY_REFRESH_RECOMMENDED;
    }
    if (!strict_consumer(consumer)) {
        return YAI_PROJECTION_POLICY_UNKNOWN;
    }
    if (freshness == YAI_HOT_FRESHNESS_REBUILDING) {
        return YAI_PROJECTION_POLICY_REFRESH_REQUIRED;
    }
    switch (stale_reason) {
    case YAI_HOT_STALE_NEW_RECEIPT_AFTER_PROJECTION:
    case YAI_HOT_STALE_NEW_DECISION_AFTER_PROJECTION:
    case YAI_HOT_STALE_NEW_MEMORY_AFTER_PROJECTION:
        return YAI_PROJECTION_POLICY_REFRESH_REQUIRED;
    case YAI_HOT_STALE_NEW_AUTHORITY_SCOPE_AFTER_PROJECTION:
    case YAI_HOT_STALE_NEW_DIVERGENCE_AFTER_PROJECTION:
    case YAI_HOT_STALE_THREAD_CHANGED:
    case YAI_HOT_STALE_MANUAL_REFRESH_REQUIRED:
    case YAI_HOT_STALE_UNKNOWN:
    default:
        return YAI_PROJECTION_POLICY_BLOCKED_FOR_MODEL;
    }
}
