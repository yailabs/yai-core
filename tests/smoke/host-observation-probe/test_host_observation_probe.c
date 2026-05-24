/*
 * YAI - Host observation probe smoke
 *
 * Purpose:
 *   Validate independent process observation and divergence candidate posture.
 *
 * Ownership:
 *   SPINE.33E smoke coverage for host probe v0.
 *
 * Boundary:
 *   This test does not enforce host state or kill arbitrary processes.
 *
 * Status:
 *   test_only
 */

#define _POSIX_C_SOURCE 200809L

#include "yai/yai.h"

#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void require_ok(yai_status_t status) {
    assert(status == YAI_OK);
}

static pid_t spawn_child(void) {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        for (;;) {
            pause();
        }
    }
    sleep(1);
    return pid;
}

int main(void) {
    pid_t child = spawn_child();
    int wait_status = 0;
    yai_host_probe_observation_t observation;
    yai_divergence_candidate_t candidate;

    assert(yai_observation_target_family_from_string("process") == YAI_OBSERVATION_TARGET_PROCESS);
    assert(yai_observation_result_from_string("mismatch") == YAI_OBSERVATION_RESULT_MISMATCH);

    require_ok(yai_host_probe_observe_process(child, &observation));
    assert(observation.target_family == YAI_OBSERVATION_TARGET_PROCESS);
    assert(observation.observed_state == YAI_PROCESS_STATE_RUNNING);
    assert(observation.observation_is_enforcement == 0);
    printf("host_probe:process observed running\n");

    require_ok(yai_host_probe_compare_process(child, YAI_PROCESS_STATE_RUNNING, &observation, &candidate));
    assert(observation.result == YAI_OBSERVATION_RESULT_MATCHED);
    printf("host_probe:compare running matched\n");

    require_ok(yai_host_probe_compare_process(child, YAI_PROCESS_STATE_STOPPED, &observation, &candidate));
    assert(observation.result == YAI_OBSERVATION_RESULT_MISMATCH);
    assert(candidate.divergence_kind == YAI_DIVERGENCE_CANDIDATE_EXPECTED_STOPPED_BUT_RUNNING);
    assert(strcmp(yai_divergence_candidate_kind_to_string(candidate.divergence_kind),
                  "expected_stopped_but_running") == 0);
    printf("host_probe:compare stopped mismatch\n");
    printf("divergence_candidate:expected_stopped_but_running\n");
    printf("host_probe:observation_is_enforcement false\n");

    assert(kill(child, SIGTERM) == 0);
    assert(waitpid(child, &wait_status, 0) == child);

    require_ok(yai_host_probe_observe_process(child, &observation));
    assert(observation.observed_state == YAI_PROCESS_STATE_NOT_FOUND);
    printf("host_probe:after_term not_found\n");

    require_ok(yai_host_probe_compare_process(child, YAI_PROCESS_STATE_STOPPED, &observation, &candidate));
    assert(observation.result == YAI_OBSERVATION_RESULT_MATCHED);
    printf("host_probe:compare stopped after_term matched\n");

    return 0;
}
