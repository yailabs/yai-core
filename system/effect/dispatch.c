/*
 * YAI - Dispatch posture
 *
 * Implements:
 *   Dispatch status conversion and zeroed dispatch initialization.
 *
 * This file owns:
 *   Minimal dispatch posture helper behavior.
 *
 * This file does not own:
 *   Dispatch queues, scheduling, carrier execution or decision outcomes.
 */

#include "yai/effect/dispatch.h"

#include <string.h>

const char *yai_dispatch_status_to_string(yai_dispatch_status_t status) {
    switch (status) {
    case YAI_DISPATCH_PENDING:
        return "pending";
    case YAI_DISPATCH_ROUTABLE:
        return "routable";
    case YAI_DISPATCH_DISPATCHED:
        return "dispatched";
    case YAI_DISPATCH_BLOCKED:
        return "blocked";
    case YAI_DISPATCH_DEFERRED:
        return "deferred";
    case YAI_DISPATCH_FAILED:
        return "failed";
    case YAI_DISPATCH_NOT_SUPPORTED:
        return "not_supported";
    case YAI_DISPATCH_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_dispatch_status_t yai_dispatch_status_from_string(const char *value) {
    if (value == 0) {
        return YAI_DISPATCH_UNKNOWN;
    }
    if (strcmp(value, "pending") == 0) return YAI_DISPATCH_PENDING;
    if (strcmp(value, "routable") == 0) return YAI_DISPATCH_ROUTABLE;
    if (strcmp(value, "dispatched") == 0) return YAI_DISPATCH_DISPATCHED;
    if (strcmp(value, "blocked") == 0) return YAI_DISPATCH_BLOCKED;
    if (strcmp(value, "deferred") == 0) return YAI_DISPATCH_DEFERRED;
    if (strcmp(value, "failed") == 0) return YAI_DISPATCH_FAILED;
    if (strcmp(value, "not_supported") == 0) return YAI_DISPATCH_NOT_SUPPORTED;
    return YAI_DISPATCH_UNKNOWN;
}

yai_status_t yai_dispatch_init(yai_dispatch_t *dispatch) {
    if (dispatch == 0) {
        return YAI_ERR_INVALID;
    }
    dispatch->status = YAI_DISPATCH_PENDING;
    dispatch->family = YAI_CARRIER_FAMILY_UNKNOWN;
    dispatch->route[0] = '\0';
    dispatch->reason[0] = '\0';
    return YAI_OK;
}
