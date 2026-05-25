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
                            const char *expected_divergence) {
    yai_carrier_outcome_harness_result_t result;
    assert(yai_carrier_outcome_harness_test(family, mode, outcome, &result) == 0);
    assert(result.execution_performed == 0);
    assert(result.carrier_attempted == 0);
    assert(result.receipt_required == 1);
    assert(strcmp(result.receipt_posture, "simulated") == 0);
    assert(strcmp(yai_carrier_outcome_to_string(result.effective_outcome), expected_effective) == 0);
    assert(strcmp(result.divergence_candidate, expected_divergence) == 0);
    printf("outcome:%s %s ok\n", name, expected_effective);
}

int main(void) {
    require_outcome(YAI_CARRIER_FAMILY_FILESYSTEM,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_BLOCKED,
                    "filesystem",
                    "blocked",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_PROCESS,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_BLOCKED,
                    "process",
                    "blocked",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_DATABASE,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_BLOCKED,
                    "database",
                    "blocked",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_NETWORK_HTTP,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_FAILED,
                    "network_http",
                    "failed",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_REPOSITORY_GIT,
                    YAI_CARRIER_MODE_OBSERVED,
                    YAI_CARRIER_OUTCOME_MISMATCH,
                    "repository_git",
                    "mismatch",
                    "generated");
    require_outcome(YAI_CARRIER_FAMILY_SERVICE,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_QUARANTINED,
                    "service",
                    "quarantined",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_ENDPOINT,
                    YAI_CARRIER_MODE_OBSERVED,
                    YAI_CARRIER_OUTCOME_OBSERVED,
                    "endpoint",
                    "observed",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_SOCKET,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_DEFERRED,
                    "socket",
                    "deferred",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_LISTENER,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_NOT_ATTEMPTED,
                    "listener",
                    "not_attempted",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_MODEL_PROVIDER,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_WAITING_OPERATOR,
                    "model_provider",
                    "waiting_operator",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_REVIEW,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_WAITING_AGENT,
                    "review",
                    "waiting_agent",
                    "none");
    require_outcome(YAI_CARRIER_FAMILY_UNKNOWN,
                    YAI_CARRIER_MODE_CONTROLLED,
                    YAI_CARRIER_OUTCOME_BLOCKED,
                    "unknown",
                    "not_attempted",
                    "none");
    printf("no_execution:skeletons confirmed\n");
    return 0;
}
