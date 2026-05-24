/*
 * YAI - SPINE.33D process carrier smoke
 *
 * Implements:
 *   Test-owned process observation and signal control validation.
 *
 * This file owns:
 *   Safe process carrier v0 smoke coverage.
 *
 * This file does not own:
 *   Arbitrary PID killing, process workflow spawning or host observation
 *   hardening.
 */

#include "yai/yai.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static void require_ok(yai_status_t status) {
    if (status != YAI_OK) {
        printf("unexpected status: %s\n", yai_status_string(status));
        assert(status == YAI_OK);
    }
}

static pid_t spawn_sleep_child(void) {
    pid_t pid = fork();
    assert(pid >= 0);
    if (pid == 0) {
        for (;;) {
            sleep(30);
        }
        _exit(0);
    }
    return pid;
}

int main(void) {
    pid_t term_pid;
    pid_t kill_pid;
    yai_process_target_t target;
    yai_process_carrier_receipt_t receipt;

    assert(strcmp(yai_process_carrier_contract_version(), "carrier.v1") == 0);
    assert(yai_process_signal_from_string("TERM") == YAI_PROCESS_SIGNAL_TERM);
    assert(yai_process_state_from_string("running") == YAI_PROCESS_STATE_RUNNING);
    printf("process_carrier:carrier.v1 active_minimal\n");

    term_pid = spawn_sleep_child();
    require_ok(yai_process_target_init(&target,
                                       term_pid,
                                       YAI_PROCESS_OWNER_TEST_OWNED,
                                       1,
                                       "sleep test child"));
    printf("process:spawned test_owned\n");
    require_ok(yai_process_carrier_observe(&target, &receipt));
    assert(receipt.post_state == YAI_PROCESS_STATE_RUNNING);
    assert(receipt.carrier_receipt.outcome == YAI_CARRIER_OUTCOME_OBSERVED);
    printf("process:observe running\n");

    require_ok(yai_process_carrier_signal(&target, YAI_PROCESS_SIGNAL_TERM, 1, &receipt));
    assert(receipt.carrier_attempted == 0);
    assert(receipt.carrier_receipt.outcome == YAI_CARRIER_OUTCOME_NOT_ATTEMPTED);
    printf("process:signal TERM dry_run routable\n");

    require_ok(yai_process_carrier_signal(&target, YAI_PROCESS_SIGNAL_TERM, 0, &receipt));
    assert(receipt.carrier_attempted == 1);
    assert(receipt.carrier_receipt.outcome == YAI_CARRIER_OUTCOME_EXECUTED);
    assert(receipt.post_state == YAI_PROCESS_STATE_SIGNALED || receipt.post_state == YAI_PROCESS_STATE_EXITED);
    assert(receipt.carrier_receipt.receipt_validated);
    printf("process:signal TERM executed\n");
    printf("process:post_state stopped\n");
    printf("process:receipt validated\n");

    kill_pid = spawn_sleep_child();
    require_ok(yai_process_target_init(&target,
                                       kill_pid,
                                       YAI_PROCESS_OWNER_TEST_OWNED,
                                       1,
                                       "sleep test child kill"));
    require_ok(yai_process_carrier_signal(&target, YAI_PROCESS_SIGNAL_KILL, 0, &receipt));
    assert(receipt.carrier_attempted == 1);
    assert(receipt.carrier_receipt.outcome == YAI_CARRIER_OUTCOME_EXECUTED);

    require_ok(yai_process_target_init(&target,
                                       999999,
                                       YAI_PROCESS_OWNER_EXTERNAL_OBSERVED,
                                       0,
                                       "unsafe external pid"));
    require_ok(yai_process_carrier_signal(&target, YAI_PROCESS_SIGNAL_KILL, 0, &receipt));
    assert(receipt.carrier_attempted == 0);
    assert(receipt.carrier_receipt.outcome == YAI_CARRIER_OUTCOME_BLOCKED);
    printf("process:unsafe_target blocked\n");
    printf("no_arbitrary_kill:confirmed\n");
    return 0;
}
