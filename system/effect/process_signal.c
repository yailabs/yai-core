/*
 * YAI - Process signal
 *
 * Implements:
 *   Process signal string conversion and POSIX signal number mapping.
 *
 * This file owns:
 *   Process signal vocabulary.
 *
 * This file does not own:
 *   Signal delivery, process ownership checks or receipts.
 */

#include "yai/effect/process_signal.h"

#include <signal.h>
#include <string.h>

const char *yai_process_signal_to_string(yai_process_signal_t signal) {
    switch (signal) {
    case YAI_PROCESS_SIGNAL_TERM:
        return "TERM";
    case YAI_PROCESS_SIGNAL_KILL:
        return "KILL";
    case YAI_PROCESS_SIGNAL_INT:
        return "INT";
    case YAI_PROCESS_SIGNAL_HUP:
        return "HUP";
    case YAI_PROCESS_SIGNAL_UNKNOWN:
        return "UNKNOWN";
    default:
        return "UNKNOWN";
    }
}

yai_process_signal_t yai_process_signal_from_string(const char *value) {
    if (value == 0) return YAI_PROCESS_SIGNAL_UNKNOWN;
    if (strcmp(value, "TERM") == 0 || strcmp(value, "SIGTERM") == 0) return YAI_PROCESS_SIGNAL_TERM;
    if (strcmp(value, "KILL") == 0 || strcmp(value, "SIGKILL") == 0) return YAI_PROCESS_SIGNAL_KILL;
    if (strcmp(value, "INT") == 0 || strcmp(value, "SIGINT") == 0) return YAI_PROCESS_SIGNAL_INT;
    if (strcmp(value, "HUP") == 0 || strcmp(value, "SIGHUP") == 0) return YAI_PROCESS_SIGNAL_HUP;
    return YAI_PROCESS_SIGNAL_UNKNOWN;
}

int yai_process_signal_to_posix(yai_process_signal_t signal) {
    switch (signal) {
    case YAI_PROCESS_SIGNAL_TERM:
        return SIGTERM;
    case YAI_PROCESS_SIGNAL_KILL:
        return SIGKILL;
    case YAI_PROCESS_SIGNAL_INT:
        return SIGINT;
    case YAI_PROCESS_SIGNAL_HUP:
        return SIGHUP;
    case YAI_PROCESS_SIGNAL_UNKNOWN:
        return 0;
    default:
        return 0;
    }
}
