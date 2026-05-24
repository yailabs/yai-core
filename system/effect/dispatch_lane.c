/*
 * YAI - Dispatch lane vocabulary
 *
 * Implements:
 *   Lane and policy enum string conversion.
 *
 * This file owns:
 *   Carrier lane vocabulary and static lane-to-family mapping.
 *
 * This file does not own:
 *   Dispatch queues, execution or receipt production.
 */

#include "yai/effect/dispatch_lane.h"

#include <string.h>

const char *yai_dispatch_lane_to_string(yai_dispatch_lane_t lane) {
    switch (lane) {
    case YAI_DISPATCH_LANE_FILESYSTEM:
        return "filesystem_lane";
    case YAI_DISPATCH_LANE_PROCESS:
        return "process_lane";
    case YAI_DISPATCH_LANE_NETWORK_HTTP:
        return "network_http_lane";
    case YAI_DISPATCH_LANE_DATABASE:
        return "database_lane";
    case YAI_DISPATCH_LANE_REPOSITORY_GIT:
        return "repository_git_lane";
    case YAI_DISPATCH_LANE_MODEL_PROVIDER:
        return "model_provider_lane";
    case YAI_DISPATCH_LANE_OBSERVATION:
        return "observation_lane";
    case YAI_DISPATCH_LANE_REVIEW:
        return "review_lane";
    case YAI_DISPATCH_LANE_UNKNOWN:
        return "unknown_lane";
    default:
        return "unknown_lane";
    }
}

yai_dispatch_lane_t yai_dispatch_lane_from_string(const char *value) {
    if (value == 0) return YAI_DISPATCH_LANE_UNKNOWN;
    if (strcmp(value, "filesystem_lane") == 0) return YAI_DISPATCH_LANE_FILESYSTEM;
    if (strcmp(value, "process_lane") == 0) return YAI_DISPATCH_LANE_PROCESS;
    if (strcmp(value, "network_http_lane") == 0) return YAI_DISPATCH_LANE_NETWORK_HTTP;
    if (strcmp(value, "database_lane") == 0) return YAI_DISPATCH_LANE_DATABASE;
    if (strcmp(value, "repository_git_lane") == 0) return YAI_DISPATCH_LANE_REPOSITORY_GIT;
    if (strcmp(value, "model_provider_lane") == 0) return YAI_DISPATCH_LANE_MODEL_PROVIDER;
    if (strcmp(value, "observation_lane") == 0) return YAI_DISPATCH_LANE_OBSERVATION;
    if (strcmp(value, "review_lane") == 0) return YAI_DISPATCH_LANE_REVIEW;
    return YAI_DISPATCH_LANE_UNKNOWN;
}

const char *yai_ordering_policy_to_string(yai_ordering_policy_t policy) {
    switch (policy) {
    case YAI_ORDERING_SERIAL_PER_CASE:
        return "serial_per_case";
    case YAI_ORDERING_PARALLEL_BY_SUBJECT:
        return "parallel_by_subject";
    case YAI_ORDERING_EXCLUSIVE_GLOBAL:
        return "exclusive_global";
    case YAI_ORDERING_UNORDERED:
        return "unordered";
    case YAI_ORDERING_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

const char *yai_capacity_policy_to_string(yai_capacity_policy_t policy) {
    switch (policy) {
    case YAI_CAPACITY_SINGLE_INFLIGHT:
        return "single_inflight";
    case YAI_CAPACITY_BOUNDED_PARALLEL:
        return "bounded_parallel";
    case YAI_CAPACITY_UNBOUNDED_PLANNED:
        return "unbounded_planned";
    case YAI_CAPACITY_DISABLED:
        return "disabled";
    case YAI_CAPACITY_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

const char *yai_lock_policy_to_string(yai_lock_policy_t policy) {
    switch (policy) {
    case YAI_LOCK_CASE:
        return "case_lock";
    case YAI_LOCK_SUBJECT:
        return "subject_lock";
    case YAI_LOCK_TARGET:
        return "target_lock";
    case YAI_LOCK_CARRIER:
        return "carrier_lock";
    case YAI_LOCK_NONE:
        return "none";
    case YAI_LOCK_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

const char *yai_timeout_policy_to_string(yai_timeout_policy_t policy) {
    switch (policy) {
    case YAI_TIMEOUT_SHORT:
        return "short";
    case YAI_TIMEOUT_NORMAL:
        return "normal";
    case YAI_TIMEOUT_LONG:
        return "long";
    case YAI_TIMEOUT_MANUAL:
        return "manual";
    case YAI_TIMEOUT_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

const char *yai_retry_policy_to_string(yai_retry_policy_t policy) {
    switch (policy) {
    case YAI_RETRY_NONE:
        return "none";
    case YAI_RETRY_SAFE:
        return "safe_retry";
    case YAI_RETRY_MANUAL:
        return "manual_retry";
    case YAI_RETRY_REQUIRES_REVIEW:
        return "requires_review";
    case YAI_RETRY_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_carrier_family_t yai_dispatch_lane_carrier_family(yai_dispatch_lane_t lane) {
    switch (lane) {
    case YAI_DISPATCH_LANE_FILESYSTEM:
        return YAI_CARRIER_FAMILY_FILESYSTEM;
    case YAI_DISPATCH_LANE_PROCESS:
        return YAI_CARRIER_FAMILY_PROCESS;
    case YAI_DISPATCH_LANE_NETWORK_HTTP:
        return YAI_CARRIER_FAMILY_NETWORK_HTTP;
    case YAI_DISPATCH_LANE_DATABASE:
        return YAI_CARRIER_FAMILY_DATABASE;
    case YAI_DISPATCH_LANE_REPOSITORY_GIT:
        return YAI_CARRIER_FAMILY_REPOSITORY_GIT;
    case YAI_DISPATCH_LANE_MODEL_PROVIDER:
        return YAI_CARRIER_FAMILY_MODEL_PROVIDER;
    case YAI_DISPATCH_LANE_OBSERVATION:
        return YAI_CARRIER_FAMILY_OBSERVATION;
    case YAI_DISPATCH_LANE_REVIEW:
        return YAI_CARRIER_FAMILY_REVIEW;
    case YAI_DISPATCH_LANE_UNKNOWN:
        return YAI_CARRIER_FAMILY_UNKNOWN;
    default:
        return YAI_CARRIER_FAMILY_UNKNOWN;
    }
}
