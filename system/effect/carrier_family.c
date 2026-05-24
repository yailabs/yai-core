/*
 * YAI - Carrier family vocabulary
 *
 * Implements:
 *   String conversion for control/carrier substrate family names.
 *
 * This file owns:
 *   Carrier family vocabulary conversion.
 *
 * This file does not own:
 *   Carrier execution, readiness or dispatch.
 */

#include "yai/effect/carrier_family.h"

#include <string.h>

const char *yai_carrier_family_to_string(yai_carrier_family_t family) {
    switch (family) {
    case YAI_CARRIER_FAMILY_FILESYSTEM:
        return "filesystem";
    case YAI_CARRIER_FAMILY_PROCESS:
        return "process";
    case YAI_CARRIER_FAMILY_NETWORK_HTTP:
        return "network_http";
    case YAI_CARRIER_FAMILY_DATABASE:
        return "database";
    case YAI_CARRIER_FAMILY_REPOSITORY_GIT:
        return "repository_git";
    case YAI_CARRIER_FAMILY_SERVICE:
        return "service";
    case YAI_CARRIER_FAMILY_ENDPOINT:
        return "endpoint";
    case YAI_CARRIER_FAMILY_SOCKET:
        return "socket";
    case YAI_CARRIER_FAMILY_LISTENER:
        return "listener";
    case YAI_CARRIER_FAMILY_MODEL_PROVIDER:
        return "model_provider";
    case YAI_CARRIER_FAMILY_OBSERVATION:
        return "observation";
    case YAI_CARRIER_FAMILY_REVIEW:
        return "review";
    case YAI_CARRIER_FAMILY_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_carrier_family_t yai_carrier_family_from_string(const char *value) {
    if (value == 0) {
        return YAI_CARRIER_FAMILY_UNKNOWN;
    }
    if (strcmp(value, "filesystem") == 0) return YAI_CARRIER_FAMILY_FILESYSTEM;
    if (strcmp(value, "process") == 0) return YAI_CARRIER_FAMILY_PROCESS;
    if (strcmp(value, "network_http") == 0) return YAI_CARRIER_FAMILY_NETWORK_HTTP;
    if (strcmp(value, "database") == 0) return YAI_CARRIER_FAMILY_DATABASE;
    if (strcmp(value, "repository_git") == 0) return YAI_CARRIER_FAMILY_REPOSITORY_GIT;
    if (strcmp(value, "service") == 0) return YAI_CARRIER_FAMILY_SERVICE;
    if (strcmp(value, "endpoint") == 0) return YAI_CARRIER_FAMILY_ENDPOINT;
    if (strcmp(value, "socket") == 0) return YAI_CARRIER_FAMILY_SOCKET;
    if (strcmp(value, "listener") == 0) return YAI_CARRIER_FAMILY_LISTENER;
    if (strcmp(value, "model_provider") == 0) return YAI_CARRIER_FAMILY_MODEL_PROVIDER;
    if (strcmp(value, "observation") == 0) return YAI_CARRIER_FAMILY_OBSERVATION;
    if (strcmp(value, "review") == 0) return YAI_CARRIER_FAMILY_REVIEW;
    return YAI_CARRIER_FAMILY_UNKNOWN;
}
