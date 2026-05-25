/*
 * YAI - Carrier consistency
 *
 * Purpose:
 *   Reconcile carrier outcome posture with decision, dispatch, receipt and
 *   observation posture.
 *
 * Ownership:
 *   SPINE.33I carrier receipt / divergence hardening ABI.
 *
 * Boundary:
 *   This file does not execute carriers, persist receipts or repair
 *   divergences. It classifies consistency posture only.
 *
 * Status:
 *   active
 */

#ifndef YAI_RECONCILE_CARRIER_CONSISTENCY_H
#define YAI_RECONCILE_CARRIER_CONSISTENCY_H

#include "yai/base/error.h"
#include "yai/control/gate_outcome.h"
#include "yai/effect/carrier_outcome.h"
#include "yai/effect/dispatch.h"
#include "yai/observation/observation_result.h"
#include "yai/reconcile/divergence.h"

typedef struct yai_carrier_consistency_input {
    yai_gate_outcome_t decision_outcome;
    yai_dispatch_status_t dispatch_status;
    yai_carrier_outcome_t carrier_outcome;
    yai_observation_result_t observation_result;
    int carrier_attempted;
    int execution_performed;
    int receipt_required;
    int receipt_present;
    int expected_effect;
    int observed_effect;
    int skeleton_carrier;
} yai_carrier_consistency_input_t;

typedef struct yai_carrier_consistency_result {
    yai_gate_outcome_t decision_outcome;
    yai_dispatch_status_t dispatch_status;
    yai_carrier_outcome_t carrier_outcome;
    const char *receipt_posture;
    yai_observation_result_t observation_result;
    int carrier_attempted;
    int execution_performed;
    int receipt_required;
    int receipt_present;
    int expected_effect;
    int observed_effect;
    yai_divergence_kind_t divergence_kind;
    yai_divergence_severity_t severity;
    const char *result;
} yai_carrier_consistency_result_t;

const char *yai_carrier_consistency_result_to_string(const yai_carrier_consistency_result_t *result);

yai_status_t yai_carrier_consistency_check(const yai_carrier_consistency_input_t *input,
                                           yai_carrier_consistency_result_t *out);

yai_status_t yai_carrier_consistency_scenario(const char *scenario,
                                              yai_carrier_consistency_result_t *out);

#endif
