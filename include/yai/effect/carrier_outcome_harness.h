/*
 * YAI - Carrier outcome harness
 *
 * Purpose:
 *   Exposes dry-run carrier outcome posture testing across carrier families.
 *
 * Ownership:
 *   Outcome harness result ABI for SPINE.33H.
 *
 * Boundary:
 *   This file does not execute carriers, mutate host state or create durable
 *   receipts. It represents receiptable posture only.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_CARRIER_OUTCOME_HARNESS_H
#define YAI_EFFECT_CARRIER_OUTCOME_HARNESS_H

#include "yai/effect/carrier_family.h"
#include "yai/effect/carrier_mode.h"
#include "yai/effect/carrier_outcome.h"

typedef struct yai_carrier_outcome_harness_result {
    yai_carrier_family_t carrier_family;
    yai_carrier_mode_t carrier_mode;
    yai_carrier_outcome_t requested_outcome;
    yai_carrier_outcome_t effective_outcome;
    const char *carrier_status;
    const char *execution_available;
    int execution_performed;
    int carrier_attempted;
    int receipt_required;
    const char *receipt_posture;
    const char *divergence_candidate;
    const char *reason;
} yai_carrier_outcome_harness_result_t;

int yai_carrier_outcome_harness_test(yai_carrier_family_t family,
                                     yai_carrier_mode_t mode,
                                     yai_carrier_outcome_t requested_outcome,
                                     yai_carrier_outcome_harness_result_t *out);

#endif
