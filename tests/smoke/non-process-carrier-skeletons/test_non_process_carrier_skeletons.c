/*
 * YAI - Non-process carrier skeleton smoke
 *
 * Purpose:
 *   Validate inspectable carrier.v1 skeletons for non-process families.
 *
 * Ownership:
 *   SPINE.33G smoke coverage for carrier skeleton registry.
 *
 * Boundary:
 *   This test does not execute carrier effects.
 *
 * Status:
 *   test_only
 */

#include "yai/yai.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void require_skeleton(yai_carrier_family_t family, const char *name) {
    const yai_carrier_skeleton_t *skeleton = yai_carrier_skeleton_for_family(family);
    const yai_carrier_coverage_t *coverage = yai_carrier_coverage_for_family(family);

    assert(skeleton != 0);
    assert(coverage != 0);
    assert(skeleton->carrier_family == family);
    assert(strcmp(skeleton->carrier_name, name) == 0);
    assert(strcmp(skeleton->contract_version, "carrier.v1") == 0);
    assert(skeleton->adapter_status == YAI_CARRIER_ADAPTER_SKELETON);
    assert(skeleton->execution_available == 0);
    assert(skeleton->receipt_required == 1);
    assert(skeleton->supports_inspect == 1);
    assert(coverage->family == family);
    assert(strcmp(coverage->execution_available, "false") == 0);
    assert(coverage->receipt_required == 1);
    assert(strcmp(coverage->carrier_contract, "carrier.v1") == 0);
    printf("carrier_skeleton:%s ok\n", name);
}

int main(void) {
    size_t count = 0;
    const yai_carrier_skeleton_t *entries = yai_carrier_skeleton_entries(&count);

    assert(entries != 0);
    assert(count == 9);

    require_skeleton(YAI_CARRIER_FAMILY_NETWORK_HTTP, "network_http");
    require_skeleton(YAI_CARRIER_FAMILY_DATABASE, "database");
    require_skeleton(YAI_CARRIER_FAMILY_REPOSITORY_GIT, "repository_git");
    require_skeleton(YAI_CARRIER_FAMILY_SERVICE, "service");
    require_skeleton(YAI_CARRIER_FAMILY_ENDPOINT, "endpoint");
    require_skeleton(YAI_CARRIER_FAMILY_SOCKET, "socket");
    require_skeleton(YAI_CARRIER_FAMILY_LISTENER, "listener");
    require_skeleton(YAI_CARRIER_FAMILY_MODEL_PROVIDER, "model_provider");
    require_skeleton(YAI_CARRIER_FAMILY_REVIEW, "review");

    assert(yai_carrier_skeleton_for_family(YAI_CARRIER_FAMILY_FILESYSTEM) == 0);
    assert(yai_carrier_skeleton_for_family(YAI_CARRIER_FAMILY_PROCESS) == 0);
    assert(yai_carrier_skeleton_for_family(YAI_CARRIER_FAMILY_OBSERVATION) == 0);

    printf("no_execution:confirmed\n");
    return 0;
}
