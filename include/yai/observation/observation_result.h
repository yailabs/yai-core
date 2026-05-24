/*
 * YAI - Observation result
 *
 * Purpose:
 *   Names the result of comparing independent observation to expectation.
 *
 * Ownership:
 *   Observation result enum and string conversion ABI.
 *
 * Boundary:
 *   Observation result is not carrier outcome, gate outcome or repair action.
 *
 * Status:
 *   active
 */

#ifndef YAI_OBSERVATION_OBSERVATION_RESULT_H
#define YAI_OBSERVATION_OBSERVATION_RESULT_H

typedef enum yai_observation_result {
    YAI_OBSERVATION_RESULT_MATCHED = 0,
    YAI_OBSERVATION_RESULT_MISMATCH = 1,
    YAI_OBSERVATION_RESULT_NOT_FOUND = 2,
    YAI_OBSERVATION_RESULT_PERMISSION_DENIED = 3,
    YAI_OBSERVATION_RESULT_NOT_OBSERVED = 4,
    YAI_OBSERVATION_RESULT_UNKNOWN = 5
} yai_observation_result_t;

const char *yai_observation_result_to_string(yai_observation_result_t result);
yai_observation_result_t yai_observation_result_from_string(const char *value);

#endif
