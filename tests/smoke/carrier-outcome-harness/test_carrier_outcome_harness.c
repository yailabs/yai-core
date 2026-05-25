/*
 * YAI - carrier outcome harness smoke
 *
 * Purpose:
 *   SPINE.33H smoke coverage for dry-run carrier outcome posture.
 *
 * Scope:
 *   Verifies outcome representation without carrier execution.
 */

#include "yai/yai.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void require_outcome(yai_carrier_family_t family,
                            yai_carrier_mode_t mode,
                            yai_carrier_outcome_t outcome,
                            const char *name,
                            const char *expected_effective,
                            const char *expected_divergence,
                            const char *expected_reason) {
    yai_carrier_outcome_harness_result_t result;
    assert(yai_carrier_outcome_harness_test(family, mode, outcome, &result) == 0);
    assert(result.execution_performed == 0);
    assert(result.carrier_attempted == 0);
    assert(result.receipt_required == 1);
    assert(strcmp(result.receipt_posture, "simulated") == 0);
    assert(strcmp(yai_carrier_outcome_to_string(result.effective_outcome), expected_effective) == 0);
    assert(strcmp(result.divergence_candidate, expected_divergence) == 0);
    assert(strcmp(result.reason, expected_reason) == 0);
    printf("outcome:%s %s ok\n", name, expected_effective);
}

static void require_unknown(void) {
    yai_carrier_outcome_harness_result_t result;
    assert(yai_carrier_outcome_harness_test(YAI_CARRIER_FAMILY_UNKNOWN,
                                            YAI_CARRIER_MODE_CONTROLLED,
                                            YAI_CARRIER_OUTCOME_BLOCKED,
                                            &result) == 0);
    assert(result.execution_performed == 0);
    assert(result.carrier_attempted == 0);
    assert(result.receipt_required == 0);
    assert(strcmp(result.receipt_posture, "none") == 0);
    assert(strcmp(result.carrier_status, "unsupported") == 0);
    assert(strcmp(yai_carrier_outcome_to_string(result.effective_outcome), "not_attempted") == 0);
    assert(strcmp(result.divergence_candidate, "none") == 0);
    assert(strcmp(result.reason, "unsupported_carrier_family") == 0);
    printf("outcome:unknown not_attempted ok\n");
}

int main(void) {
    require_outcome(YAI_CARRIER_FAMILY_FILESYSTEM,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_BLOCKED,
                    "filesystem",
                    "blocked",
                    "none",
                    "policy_blocked_harness");
    require_outcome(YAI_CARRIER_FAMILY_PROCESS,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_BLOCKED,
                    "process",
                    "blocked",
                    "none",
                    "unsafe_or_policy_blocked");
    require_outcome(YAI_CARRIER_FAMILY_DATABASE,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_BLOCKED,
                    "database",
                    "blocked",
                    "none",
                    "skeleton_carrier_no_execution");
    require_outcome(YAI_CARRIER_FAMILY_NETWORK_HTTP,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_FAILED,
                    "network_http",
                    "failed",
                    "none",
                    "simulated_failure_posture");
    require_outcome(YAI_CARRIER_FAMILY_REPOSITORY_GIT,
                    YAI_CARRIER_MODE_OBSERVED,
                    YAI_CARRIER_OUTCOME_MISMATCH,
                    "repository_git",
                    "mismatch",
                    "generated",
                    "simulated_mismatch_posture");
    require_outcome(YAI_CARRIER_FAMILY_SERVICE,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_QUARANTINED,
                    "service",
                    "quarantined",
                    "none",
                    "simulated_quarantine_posture");
    require_outcome(YAI_CARRIER_FAMILY_ENDPOINT,
                    YAI_CARRIER_MODE_OBSERVED,
                    YAI_CARRIER_OUTCOME_OBSERVED,
                    "endpoint",
                    "observed",
                    "none",
                    "simulated_observed_posture");
    require_outcome(YAI_CARRIER_FAMILY_SOCKET,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_DEFERRED,
                    "socket",
                    "deferred",
                    "none",
                    "simulated_deferred_posture");
    require_outcome(YAI_CARRIER_FAMILY_LISTENER,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_NOT_ATTEMPTED,
                    "listener",
                    "not_attempted",
                    "none",
                    "not_attempted");
    require_outcome(YAI_CARRIER_FAMILY_MODEL_PROVIDER,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_WAITING_OPERATOR,
                    "model_provider",
                    "waiting_operator",
                    "none",
                    "simulated_waiting_posture");
    require_outcome(YAI_CARRIER_FAMILY_REVIEW,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_WAITING_AGENT,
                    "review",
                    "waiting_agent",
                    "none",
                    "simulated_waiting_posture");
    require_unknown();
    printf("no_execution:skeletons confirmed\n");
    return 0;
}
