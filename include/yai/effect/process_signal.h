/*
 * YAI - Process signal
 *
 * Purpose:
 *   Names POSIX process signals admitted by process carrier vocabulary.
 *
 * Ownership:
 *   Process signal enum, string conversion ABI and POSIX signal number mapping.
 *
 * Boundary:
 *   This file does not send signals or choose policy.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_PROCESS_SIGNAL_H
#define YAI_EFFECT_PROCESS_SIGNAL_H

typedef enum yai_process_signal {
    YAI_PROCESS_SIGNAL_TERM = 0,
    YAI_PROCESS_SIGNAL_KILL = 1,
    YAI_PROCESS_SIGNAL_INT = 2,
    YAI_PROCESS_SIGNAL_HUP = 3,
    YAI_PROCESS_SIGNAL_UNKNOWN = 4
} yai_process_signal_t;

const char *yai_process_signal_to_string(yai_process_signal_t signal);
yai_process_signal_t yai_process_signal_from_string(const char *value);
int yai_process_signal_to_posix(yai_process_signal_t signal);

#endif
