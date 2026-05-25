/*
 * YAI - Carrier outcome harness
 *
 * Implements:
 *   Dry-run outcome posture mapping across carrier families.
 *
 * This file owns:
 *   Simulated outcome result posture for SPINE.33H.
 *
 * This file does not own:
 *   Carrier execution, receipt persistence, divergence hardening or adapter
 *   activation.
 */

#include "yai/effect/carrier_outcome_harness.h"

static int is_skeleton_family(yai_carrier_family_t family) {
    return family == YAI_CARRIER_FAMILY_NETWORK_HTTP ||
           family == YAI_CARRIER_FAMILY_DATABASE ||
           family == YAI_CARRIER_FAMILY_REPOSITORY_GIT ||
           family == YAI_CARRIER_FAMILY_SERVICE ||
           family == YAI_CARRIER_FAMILY_ENDPOINT ||
           family == YAI_CARRIER_FAMILY_SOCKET ||
           family == YAI_CARRIER_FAMILY_LISTENER ||
           family == YAI_CARRIER_FAMILY_MODEL_PROVIDER ||
           family == YAI_CARRIER_FAMILY_REVIEW;
}

static const char *carrier_status_for_family(yai_carrier_family_t family) {
    switch (family) {
    case YAI_CARRIER_FAMILY_FILESYSTEM:
    case YAI_CARRIER_FAMILY_PROCESS:
        return "active_minimal";
    case YAI_CARRIER_FAMILY_OBSERVATION:
        return "active_minimal";
    case YAI_CARRIER_FAMILY_NETWORK_HTTP:
    case YAI_CARRIER_FAMILY_DATABASE:
    case YAI_CARRIER_FAMILY_REPOSITORY_GIT:
    case YAI_CARRIER_FAMILY_SERVICE:
    case YAI_CARRIER_FAMILY_ENDPOINT:
    case YAI_CARRIER_FAMILY_SOCKET:
    case YAI_CARRIER_FAMILY_LISTENER:
    case YAI_CARRIER_FAMILY_MODEL_PROVIDER:
    case YAI_CARRIER_FAMILY_REVIEW:
        return "skeleton";
    case YAI_CARRIER_FAMILY_UNKNOWN:
    default:
        return "unsupported";
    }
}

static const char *execution_available_for_family(yai_carrier_family_t family) {
    switch (family) {
    case YAI_CARRIER_FAMILY_FILESYSTEM:
        return "true";
    case YAI_CARRIER_FAMILY_PROCESS:
        return "true_limited";
    default:
        return "false";
    }
}

static const char *reason_for(yai_carrier_family_t family, yai_carrier_outcome_t outcome) {
    if (family == YAI_CARRIER_FAMILY_UNKNOWN) {
        return "unsupported_family";
    }
    if (outcome == YAI_CARRIER_OUTCOME_MISMATCH) {
        return "simulated_mismatch_posture";
    }
    if (is_skeleton_family(family)) {
        return "skeleton_carrier_no_execution";
    }
    if (family == YAI_CARRIER_FAMILY_PROCESS && outcome == YAI_CARRIER_OUTCOME_BLOCKED) {
        return "unsafe_or_policy_blocked";
    }
    switch (outcome) {
    case YAI_CARRIER_OUTCOME_EXECUTED:
        return "active_carrier_harness_dry_run";
    case YAI_CARRIER_OUTCOME_BLOCKED:
        return "policy_blocked_harness";
    case YAI_CARRIER_OUTCOME_DEFERRED:
        return "simulated_deferred_posture";
    case YAI_CARRIER_OUTCOME_FAILED:
        return "simulated_failed_posture";
    case YAI_CARRIER_OUTCOME_OBSERVED:
        return "simulated_observed_posture";
    case YAI_CARRIER_OUTCOME_QUARANTINED:
        return "simulated_quarantine_posture";
    case YAI_CARRIER_OUTCOME_WAITING_OPERATOR:
    case YAI_CARRIER_OUTCOME_WAITING_AGENT:
        return "simulated_waiting_posture";
    case YAI_CARRIER_OUTCOME_NOT_ATTEMPTED:
        return "not_attempted";
    case YAI_CARRIER_OUTCOME_UNKNOWN:
    default:
        return "unsupported_outcome";
    }
}

int yai_carrier_outcome_harness_test(yai_carrier_family_t family,
                                     yai_carrier_mode_t mode,
                                     yai_carrier_outcome_t requested_outcome,
                                     yai_carrier_outcome_harness_result_t *out) {
    yai_carrier_outcome_t effective_outcome = requested_outcome;

    if (out == 0) {
        return -1;
    }
    if (mode == YAI_CARRIER_MODE_UNKNOWN) {
        mode = YAI_CARRIER_MODE_CONTROLLED;
    }
    if (family == YAI_CARRIER_FAMILY_UNKNOWN ||
        requested_outcome == YAI_CARRIER_OUTCOME_UNKNOWN) {
        effective_outcome = YAI_CARRIER_OUTCOME_NOT_ATTEMPTED;
    }

    out->carrier_family = family;
    out->carrier_mode = mode;
    out->requested_outcome = requested_outcome;
    out->effective_outcome = effective_outcome;
    out->carrier_status = carrier_status_for_family(family);
    out->execution_available = execution_available_for_family(family);
    out->execution_performed = 0;
    out->carrier_attempted = 0;
    out->receipt_required = 1;
    out->receipt_posture = "simulated";
    out->divergence_candidate =
        effective_outcome == YAI_CARRIER_OUTCOME_MISMATCH ? "generated" : "none";
    out->reason = reason_for(family, effective_outcome);
    return 0;
}
