/*
 * YAI - Carrier consistency
 *
 * Implements:
 *   Carrier receipt / divergence hardening posture for SPINE.33I.
 *
 * This file owns:
 *   Deterministic consistency classification between decision, dispatch,
 *   carrier outcome, receipt posture and observation.
 *
 * This file does not own:
 *   Carrier execution, receipt persistence, host probing or repair.
 */

#include "yai/reconcile/carrier_consistency.h"

#include <string.h>

static int decision_is_deny_like(yai_gate_outcome_t decision) {
    return decision == YAI_GATE_OUTCOME_DENY ||
           decision == YAI_GATE_OUTCOME_QUARANTINE ||
           decision == YAI_GATE_OUTCOME_REQUIRE_REVIEW ||
           decision == YAI_GATE_OUTCOME_REQUIRE_EVIDENCE ||
           decision == YAI_GATE_OUTCOME_REQUIRE_REDACTION;
}

static int observation_claims_missing_effect(yai_observation_result_t observation) {
    return observation == YAI_OBSERVATION_RESULT_MISMATCH ||
           observation == YAI_OBSERVATION_RESULT_NOT_FOUND ||
           observation == YAI_OBSERVATION_RESULT_NOT_OBSERVED;
}

static const char *receipt_posture_for(const yai_carrier_consistency_input_t *input) {
    if (input->receipt_required == 0) {
        return "none";
    }
    return input->receipt_present ? "present" : "missing";
}

const char *yai_carrier_consistency_result_to_string(const yai_carrier_consistency_result_t *result) {
    if (result == 0) {
        return "unknown";
    }
    return result->result;
}

yai_status_t yai_carrier_consistency_check(const yai_carrier_consistency_input_t *input,
                                           yai_carrier_consistency_result_t *out) {
    if (input == 0 || out == 0) {
        return YAI_ERR_INVALID;
    }

    out->decision_outcome = input->decision_outcome;
    out->dispatch_status = input->dispatch_status;
    out->carrier_outcome = input->carrier_outcome;
    out->receipt_posture = receipt_posture_for(input);
    out->observation_result = input->observation_result;
    out->carrier_attempted = input->carrier_attempted;
    out->execution_performed = input->execution_performed;
    out->receipt_required = input->receipt_required;
    out->receipt_present = input->receipt_present;
    out->expected_effect = input->expected_effect;
    out->observed_effect = input->observed_effect;
    out->divergence_kind = YAI_DIVERGENCE_NONE;
    out->severity = YAI_DIVERGENCE_INFO;
    out->result = "consistent";

    if (input->skeleton_carrier &&
        (input->execution_performed || input->carrier_attempted)) {
        out->divergence_kind = YAI_DIVERGENCE_SKELETON_EXECUTED_UNEXPECTEDLY;
        out->severity = YAI_DIVERGENCE_CRITICAL;
    } else if (decision_is_deny_like(input->decision_outcome) &&
               input->carrier_attempted) {
        out->divergence_kind = YAI_DIVERGENCE_DENIED_BUT_ATTEMPTED;
        out->severity = YAI_DIVERGENCE_CRITICAL;
    } else if (input->carrier_outcome == YAI_CARRIER_OUTCOME_EXECUTED &&
               input->receipt_required && !input->receipt_present) {
        out->divergence_kind = YAI_DIVERGENCE_EXECUTED_WITHOUT_RECEIPT;
        out->severity = YAI_DIVERGENCE_ERROR;
    } else if (input->carrier_outcome == YAI_CARRIER_OUTCOME_BLOCKED &&
               input->observed_effect) {
        out->divergence_kind = YAI_DIVERGENCE_BLOCKED_BUT_EFFECT_OBSERVED;
        out->severity = YAI_DIVERGENCE_CRITICAL;
    } else if (input->carrier_outcome == YAI_CARRIER_OUTCOME_EXECUTED &&
               !input->observed_effect &&
               observation_claims_missing_effect(input->observation_result)) {
        out->divergence_kind = YAI_DIVERGENCE_RECEIPT_CLAIMED_EXECUTED_BUT_NOT_OBSERVED;
        out->severity = YAI_DIVERGENCE_ERROR;
    } else if (input->carrier_outcome == YAI_CARRIER_OUTCOME_FAILED &&
               input->observed_effect) {
        out->divergence_kind = YAI_DIVERGENCE_FAILED_WITH_PARTIAL_EFFECT;
        out->severity = YAI_DIVERGENCE_ERROR;
    } else if (input->carrier_outcome == YAI_CARRIER_OUTCOME_MISMATCH) {
        out->divergence_kind = YAI_DIVERGENCE_UNKNOWN;
        out->severity = YAI_DIVERGENCE_WARNING;
    } else if (input->receipt_required && !input->receipt_present &&
               input->carrier_outcome != YAI_CARRIER_OUTCOME_NOT_ATTEMPTED) {
        out->divergence_kind = YAI_DIVERGENCE_MISSING_RECEIPT;
        out->severity = YAI_DIVERGENCE_ERROR;
    } else if (input->decision_outcome == YAI_GATE_OUTCOME_UNKNOWN &&
               input->receipt_present) {
        out->divergence_kind = YAI_DIVERGENCE_RECEIPT_WITHOUT_DECISION;
        out->severity = YAI_DIVERGENCE_WARNING;
    }

    if (out->divergence_kind != YAI_DIVERGENCE_NONE) {
        out->result = "inconsistent";
    }
    return YAI_OK;
}

