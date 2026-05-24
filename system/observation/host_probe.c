/*
 * YAI - Host probe
 *
 * Implements:
 *   POSIX process host observation and expected-state comparison.
 *
 * This file owns:
 *   Independent host probe v0 behavior for process state.
 *
 * This file does not own:
 *   Enforcement, carrier signal execution, silent repair or hardened reconcile.
 */

#define _POSIX_C_SOURCE 200809L

#include "yai/observation/host_probe.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>

static yai_process_state_t probe_process_state(pid_t pid) {
    if (pid <= 0) {
        return YAI_PROCESS_STATE_NOT_FOUND;
    }
    if (kill(pid, 0) == 0) {
        return YAI_PROCESS_STATE_RUNNING;
    }
    if (errno == ESRCH) {
        return YAI_PROCESS_STATE_NOT_FOUND;
    }
    if (errno == EPERM) {
        return YAI_PROCESS_STATE_PERMISSION_DENIED;
    }
    return YAI_PROCESS_STATE_UNKNOWN;
}

static int expected_matches_observed(yai_process_state_t expected, yai_process_state_t observed) {
    if (expected == observed) {
        return 1;
    }
    if (expected == YAI_PROCESS_STATE_STOPPED) {
        return observed == YAI_PROCESS_STATE_NOT_FOUND ||
               observed == YAI_PROCESS_STATE_EXITED ||
               observed == YAI_PROCESS_STATE_SIGNALED ||
               observed == YAI_PROCESS_STATE_STOPPED;
    }
    return 0;
}

static yai_divergence_candidate_kind_t candidate_kind_for(yai_process_state_t expected,
                                                          yai_process_state_t observed) {
    if (expected == YAI_PROCESS_STATE_STOPPED && observed == YAI_PROCESS_STATE_RUNNING) {
        return YAI_DIVERGENCE_CANDIDATE_EXPECTED_STOPPED_BUT_RUNNING;
    }
    if (expected == YAI_PROCESS_STATE_RUNNING && observed == YAI_PROCESS_STATE_NOT_FOUND) {
        return YAI_DIVERGENCE_CANDIDATE_EXPECTED_RUNNING_BUT_NOT_FOUND;
    }
    return YAI_DIVERGENCE_CANDIDATE_UNKNOWN;
}

yai_status_t yai_host_probe_observe_process(pid_t pid,
                                            yai_host_probe_observation_t *observation) {
    if (observation == 0) {
        return YAI_ERR_INVALID;
    }
    memset(observation, 0, sizeof(*observation));
    observation->target_family = YAI_OBSERVATION_TARGET_PROCESS;
    observation->pid = pid;
    observation->observed_state = probe_process_state(pid);
    observation->observation_is_enforcement = 0;
    (void)snprintf(observation->observation_ref,
                   sizeof(observation->observation_ref),
                   "obs:process:%ld",
                   (long)pid);

    switch (observation->observed_state) {
    case YAI_PROCESS_STATE_RUNNING:
        observation->result = YAI_OBSERVATION_RESULT_MATCHED;
        break;
    case YAI_PROCESS_STATE_NOT_FOUND:
        observation->result = YAI_OBSERVATION_RESULT_NOT_FOUND;
        break;
    case YAI_PROCESS_STATE_PERMISSION_DENIED:
        observation->result = YAI_OBSERVATION_RESULT_PERMISSION_DENIED;
        break;
    default:
        observation->result = YAI_OBSERVATION_RESULT_UNKNOWN;
        break;
    }
    return YAI_OK;
}

yai_status_t yai_host_probe_compare_process(pid_t pid,
                                            yai_process_state_t expected_state,
                                            yai_host_probe_observation_t *observation,
                                            yai_divergence_candidate_t *candidate) {
    yai_status_t status = yai_host_probe_observe_process(pid, observation);
    if (status != YAI_OK) {
        return status;
    }
    if (expected_matches_observed(expected_state, observation->observed_state)) {
        observation->result = YAI_OBSERVATION_RESULT_MATCHED;
        if (candidate != 0) {
            memset(candidate, 0, sizeof(*candidate));
            candidate->divergence_kind = YAI_DIVERGENCE_CANDIDATE_UNKNOWN;
            candidate->severity = YAI_DIVERGENCE_CANDIDATE_INFO;
        }
        return YAI_OK;
    }
    observation->result = YAI_OBSERVATION_RESULT_MISMATCH;
    if (candidate != 0) {
        return yai_divergence_candidate_init(
            candidate,
            yai_process_state_to_string(expected_state),
            yai_process_state_to_string(observation->observed_state),
            candidate_kind_for(expected_state, observation->observed_state),
            YAI_DIVERGENCE_CANDIDATE_WARNING,
            observation->observation_ref);
    }
    return YAI_OK;
}
