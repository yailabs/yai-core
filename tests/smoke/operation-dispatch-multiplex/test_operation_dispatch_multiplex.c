/*
 * YAI - SPINE.33B operation dispatch multiplex smoke
 *
 * Implements:
 *   Validation for dispatch lane, plan and multiplex route helpers.
 *
 * This file owns:
 *   No-execution smoke coverage for operation dispatch planning.
 *
 * This file does not own:
 *   Carrier execution, process control, real queues or receipt hardening.
 */

#include "yai/effect/carrier_family.h"
#include "yai/effect/dispatch.h"
#include "yai/effect/dispatch_lane.h"
#include "yai/effect/dispatch_multiplex.h"
#include "yai/effect/dispatch_plan.h"

#include <stdio.h>
#include <string.h>

static int require_string(const char *actual, const char *expected) {
    if (!actual || strcmp(actual, expected) != 0) {
        fprintf(stderr, "expected %s got %s\n", expected, actual ? actual : "(null)");
        return 1;
    }
    return 0;
}

static int check_lane_roundtrip(void) {
    const yai_dispatch_lane_t values[] = {
        YAI_DISPATCH_LANE_FILESYSTEM,
        YAI_DISPATCH_LANE_PROCESS,
        YAI_DISPATCH_LANE_NETWORK_HTTP,
        YAI_DISPATCH_LANE_DATABASE,
        YAI_DISPATCH_LANE_REPOSITORY_GIT,
        YAI_DISPATCH_LANE_MODEL_PROVIDER,
        YAI_DISPATCH_LANE_OBSERVATION,
        YAI_DISPATCH_LANE_REVIEW,
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        const char *name = yai_dispatch_lane_to_string(values[i]);
        if (yai_dispatch_lane_from_string(name) != values[i]) return 1;
        if (yai_dispatch_lane_carrier_family(values[i]) == YAI_CARRIER_FAMILY_UNKNOWN) return 1;
    }
    if (yai_dispatch_lane_from_string("nope") != YAI_DISPATCH_LANE_UNKNOWN) return 1;
    printf("dispatch_lane:roundtrip ok\n");
    return 0;
}

static int check_plan_init(void) {
    yai_dispatch_plan_t plan;
    if (yai_dispatch_plan_init(&plan) != YAI_OK) return 1;
    if (plan.carrier_family != YAI_CARRIER_FAMILY_UNKNOWN) return 1;
    if (plan.lane != YAI_DISPATCH_LANE_UNKNOWN) return 1;
    if (plan.dispatch_status != YAI_DISPATCH_NOT_SUPPORTED) return 1;
    if (plan.execution_performed) return 1;
    if (require_string(plan.receipt_requirement, "not_available") != 0) return 1;
    printf("dispatch_plan:init ok\n");
    return 0;
}

static int check_route(
    yai_carrier_family_t family,
    yai_dispatch_lane_t expected_lane,
    const char *label
) {
    yai_dispatch_plan_t plan;
    if (yai_dispatch_multiplex_route(family, &plan) != YAI_OK) return 1;
    if (plan.lane != expected_lane) return 1;
    if (plan.dispatch_status != YAI_DISPATCH_ROUTABLE) return 1;
    if (plan.execution_performed) return 1;
    if (require_string(plan.receipt_requirement, "required") != 0) return 1;
    if (yai_ordering_policy_to_string(plan.ordering_policy)[0] == '\0') return 1;
    if (yai_capacity_policy_to_string(plan.capacity_policy)[0] == '\0') return 1;
    if (yai_lock_policy_to_string(plan.lock_policy)[0] == '\0') return 1;
    if (yai_timeout_policy_to_string(plan.timeout_policy)[0] == '\0') return 1;
    if (yai_retry_policy_to_string(plan.retry_policy)[0] == '\0') return 1;
    printf(
        "route:%s %s routable\n",
        label,
        yai_dispatch_lane_to_string(plan.lane)
    );
    return 0;
}

static int check_unknown_route(void) {
    yai_dispatch_plan_t plan;
    if (yai_dispatch_multiplex_route(YAI_CARRIER_FAMILY_UNKNOWN, &plan) != YAI_OK) return 1;
    if (plan.lane != YAI_DISPATCH_LANE_UNKNOWN) return 1;
    if (plan.dispatch_status != YAI_DISPATCH_NOT_SUPPORTED) return 1;
    if (plan.execution_performed) return 1;
    printf("route:unknown not_supported\n");
    return 0;
}

int main(void) {
    if (check_lane_roundtrip() != 0) return 1;
    if (check_plan_init() != 0) return 1;
    if (check_route(YAI_CARRIER_FAMILY_FILESYSTEM, YAI_DISPATCH_LANE_FILESYSTEM, "filesystem") != 0) return 1;
    if (check_route(YAI_CARRIER_FAMILY_PROCESS, YAI_DISPATCH_LANE_PROCESS, "process") != 0) return 1;
    if (check_route(YAI_CARRIER_FAMILY_NETWORK_HTTP, YAI_DISPATCH_LANE_NETWORK_HTTP, "network_http") != 0) return 1;
    if (check_route(YAI_CARRIER_FAMILY_DATABASE, YAI_DISPATCH_LANE_DATABASE, "database") != 0) return 1;
    if (check_route(YAI_CARRIER_FAMILY_REPOSITORY_GIT, YAI_DISPATCH_LANE_REPOSITORY_GIT, "repository_git") != 0) return 1;
    if (check_route(YAI_CARRIER_FAMILY_MODEL_PROVIDER, YAI_DISPATCH_LANE_MODEL_PROVIDER, "model_provider") != 0) return 1;
    if (check_route(YAI_CARRIER_FAMILY_OBSERVATION, YAI_DISPATCH_LANE_OBSERVATION, "observation") != 0) return 1;
    if (check_route(YAI_CARRIER_FAMILY_REVIEW, YAI_DISPATCH_LANE_REVIEW, "review") != 0) return 1;
    if (check_unknown_route() != 0) return 1;
    printf("execution:false\n");
    printf("multiplex:no_execution confirmed\n");
    return 0;
}
