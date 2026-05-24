/*
 * YAI - Host observation posture
 *
 * Purpose:
 *   Names what YAI observed about host or external effects.
 *
 * Ownership:
 *   Host observation posture enum and string conversion ABI.
 *
 * Boundary:
 *   Observation is not enforcement and not carrier execution.
 *
 * Status:
 *   active
 */

#ifndef YAI_OBSERVATION_HOST_OBSERVATION_H
#define YAI_OBSERVATION_HOST_OBSERVATION_H

typedef enum yai_host_observation_posture {
    YAI_HOST_OBSERVATION_NOT_OBSERVED = 0,
    YAI_HOST_OBSERVATION_OBSERVED = 1,
    YAI_HOST_OBSERVATION_MISMATCH = 2,
    YAI_HOST_OBSERVATION_BYPASS_SUSPECTED = 3,
    YAI_HOST_OBSERVATION_UNTRACKED_EFFECT = 4,
    YAI_HOST_OBSERVATION_UNKNOWN = 5
} yai_host_observation_posture_t;

const char *yai_host_observation_posture_to_string(yai_host_observation_posture_t posture);
yai_host_observation_posture_t yai_host_observation_posture_from_string(const char *value);

#endif
