/*
 * YAI - Process state
 *
 * Purpose:
 *   Names observed process states for the process carrier.
 *
 * Ownership:
 *   Process state enum and string conversion ABI.
 *
 * Boundary:
 *   This file does not observe processes or send signals.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_PROCESS_STATE_H
#define YAI_EFFECT_PROCESS_STATE_H

typedef enum yai_process_state {
    YAI_PROCESS_STATE_UNKNOWN = 0,
    YAI_PROCESS_STATE_RUNNING = 1,
    YAI_PROCESS_STATE_STOPPED = 2,
    YAI_PROCESS_STATE_EXITED = 3,
    YAI_PROCESS_STATE_SIGNALED = 4,
    YAI_PROCESS_STATE_NOT_FOUND = 5,
    YAI_PROCESS_STATE_PERMISSION_DENIED = 6
} yai_process_state_t;

const char *yai_process_state_to_string(yai_process_state_t state);
yai_process_state_t yai_process_state_from_string(const char *value);

#endif
