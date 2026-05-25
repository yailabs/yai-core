/*
 * YAI - carrier receipt divergence smoke
 *
 * Purpose:
 *   SPINE.33I smoke coverage for carrier consistency reconciliation posture.
 *
 * Boundary:
 *   This test does not execute carriers or persist receipts.
 */

#include "yai/reconcile/carrier_consistency.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void require_scenario(const char *scenario,
                             const char *expected_result,
                             yai_divergence_kind_t expected_kind,
                             yai_divergence_severity_t expected_severity) {
    yai_carrier_consistency_result_t result;
    assert(yai_carrier_consistency_scenario(scenario, &result) == YAI_OK);
    assert(strcmp(result.result, expected_result) == 0);
    assert(result.divergence_kind == expected_kind);
    assert(result.severity == expected_severity);
    if (strcmp(scenario, "clean_blocked") == 0) {
        assert(result.execution_performed == 0);
        assert(result.carrier_attempted == 0);
    }
    printf("reconcile:%s %s\n", scenario, yai_divergence_severity_string(result.severity));
}

int main(void) {
    require_scenario("clean_executed",
                     "consistent",
                     YAI_DIVERGENCE_NONE,
                     YAI_DIVERGENCE_INFO);
    require_scenario("clean_blocked",
                     "consistent",
                     YAI_DIVERGENCE_NONE,
                     YAI_DIVERGENCE_INFO);
    require_scenario("denied_but_attempted",
                     "inconsistent",
                     YAI_DIVERGENCE_DENIED_BUT_ATTEMPTED,
                     YAI_DIVERGENCE_CRITICAL);
    require_scenario("executed_without_receipt",
                     "inconsistent",
                     YAI_DIVERGENCE_EXECUTED_WITHOUT_RECEIPT,
                     YAI_DIVERGENCE_ERROR);
    require_scenario("blocked_but_effect_observed",
                     "inconsistent",
                     YAI_DIVERGENCE_BLOCKED_BUT_EFFECT_OBSERVED,
                     YAI_DIVERGENCE_CRITICAL);
    require_scenario("receipt_claimed_executed_but_not_observed",
                     "inconsistent",
                     YAI_DIVERGENCE_RECEIPT_CLAIMED_EXECUTED_BUT_NOT_OBSERVED,
                     YAI_DIVERGENCE_ERROR);
    require_scenario("failed_with_partial_effect",
                     "inconsistent",
                     YAI_DIVERGENCE_FAILED_WITH_PARTIAL_EFFECT,
                     YAI_DIVERGENCE_ERROR);
    require_scenario("skeleton_executed_unexpectedly",
                     "inconsistent",
                     YAI_DIVERGENCE_SKELETON_EXECUTED_UNEXPECTEDLY,
                     YAI_DIVERGENCE_CRITICAL);
    printf("receipt_divergence:no_execution confirmed\n");
    return 0;
}
