/*
 * YAI - Observation target
 *
 * Purpose:
 *   Names host/external target families that can be observed independently.
 *
 * Ownership:
 *   Observation target family enum and string conversion ABI.
 *
 * Boundary:
 *   Observation target classification is not enforcement and not execution.
 *
 * Status:
 *   active
 */

#ifndef YAI_OBSERVATION_OBSERVATION_TARGET_H
#define YAI_OBSERVATION_OBSERVATION_TARGET_H

typedef enum yai_observation_target_family {
    YAI_OBSERVATION_TARGET_PROCESS = 0,
    YAI_OBSERVATION_TARGET_FILESYSTEM = 1,
    YAI_OBSERVATION_TARGET_NETWORK = 2,
    YAI_OBSERVATION_TARGET_DATABASE = 3,
    YAI_OBSERVATION_TARGET_REPOSITORY = 4,
    YAI_OBSERVATION_TARGET_UNKNOWN = 5
} yai_observation_target_family_t;

const char *yai_observation_target_family_to_string(yai_observation_target_family_t target);
yai_observation_target_family_t yai_observation_target_family_from_string(const char *value);

#endif
