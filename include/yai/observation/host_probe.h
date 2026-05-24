/*
 * YAI - Host probe
 *
 * Purpose:
 *   Provides independent host observation helpers.
 *
 * Ownership:
 *   Host probe v0 ABI for process observation and expected-state comparison.
 *
 * Boundary:
 *   Host probing is not enforcement, carrier execution or silent repair.
 *
 * Status:
 *   active
 */

#ifndef YAI_OBSERVATION_HOST_PROBE_H
#define YAI_OBSERVATION_HOST_PROBE_H

#include <sys/types.h>

#include "yai/base/error.h"
#include "yai/effect/process_state.h"
#include "yai/observation/observation_result.h"
#include "yai/observation/observation_target.h"
#include "yai/reconcile/divergence_candidate.h"

typedef struct yai_host_probe_observation {
    yai_observation_target_family_t target_family;
    pid_t pid;
    yai_observation_result_t result;
    yai_process_state_t observed_state;
    int observation_is_enforcement;
    char observation_ref[64];
} yai_host_probe_observation_t;

yai_status_t yai_host_probe_observe_process(pid_t pid,
                                            yai_host_probe_observation_t *observation);
yai_status_t yai_host_probe_compare_process(pid_t pid,
                                            yai_process_state_t expected_state,
                                            yai_host_probe_observation_t *observation,
                                            yai_divergence_candidate_t *candidate);

#endif
