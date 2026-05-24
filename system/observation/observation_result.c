/*
 * YAI - Observation result
 *
 * Implements:
 *   String conversion for observation result values.
 *
 * This file owns:
 *   Observation result vocabulary conversion.
 *
 * This file does not own:
 *   Host probing, carrier outcomes or reconciliation hardening.
 */

#include "yai/observation/observation_result.h"

#include <string.h>

const char *yai_observation_result_to_string(yai_observation_result_t result) {
    switch (result) {
    case YAI_OBSERVATION_RESULT_MATCHED:
        return "matched";
    case YAI_OBSERVATION_RESULT_MISMATCH:
        return "mismatch";
    case YAI_OBSERVATION_RESULT_NOT_FOUND:
        return "not_found";
    case YAI_OBSERVATION_RESULT_PERMISSION_DENIED:
        return "permission_denied";
    case YAI_OBSERVATION_RESULT_NOT_OBSERVED:
        return "not_observed";
    case YAI_OBSERVATION_RESULT_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_observation_result_t yai_observation_result_from_string(const char *value) {
    if (value == 0) return YAI_OBSERVATION_RESULT_UNKNOWN;
    if (strcmp(value, "matched") == 0) return YAI_OBSERVATION_RESULT_MATCHED;
    if (strcmp(value, "mismatch") == 0) return YAI_OBSERVATION_RESULT_MISMATCH;
    if (strcmp(value, "not_found") == 0) return YAI_OBSERVATION_RESULT_NOT_FOUND;
    if (strcmp(value, "permission_denied") == 0) return YAI_OBSERVATION_RESULT_PERMISSION_DENIED;
    if (strcmp(value, "not_observed") == 0) return YAI_OBSERVATION_RESULT_NOT_OBSERVED;
    return YAI_OBSERVATION_RESULT_UNKNOWN;
}
