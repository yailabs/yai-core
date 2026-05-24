/*
 * YAI - Dispatch plan
 *
 * Implements:
 *   Static carrier-family to dispatch-lane planning.
 *
 * This file owns:
 *   Inspectable dispatch plan defaults for SPINE.33B.
 *
 * This file does not own:
 *   Runtime scheduling, worker pools, carrier execution or decision outcomes.
 */

#include "yai/effect/dispatch_plan.h"

#include <string.h>

static void set_dispatch_ref(yai_dispatch_plan_t *plan, const char *value) {
    if (value == 0) value = "";
    strncpy(plan->dispatch_ref, value, sizeof(plan->dispatch_ref) - 1);
    plan->dispatch_ref[sizeof(plan->dispatch_ref) - 1] = '\0';
}

yai_status_t yai_dispatch_plan_init(yai_dispatch_plan_t *plan) {
    if (plan == 0) {
        return YAI_ERR_INVALID;
    }
    set_dispatch_ref(plan, "dispatch:unknown");
    plan->carrier_family = YAI_CARRIER_FAMILY_UNKNOWN;
    plan->lane = YAI_DISPATCH_LANE_UNKNOWN;
    plan->dispatch_status = YAI_DISPATCH_NOT_SUPPORTED;
    plan->lane_status = "not_supported";
    plan->ordering_policy = YAI_ORDERING_UNKNOWN;
    plan->capacity_policy = YAI_CAPACITY_DISABLED;
    plan->lock_policy = YAI_LOCK_UNKNOWN;
    plan->timeout_policy = YAI_TIMEOUT_UNKNOWN;
    plan->retry_policy = YAI_RETRY_UNKNOWN;
    plan->receipt_requirement = "not_available";
    plan->failure_mode = "unsupported_family";
    plan->execution_performed = false;
    return YAI_OK;
}

yai_status_t yai_dispatch_plan_for_carrier_family(
    yai_carrier_family_t family,
    yai_dispatch_plan_t *out
) {
    if (yai_dispatch_plan_init(out) != YAI_OK) {
        return YAI_ERR_INVALID;
    }

    out->carrier_family = family;
    out->dispatch_status = YAI_DISPATCH_ROUTABLE;
    out->lane_status = "planned";
    out->ordering_policy = YAI_ORDERING_SERIAL_PER_CASE;
    out->capacity_policy = YAI_CAPACITY_SINGLE_INFLIGHT;
    out->timeout_policy = YAI_TIMEOUT_NORMAL;
    out->retry_policy = YAI_RETRY_REQUIRES_REVIEW;
    out->receipt_requirement = "required";
    out->failure_mode = "explicit_failure";
    out->execution_performed = false;

    switch (family) {
    case YAI_CARRIER_FAMILY_FILESYSTEM:
        set_dispatch_ref(out, "dispatch:filesystem");
        out->lane = YAI_DISPATCH_LANE_FILESYSTEM;
        out->lane_status = "active_minimal";
        out->lock_policy = YAI_LOCK_CASE;
        out->retry_policy = YAI_RETRY_SAFE;
        return YAI_OK;
    case YAI_CARRIER_FAMILY_PROCESS:
        set_dispatch_ref(out, "dispatch:process");
        out->lane = YAI_DISPATCH_LANE_PROCESS;
        out->lock_policy = YAI_LOCK_TARGET;
        return YAI_OK;
    case YAI_CARRIER_FAMILY_NETWORK_HTTP:
        set_dispatch_ref(out, "dispatch:network_http");
        out->lane = YAI_DISPATCH_LANE_NETWORK_HTTP;
        out->lock_policy = YAI_LOCK_CARRIER;
        return YAI_OK;
    case YAI_CARRIER_FAMILY_DATABASE:
        set_dispatch_ref(out, "dispatch:database");
        out->lane = YAI_DISPATCH_LANE_DATABASE;
        out->lock_policy = YAI_LOCK_TARGET;
        return YAI_OK;
    case YAI_CARRIER_FAMILY_REPOSITORY_GIT:
        set_dispatch_ref(out, "dispatch:repository_git");
        out->lane = YAI_DISPATCH_LANE_REPOSITORY_GIT;
        out->lock_policy = YAI_LOCK_TARGET;
        return YAI_OK;
    case YAI_CARRIER_FAMILY_MODEL_PROVIDER:
        set_dispatch_ref(out, "dispatch:model_provider");
        out->lane = YAI_DISPATCH_LANE_MODEL_PROVIDER;
        out->lock_policy = YAI_LOCK_CARRIER;
        return YAI_OK;
    case YAI_CARRIER_FAMILY_OBSERVATION:
        set_dispatch_ref(out, "dispatch:observation");
        out->lane = YAI_DISPATCH_LANE_OBSERVATION;
        out->ordering_policy = YAI_ORDERING_PARALLEL_BY_SUBJECT;
        out->lock_policy = YAI_LOCK_NONE;
        out->retry_policy = YAI_RETRY_NONE;
        return YAI_OK;
    case YAI_CARRIER_FAMILY_REVIEW:
        set_dispatch_ref(out, "dispatch:review");
        out->lane = YAI_DISPATCH_LANE_REVIEW;
        out->timeout_policy = YAI_TIMEOUT_MANUAL;
        out->retry_policy = YAI_RETRY_MANUAL;
        out->lock_policy = YAI_LOCK_CASE;
        return YAI_OK;
    case YAI_CARRIER_FAMILY_UNKNOWN:
        break;
    default:
        break;
    }

    out->carrier_family = YAI_CARRIER_FAMILY_UNKNOWN;
    out->lane = YAI_DISPATCH_LANE_UNKNOWN;
    out->dispatch_status = YAI_DISPATCH_NOT_SUPPORTED;
    out->lane_status = "not_supported";
    out->receipt_requirement = "not_available";
    out->failure_mode = "unsupported_family";
    out->execution_performed = false;
    return YAI_OK;
}
