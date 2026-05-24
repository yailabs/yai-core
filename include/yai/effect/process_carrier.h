/*
 * YAI - Process carrier
 *
 * Purpose:
 *   Declares process carrier v0 observation and signal control helpers.
 *
 * Ownership:
 *   Test-owned process carrier target/receipt ABI for SPINE.33D.
 *
 * Boundary:
 *   This file does not allow arbitrary PID killing, shell execution or process
 *   workflow spawning.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_PROCESS_CARRIER_H
#define YAI_EFFECT_PROCESS_CARRIER_H

#include <sys/types.h>

#include "yai/base/error.h"
#include "yai/effect/carrier_receipt.h"
#include "yai/effect/process_signal.h"
#include "yai/effect/process_state.h"

typedef enum yai_process_owner_scope {
    YAI_PROCESS_OWNER_TEST_OWNED = 0,
    YAI_PROCESS_OWNER_YAI_OWNED = 1,
    YAI_PROCESS_OWNER_EXTERNAL_OBSERVED = 2,
    YAI_PROCESS_OWNER_UNKNOWN = 3
} yai_process_owner_scope_t;

typedef struct yai_process_target {
    char process_ref[64];
    pid_t pid;
    yai_process_owner_scope_t owner_scope;
    int spawned_by_yai;
    yai_process_state_t observed_state;
    char command_preview[128];
} yai_process_target_t;

typedef struct yai_process_carrier_receipt {
    yai_carrier_receipt_t carrier_receipt;
    char operation[32];
    pid_t pid;
    yai_process_signal_t signal;
    yai_process_state_t pre_state;
    yai_process_state_t post_state;
    int carrier_attempted;
    char reason[128];
} yai_process_carrier_receipt_t;

const char *yai_process_owner_scope_to_string(yai_process_owner_scope_t scope);
yai_process_owner_scope_t yai_process_owner_scope_from_string(const char *value);

const char *yai_process_carrier_contract_version(void);
yai_status_t yai_process_target_init(yai_process_target_t *target,
                                     pid_t pid,
                                     yai_process_owner_scope_t owner_scope,
                                     int spawned_by_yai,
                                     const char *command_preview);
yai_status_t yai_process_carrier_observe(yai_process_target_t *target,
                                         yai_process_carrier_receipt_t *receipt);
yai_status_t yai_process_carrier_signal(yai_process_target_t *target,
                                        yai_process_signal_t signal,
                                        int dry_run,
                                        yai_process_carrier_receipt_t *receipt);

#endif
