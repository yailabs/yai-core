/*
 * YAI - Process state
 *
 * Implements:
 *   Process state string conversion.
 *
 * This file owns:
 *   Process state vocabulary.
 *
 * This file does not own:
 *   Process observation, signal delivery or receipts.
 */

#include "yai/effect/process_state.h"

#include <string.h>

const char *yai_process_state_to_string(yai_process_state_t state) {
    switch (state) {
    case YAI_PROCESS_STATE_UNKNOWN:
        return "unknown";
    case YAI_PROCESS_STATE_RUNNING:
        return "running";
    case YAI_PROCESS_STATE_STOPPED:
        return "stopped";
    case YAI_PROCESS_STATE_EXITED:
        return "exited";
    case YAI_PROCESS_STATE_SIGNALED:
        return "signaled";
    case YAI_PROCESS_STATE_NOT_FOUND:
        return "not_found";
    case YAI_PROCESS_STATE_PERMISSION_DENIED:
        return "permission_denied";
    default:
        return "unknown";
    }
}

yai_process_state_t yai_process_state_from_string(const char *value) {
    if (value == 0) return YAI_PROCESS_STATE_UNKNOWN;
    if (strcmp(value, "running") == 0) return YAI_PROCESS_STATE_RUNNING;
    if (strcmp(value, "stopped") == 0) return YAI_PROCESS_STATE_STOPPED;
    if (strcmp(value, "exited") == 0) return YAI_PROCESS_STATE_EXITED;
    if (strcmp(value, "signaled") == 0) return YAI_PROCESS_STATE_SIGNALED;
    if (strcmp(value, "not_found") == 0) return YAI_PROCESS_STATE_NOT_FOUND;
    if (strcmp(value, "permission_denied") == 0) return YAI_PROCESS_STATE_PERMISSION_DENIED;
    return YAI_PROCESS_STATE_UNKNOWN;
}