static yai_status_t scenario_input(const char *scenario,
                                   yai_carrier_consistency_input_t *input) {
    if (scenario == 0 || input == 0) {
        return YAI_ERR_INVALID;
    }

    input->decision_outcome = YAI_GATE_OUTCOME_ALLOW;
    input->dispatch_status = YAI_DISPATCH_DISPATCHED;
    input->carrier_outcome = YAI_CARRIER_OUTCOME_EXECUTED;
    input->observation_result = YAI_OBSERVATION_RESULT_MATCHED;
    input->carrier_attempted = 1;
    input->execution_performed = 1;
    input->receipt_required = 1;
    input->receipt_present = 1;
    input->expected_effect = 1;
    input->observed_effect = 1;
    input->skeleton_carrier = 0;

    if (strcmp(scenario, "clean_executed") == 0) {
        return YAI_OK;
    }
    if (strcmp(scenario, "clean_blocked") == 0) {
        input->decision_outcome = YAI_GATE_OUTCOME_DENY;
        input->dispatch_status = YAI_DISPATCH_BLOCKED;
        input->carrier_outcome = YAI_CARRIER_OUTCOME_BLOCKED;
        input->observation_result = YAI_OBSERVATION_RESULT_NOT_OBSERVED;
        input->carrier_attempted = 0;
        input->execution_performed = 0;
        input->expected_effect = 0;
        input->observed_effect = 0;
        return YAI_OK;
    }
    if (strcmp(scenario, "clean_observed") == 0) {
        input->decision_outcome = YAI_GATE_OUTCOME_OBSERVE_ONLY;
        input->dispatch_status = YAI_DISPATCH_DISPATCHED;
        input->carrier_outcome = YAI_CARRIER_OUTCOME_OBSERVED;
        input->carrier_attempted = 0;
        input->execution_performed = 0;
        input->expected_effect = 0;
        input->observed_effect = 0;
        return YAI_OK;
    }
    if (strcmp(scenario, "denied_but_attempted") == 0) {
        input->decision_outcome = YAI_GATE_OUTCOME_DENY;
        input->dispatch_status = YAI_DISPATCH_BLOCKED;
        return YAI_OK;
    }
    if (strcmp(scenario, "executed_without_receipt") == 0) {
        input->receipt_present = 0;
        return YAI_OK;
    }
    if (strcmp(scenario, "blocked_but_effect_observed") == 0) {
        input->decision_outcome = YAI_GATE_OUTCOME_DENY;
        input->dispatch_status = YAI_DISPATCH_BLOCKED;
        input->carrier_outcome = YAI_CARRIER_OUTCOME_BLOCKED;
        input->observation_result = YAI_OBSERVATION_RESULT_MISMATCH;
        input->carrier_attempted = 0;
        input->execution_performed = 0;
        input->expected_effect = 0;
        input->observed_effect = 1;
        return YAI_OK;
    }
    if (strcmp(scenario, "receipt_claimed_executed_but_not_observed") == 0) {
        input->observation_result = YAI_OBSERVATION_RESULT_MISMATCH;
        input->observed_effect = 0;
        return YAI_OK;
    }
    if (strcmp(scenario, "failed_with_partial_effect") == 0) {
        input->carrier_outcome = YAI_CARRIER_OUTCOME_FAILED;
        input->observation_result = YAI_OBSERVATION_RESULT_MISMATCH;
        input->observed_effect = 1;
        return YAI_OK;
    }
    if (strcmp(scenario, "skeleton_executed_unexpectedly") == 0) {
        input->skeleton_carrier = 1;
        return YAI_OK;
    }

    return YAI_ERR_INVALID;
}

yai_status_t yai_carrier_consistency_scenario(const char *scenario,
                                              yai_carrier_consistency_result_t *out) {
    yai_carrier_consistency_input_t input;
    yai_status_t status = scenario_input(scenario, &input);
    if (status != YAI_OK) {
        return status;
    }
    return yai_carrier_consistency_check(&input, out);
}
