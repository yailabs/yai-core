/*
 * YAI - Divergence candidate
 *
 * Implements:
 *   Minimal divergence candidate initialization and vocabulary conversion.
 *
 * This file owns:
 *   Observation mismatch candidate posture.
 *
 * This file does not own:
 *   Hardened divergence records, repair, graph semantics or analytics facts.
 */

#include "yai/reconcile/divergence_candidate.h"

#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

const char *yai_divergence_candidate_kind_to_string(yai_divergence_candidate_kind_t kind) {
    switch (kind) {
    case YAI_DIVERGENCE_CANDIDATE_EXPECTED_STOPPED_BUT_RUNNING:
        return "expected_stopped_but_running";
    case YAI_DIVERGENCE_CANDIDATE_EXPECTED_RUNNING_BUT_NOT_FOUND:
        return "expected_running_but_not_found";
    case YAI_DIVERGENCE_CANDIDATE_RECEIPT_CLAIMED_EXECUTED_BUT_NOT_OBSERVED:
        return "receipt_claimed_executed_but_not_observed";
    case YAI_DIVERGENCE_CANDIDATE_DECISION_DENIED_BUT_EFFECT_OBSERVED:
        return "decision_denied_but_effect_observed";
    case YAI_DIVERGENCE_CANDIDATE_RECEIPT_MISSING_FOR_OBSERVED_EFFECT:
        return "receipt_missing_for_observed_effect";
    case YAI_DIVERGENCE_CANDIDATE_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_divergence_candidate_kind_t yai_divergence_candidate_kind_from_string(const char *value) {
    if (value == 0) return YAI_DIVERGENCE_CANDIDATE_UNKNOWN;
    if (strcmp(value, "expected_stopped_but_running") == 0) {
        return YAI_DIVERGENCE_CANDIDATE_EXPECTED_STOPPED_BUT_RUNNING;
    }
    if (strcmp(value, "expected_running_but_not_found") == 0) {
        return YAI_DIVERGENCE_CANDIDATE_EXPECTED_RUNNING_BUT_NOT_FOUND;
    }
    if (strcmp(value, "receipt_claimed_executed_but_not_observed") == 0) {
        return YAI_DIVERGENCE_CANDIDATE_RECEIPT_CLAIMED_EXECUTED_BUT_NOT_OBSERVED;
    }
    if (strcmp(value, "decision_denied_but_effect_observed") == 0) {
        return YAI_DIVERGENCE_CANDIDATE_DECISION_DENIED_BUT_EFFECT_OBSERVED;
    }
    if (strcmp(value, "receipt_missing_for_observed_effect") == 0) {
        return YAI_DIVERGENCE_CANDIDATE_RECEIPT_MISSING_FOR_OBSERVED_EFFECT;
    }
    return YAI_DIVERGENCE_CANDIDATE_UNKNOWN;
}

const char *yai_divergence_candidate_severity_to_string(yai_divergence_candidate_severity_t severity) {
    switch (severity) {
    case YAI_DIVERGENCE_CANDIDATE_INFO:
        return "info";
    case YAI_DIVERGENCE_CANDIDATE_WARNING:
        return "warning";
    case YAI_DIVERGENCE_CANDIDATE_ERROR:
        return "error";
    case YAI_DIVERGENCE_CANDIDATE_CRITICAL:
        return "critical";
    default:
        return "unknown";
    }
}

yai_status_t yai_divergence_candidate_init(yai_divergence_candidate_t *candidate,
                                           const char *expected_state,
                                           const char *observed_state,
                                           yai_divergence_candidate_kind_t kind,
                                           yai_divergence_candidate_severity_t severity,
                                           const char *observation_ref) {
    if (candidate == 0 || expected_state == 0 || observed_state == 0) {
        return YAI_ERR_INVALID;
    }
    memset(candidate, 0, sizeof(*candidate));
    yai_copy_string(candidate->expected_state, sizeof(candidate->expected_state), expected_state);
    yai_copy_string(candidate->observed_state, sizeof(candidate->observed_state), observed_state);
    yai_copy_string(candidate->source_receipt_ref, sizeof(candidate->source_receipt_ref), "");
    yai_copy_string(candidate->source_decision_ref, sizeof(candidate->source_decision_ref), "");
    yai_copy_string(candidate->observation_ref,
                    sizeof(candidate->observation_ref),
                    observation_ref ? observation_ref : "");
    candidate->divergence_kind = kind;
    candidate->severity = severity;
    return YAI_OK;
}
