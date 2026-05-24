/*
 * YAI - Carrier family vocabulary
 *
 * Purpose:
 *   Names the supported control/carrier substrate families.
 *
 * Ownership:
 *   Canonical carrier family enum and string conversion ABI.
 *
 * Boundary:
 *   This file does not execute carriers or claim carrier readiness.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_CARRIER_FAMILY_H
#define YAI_EFFECT_CARRIER_FAMILY_H

typedef enum yai_carrier_family {
    YAI_CARRIER_FAMILY_FILESYSTEM = 0,
    YAI_CARRIER_FAMILY_PROCESS = 1,
    YAI_CARRIER_FAMILY_NETWORK_HTTP = 2,
    YAI_CARRIER_FAMILY_DATABASE = 3,
    YAI_CARRIER_FAMILY_REPOSITORY_GIT = 4,
    YAI_CARRIER_FAMILY_MODEL_PROVIDER = 5,
    YAI_CARRIER_FAMILY_OBSERVATION = 6,
    YAI_CARRIER_FAMILY_REVIEW = 7,
    YAI_CARRIER_FAMILY_UNKNOWN = 8
} yai_carrier_family_t;

const char *yai_carrier_family_to_string(yai_carrier_family_t family);
yai_carrier_family_t yai_carrier_family_from_string(const char *value);

#endif
