/*
 * YAI - Observation target
 *
 * Implements:
 *   String conversion for observation target families.
 *
 * This file owns:
 *   Observation target vocabulary conversion.
 *
 * This file does not own:
 *   Host probing, enforcement or carrier execution.
 */

#include "yai/observation/observation_target.h"

#include <string.h>

const char *yai_observation_target_family_to_string(yai_observation_target_family_t target) {
    switch (target) {
    case YAI_OBSERVATION_TARGET_PROCESS:
        return "process";
    case YAI_OBSERVATION_TARGET_FILESYSTEM:
        return "filesystem";
    case YAI_OBSERVATION_TARGET_NETWORK:
        return "network";
    case YAI_OBSERVATION_TARGET_DATABASE:
        return "database";
    case YAI_OBSERVATION_TARGET_REPOSITORY:
        return "repository";
    case YAI_OBSERVATION_TARGET_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_observation_target_family_t yai_observation_target_family_from_string(const char *value) {
    if (value == 0) return YAI_OBSERVATION_TARGET_UNKNOWN;
    if (strcmp(value, "process") == 0) return YAI_OBSERVATION_TARGET_PROCESS;
    if (strcmp(value, "filesystem") == 0) return YAI_OBSERVATION_TARGET_FILESYSTEM;
    if (strcmp(value, "network") == 0) return YAI_OBSERVATION_TARGET_NETWORK;
    if (strcmp(value, "database") == 0) return YAI_OBSERVATION_TARGET_DATABASE;
    if (strcmp(value, "repository") == 0) return YAI_OBSERVATION_TARGET_REPOSITORY;
    return YAI_OBSERVATION_TARGET_UNKNOWN;
}
