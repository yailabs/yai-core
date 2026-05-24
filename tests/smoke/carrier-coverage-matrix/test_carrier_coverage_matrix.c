/*
 * YAI - Carrier coverage matrix smoke
 *
 * Purpose:
 *   Validate carrier mode taxonomy and static coverage matrix visibility.
 *
 * Ownership:
 *   SPINE.33F smoke coverage for carrier coverage matrix.
 *
 * Boundary:
 *   This test does not execute skeleton carriers.
 *
 * Status:
 *   test_only
 */

#include "yai/yai.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void require_family(yai_carrier_family_t family) {
    const yai_carrier_coverage_t *entry = yai_carrier_coverage_for_family(family);
    assert(entry != 0);
    assert(entry->family == family);
}

static void require_skeleton_no_execution(yai_carrier_family_t family) {
    const yai_carrier_coverage_t *entry = yai_carrier_coverage_for_family(family);
    assert(entry != 0);
    assert(entry->controlled_status == YAI_CARRIER_ADAPTER_SKELETON);
    assert(strcmp(entry->execution_available, "false") == 0);
}

int main(void) {
    size_t count = 0;
    const yai_carrier_coverage_t *entries = yai_carrier_coverage_entries(&count);
    const yai_carrier_coverage_t *filesystem = 0;
    const yai_carrier_coverage_t *process = 0;
    const yai_carrier_coverage_t *unknown = 0;

    assert(entries != 0);
    assert(count == 13);
    assert(yai_carrier_mode_from_string("controlled") == YAI_CARRIER_MODE_CONTROLLED);
    assert(yai_carrier_mode_from_string("observed") == YAI_CARRIER_MODE_OBSERVED);
    assert(yai_carrier_mode_from_string("imported") == YAI_CARRIER_MODE_IMPORTED);
    printf("carrier_modes:controlled observed imported ok\n");

    require_family(YAI_CARRIER_FAMILY_FILESYSTEM);
    require_family(YAI_CARRIER_FAMILY_PROCESS);
    require_family(YAI_CARRIER_FAMILY_NETWORK_HTTP);
    require_family(YAI_CARRIER_FAMILY_DATABASE);
    require_family(YAI_CARRIER_FAMILY_REPOSITORY_GIT);
    require_family(YAI_CARRIER_FAMILY_SERVICE);
    require_family(YAI_CARRIER_FAMILY_ENDPOINT);
    require_family(YAI_CARRIER_FAMILY_SOCKET);
    require_family(YAI_CARRIER_FAMILY_LISTENER);
    require_family(YAI_CARRIER_FAMILY_MODEL_PROVIDER);
    require_family(YAI_CARRIER_FAMILY_OBSERVATION);
    require_family(YAI_CARRIER_FAMILY_REVIEW);
    require_family(YAI_CARRIER_FAMILY_UNKNOWN);
    printf("carrier_coverage:families ok\n");

    filesystem = yai_carrier_coverage_for_family(YAI_CARRIER_FAMILY_FILESYSTEM);
    assert(filesystem->controlled_status == YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL);
    printf("coverage:filesystem active_minimal\n");

    process = yai_carrier_coverage_for_family(YAI_CARRIER_FAMILY_PROCESS);
    assert(process->controlled_status == YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL);
    assert(process->observed_status == YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL);
    assert(strcmp(process->execution_available, "true_limited") == 0);
    printf("coverage:process active_minimal\n");

    require_skeleton_no_execution(YAI_CARRIER_FAMILY_NETWORK_HTTP);
    require_skeleton_no_execution(YAI_CARRIER_FAMILY_DATABASE);
    require_skeleton_no_execution(YAI_CARRIER_FAMILY_REPOSITORY_GIT);
    require_skeleton_no_execution(YAI_CARRIER_FAMILY_SERVICE);
    require_skeleton_no_execution(YAI_CARRIER_FAMILY_ENDPOINT);
    require_skeleton_no_execution(YAI_CARRIER_FAMILY_SOCKET);
    require_skeleton_no_execution(YAI_CARRIER_FAMILY_LISTENER);
    printf("coverage:database skeleton\n");

    unknown = yai_carrier_coverage_for_family(YAI_CARRIER_FAMILY_UNKNOWN);
    assert(unknown->controlled_status == YAI_CARRIER_ADAPTER_UNSUPPORTED);
    assert(unknown->observed_status == YAI_CARRIER_ADAPTER_UNSUPPORTED);
    assert(unknown->imported_status == YAI_CARRIER_ADAPTER_UNSUPPORTED);
    printf("coverage:unknown unsupported\n");

    assert(yai_carrier_outcome_from_string("mismatch") == YAI_CARRIER_OUTCOME_MISMATCH);
    printf("coverage:no_execution confirmed\n");
    return 0;
}
