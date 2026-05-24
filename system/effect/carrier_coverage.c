/*
 * YAI - Carrier coverage
 *
 * Implements:
 *   Static carrier family/mode/outcome coverage matrix.
 *
 * This file owns:
 *   Coverage visibility for active, skeleton, planned and unsupported carriers.
 *
 * This file does not own:
 *   Carrier execution, fake readiness or dynamic adapter loading.
 */

#include "yai/effect/carrier_coverage.h"

#include <string.h>

static const yai_carrier_coverage_t coverage[] = {
    {YAI_CARRIER_FAMILY_FILESYSTEM, YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_PLANNED, "true", "filesystem_sandbox_only", 1, "carrier.v1", "executed,blocked,failed,observed,not_attempted", "filesystem carrier.v1 active-minimal"},
    {YAI_CARRIER_FAMILY_PROCESS, YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL, YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL, YAI_CARRIER_ADAPTER_PLANNED, "true_limited", "test_owned_or_safe_policy_only", 1, "carrier.v1", "executed,blocked,failed,observed,mismatch,not_attempted", "process carrier controls only safe/test-owned signal paths"},
    {YAI_CARRIER_FAMILY_NETWORK_HTTP, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, "false", "none", 1, "planned", "executed,blocked,failed,mismatch", "visible skeleton; no network execution"},
    {YAI_CARRIER_FAMILY_DATABASE, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, "false", "none", 1, "planned", "executed,blocked,failed,mismatch", "visible skeleton; no database execution"},
    {YAI_CARRIER_FAMILY_REPOSITORY_GIT, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, "false", "none", 1, "planned", "executed,blocked,failed,mismatch", "visible skeleton; no git execution"},
    {YAI_CARRIER_FAMILY_SERVICE, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, "false", "none", 1, "planned", "executed,blocked,failed,mismatch", "visible skeleton; no service lifecycle control"},
    {YAI_CARRIER_FAMILY_ENDPOINT, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, "false", "none", 1, "planned", "executed,blocked,failed,mismatch", "visible skeleton; no endpoint execution"},
    {YAI_CARRIER_FAMILY_SOCKET, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, "false", "none", 1, "planned", "executed,blocked,failed,mismatch", "visible skeleton; no socket control"},
    {YAI_CARRIER_FAMILY_LISTENER, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, "false", "none", 1, "planned", "executed,blocked,failed,mismatch", "visible skeleton; no listener control"},
    {YAI_CARRIER_FAMILY_MODEL_PROVIDER, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_PLANNED, YAI_CARRIER_ADAPTER_SKELETON, "false", "none", 1, "planned", "observed,failed,mismatch,not_attempted", "visible skeleton; no model provider carrier execution"},
    {YAI_CARRIER_FAMILY_OBSERVATION, YAI_CARRIER_ADAPTER_UNSUPPORTED, YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL, YAI_CARRIER_ADAPTER_PLANNED, "false", "observation_only", 1, "host_probe.v0", "observed,mismatch,failed,not_attempted", "observation is not effect execution"},
    {YAI_CARRIER_FAMILY_REVIEW, YAI_CARRIER_ADAPTER_UNSUPPORTED, YAI_CARRIER_ADAPTER_UNSUPPORTED, YAI_CARRIER_ADAPTER_SKELETON, "false", "review_lane_only", 1, "planned", "deferred,waiting_operator,waiting_agent,not_attempted", "review is not machine effect execution"},
    {YAI_CARRIER_FAMILY_UNKNOWN, YAI_CARRIER_ADAPTER_UNSUPPORTED, YAI_CARRIER_ADAPTER_UNSUPPORTED, YAI_CARRIER_ADAPTER_UNSUPPORTED, "false", "none", 0, "none", "not_attempted", "unsupported/unclassified family"},
};

const char *yai_carrier_adapter_status_to_string(yai_carrier_adapter_status_t status) {
    switch (status) {
    case YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL:
        return "active_minimal";
    case YAI_CARRIER_ADAPTER_SKELETON:
        return "skeleton";
    case YAI_CARRIER_ADAPTER_PLANNED:
        return "planned";
    case YAI_CARRIER_ADAPTER_UNSUPPORTED:
        return "unsupported";
    case YAI_CARRIER_ADAPTER_DEFERRED:
        return "deferred";
    case YAI_CARRIER_ADAPTER_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_carrier_adapter_status_t yai_carrier_adapter_status_from_string(const char *value) {
    if (value == 0) return YAI_CARRIER_ADAPTER_UNKNOWN;
    if (strcmp(value, "active_minimal") == 0) return YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL;
    if (strcmp(value, "skeleton") == 0) return YAI_CARRIER_ADAPTER_SKELETON;
    if (strcmp(value, "planned") == 0) return YAI_CARRIER_ADAPTER_PLANNED;
    if (strcmp(value, "unsupported") == 0) return YAI_CARRIER_ADAPTER_UNSUPPORTED;
    if (strcmp(value, "deferred") == 0) return YAI_CARRIER_ADAPTER_DEFERRED;
    return YAI_CARRIER_ADAPTER_UNKNOWN;
}

const yai_carrier_coverage_t *yai_carrier_coverage_entries(size_t *count) {
    if (count != 0) {
        *count = sizeof(coverage) / sizeof(coverage[0]);
    }
    return coverage;
}

const yai_carrier_coverage_t *yai_carrier_coverage_for_family(yai_carrier_family_t family) {
    size_t i;
    for (i = 0; i < sizeof(coverage) / sizeof(coverage[0]); i += 1) {
        if (coverage[i].family == family) {
            return &coverage[i];
        }
    }
    return &coverage[sizeof(coverage) / sizeof(coverage[0]) - 1];
}
