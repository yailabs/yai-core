/*
 * YAI - Carrier skeleton registry
 *
 * Implements:
 *   Static inspectable carrier.v1 skeleton metadata for non-process carriers.
 *
 * This file owns:
 *   Skeleton status, receipt requirement and non-execution posture.
 *
 * This file does not own:
 *   Carrier execution, adapter activation, provider invocation or host effects.
 */

#include "yai/effect/carrier_skeleton.h"

static const yai_carrier_skeleton_t skeletons[] = {
    {YAI_CARRIER_FAMILY_NETWORK_HTTP, "network_http", "carrier.v1", YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, 0, 1, 1, "adapter_not_implemented", "planned"},
    {YAI_CARRIER_FAMILY_DATABASE, "database", "carrier.v1", YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, 0, 1, 1, "adapter_not_implemented", "planned"},
    {YAI_CARRIER_FAMILY_REPOSITORY_GIT, "repository_git", "carrier.v1", YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, 0, 1, 1, "adapter_not_implemented", "planned"},
    {YAI_CARRIER_FAMILY_SERVICE, "service", "carrier.v1", YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, 0, 1, 1, "adapter_not_implemented", "planned"},
    {YAI_CARRIER_FAMILY_ENDPOINT, "endpoint", "carrier.v1", YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, 0, 1, 1, "adapter_not_implemented", "planned"},
    {YAI_CARRIER_FAMILY_SOCKET, "socket", "carrier.v1", YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, 0, 1, 1, "adapter_not_implemented", "planned"},
    {YAI_CARRIER_FAMILY_LISTENER, "listener", "carrier.v1", YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_SKELETON, 0, 1, 1, "adapter_not_implemented", "planned"},
    {YAI_CARRIER_FAMILY_MODEL_PROVIDER, "model_provider", "carrier.v1", YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_PLANNED, YAI_CARRIER_ADAPTER_PLANNED, YAI_CARRIER_ADAPTER_SKELETON, 0, 1, 1, "model_provider_carrier_not_implemented", "planned"},
    {YAI_CARRIER_FAMILY_REVIEW, "review", "carrier.v1", YAI_CARRIER_ADAPTER_SKELETON, YAI_CARRIER_ADAPTER_UNSUPPORTED, YAI_CARRIER_ADAPTER_UNSUPPORTED, YAI_CARRIER_ADAPTER_SKELETON, 0, 1, 1, "review_lane_not_implemented", "planned"},
};

const yai_carrier_skeleton_t *yai_carrier_skeleton_entries(size_t *count) {
    if (count != 0) {
        *count = sizeof(skeletons) / sizeof(skeletons[0]);
    }
    return skeletons;
}

const yai_carrier_skeleton_t *yai_carrier_skeleton_for_family(yai_carrier_family_t family) {
    size_t i;
    for (i = 0; i < sizeof(skeletons) / sizeof(skeletons[0]); i += 1) {
        if (skeletons[i].carrier_family == family) {
            return &skeletons[i];
        }
    }
    return 0;
}
