/*
 * YAI - Host observation posture
 *
 * Implements:
 *   String conversion for host observation posture.
 *
 * This file owns:
 *   Observation posture vocabulary conversion.
 *
 * This file does not own:
 *   Enforcement, carrier execution or host probing.
 */

#include "yai/observation/host_observation.h"

#include <string.h>

const char *yai_host_observation_posture_to_string(yai_host_observation_posture_t posture) {
    switch (posture) {
    case YAI_HOST_OBSERVATION_NOT_OBSERVED:
        return "not_observed";
    case YAI_HOST_OBSERVATION_OBSERVED:
        return "observed";
    case YAI_HOST_OBSERVATION_MISMATCH:
        return "mismatch";
    case YAI_HOST_OBSERVATION_BYPASS_SUSPECTED:
        return "bypass_suspected";
    case YAI_HOST_OBSERVATION_UNTRACKED_EFFECT:
        return "untracked_effect";
    case YAI_HOST_OBSERVATION_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_host_observation_posture_t yai_host_observation_posture_from_string(const char *value) {
    if (value == 0) {
        return YAI_HOST_OBSERVATION_UNKNOWN;
    }
    if (strcmp(value, "not_observed") == 0) return YAI_HOST_OBSERVATION_NOT_OBSERVED;
    if (strcmp(value, "observed") == 0) return YAI_HOST_OBSERVATION_OBSERVED;
    if (strcmp(value, "mismatch") == 0) return YAI_HOST_OBSERVATION_MISMATCH;
    if (strcmp(value, "bypass_suspected") == 0) return YAI_HOST_OBSERVATION_BYPASS_SUSPECTED;
    if (strcmp(value, "untracked_effect") == 0) return YAI_HOST_OBSERVATION_UNTRACKED_EFFECT;
    return YAI_HOST_OBSERVATION_UNKNOWN;
}
