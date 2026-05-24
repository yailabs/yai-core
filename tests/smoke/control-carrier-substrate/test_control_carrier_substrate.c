/*
 * YAI - SPINE.33A control/carrier substrate smoke
 *
 * Implements:
 *   Validation for low-level carrier/control/dispatch/receipt/observation
 *   posture primitives.
 *
 * This file owns:
 *   Roundtrip and initialization smoke coverage for SPINE.33A primitives.
 *
 * This file does not own:
 *   Carrier execution, dispatch queues or runtime scheduling.
 */

#include "yai/control/gate_outcome.h"
#include "yai/effect/carrier_family.h"
#include "yai/effect/dispatch.h"
#include "yai/effect/receipt_guarantee.h"
#include "yai/observation/host_observation.h"

#include <stdio.h>
#include <string.h>

static int require_string(const char *actual, const char *expected) {
    if (!actual || strcmp(actual, expected) != 0) {
        fprintf(stderr, "expected %s got %s\n", expected, actual ? actual : "(null)");
        return 1;
    }
    return 0;
}

static int check_carrier_family(void) {
    const yai_carrier_family_t values[] = {
        YAI_CARRIER_FAMILY_FILESYSTEM,
        YAI_CARRIER_FAMILY_PROCESS,
        YAI_CARRIER_FAMILY_NETWORK_HTTP,
        YAI_CARRIER_FAMILY_DATABASE,
        YAI_CARRIER_FAMILY_REPOSITORY_GIT,
        YAI_CARRIER_FAMILY_MODEL_PROVIDER,
        YAI_CARRIER_FAMILY_OBSERVATION,
        YAI_CARRIER_FAMILY_REVIEW,
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        const char *name = yai_carrier_family_to_string(values[i]);
        if (yai_carrier_family_from_string(name) != values[i]) return 1;
    }
    if (yai_carrier_family_from_string("nope") != YAI_CARRIER_FAMILY_UNKNOWN) return 1;
    printf("carrier_family:roundtrip ok\n");
    return 0;
}

static int check_gate_outcome(void) {
    const yai_gate_outcome_t values[] = {
        YAI_GATE_OUTCOME_ALLOW,
        YAI_GATE_OUTCOME_DENY,
        YAI_GATE_OUTCOME_DEFER,
        YAI_GATE_OUTCOME_OBSERVE_ONLY,
        YAI_GATE_OUTCOME_REQUIRE_REVIEW,
        YAI_GATE_OUTCOME_REQUIRE_EVIDENCE,
        YAI_GATE_OUTCOME_REQUIRE_REDACTION,
        YAI_GATE_OUTCOME_ALLOW_WITH_CONSTRAINTS,
        YAI_GATE_OUTCOME_QUARANTINE,
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        const char *name = yai_gate_outcome_to_string(values[i]);
        if (yai_gate_outcome_from_string(name) != values[i]) return 1;
    }
    if (yai_gate_outcome_from_string("nope") != YAI_GATE_OUTCOME_UNKNOWN) return 1;
    printf("gate_outcome:roundtrip ok\n");
    return 0;
}

static int check_dispatch_status(void) {
    const yai_dispatch_status_t values[] = {
        YAI_DISPATCH_PENDING,
        YAI_DISPATCH_ROUTABLE,
        YAI_DISPATCH_DISPATCHED,
        YAI_DISPATCH_BLOCKED,
        YAI_DISPATCH_DEFERRED,
        YAI_DISPATCH_FAILED,
        YAI_DISPATCH_NOT_SUPPORTED,
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        const char *name = yai_dispatch_status_to_string(values[i]);
        if (yai_dispatch_status_from_string(name) != values[i]) return 1;
    }
    if (yai_dispatch_status_from_string("nope") != YAI_DISPATCH_UNKNOWN) return 1;
    printf("dispatch_status:roundtrip ok\n");
    return 0;
}

static int check_receipt_guarantee(void) {
    const yai_receipt_guarantee_mode_t values[] = {
        YAI_RECEIPT_GUARANTEE_OBSERVED,
        YAI_RECEIPT_GUARANTEE_INTERPOSED,
        YAI_RECEIPT_GUARANTEE_CARRIER_OWNED,
        YAI_RECEIPT_GUARANTEE_EMBEDDED,
        YAI_RECEIPT_GUARANTEE_EXTERNAL_IMPORT,
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        const char *name = yai_receipt_guarantee_mode_to_string(values[i]);
        if (yai_receipt_guarantee_mode_from_string(name) != values[i]) return 1;
    }
    if (yai_receipt_guarantee_mode_from_string("nope") != YAI_RECEIPT_GUARANTEE_UNKNOWN) return 1;
    printf("receipt_guarantee:roundtrip ok\n");
    return 0;
}

static int check_host_observation(void) {
    const yai_host_observation_posture_t values[] = {
        YAI_HOST_OBSERVATION_NOT_OBSERVED,
        YAI_HOST_OBSERVATION_OBSERVED,
        YAI_HOST_OBSERVATION_MISMATCH,
        YAI_HOST_OBSERVATION_BYPASS_SUSPECTED,
        YAI_HOST_OBSERVATION_UNTRACKED_EFFECT,
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        const char *name = yai_host_observation_posture_to_string(values[i]);
        if (yai_host_observation_posture_from_string(name) != values[i]) return 1;
    }
    if (yai_host_observation_posture_from_string("nope") != YAI_HOST_OBSERVATION_UNKNOWN) return 1;
    printf("host_observation:roundtrip ok\n");
    return 0;
}

static int check_dispatch_init(void) {
    yai_dispatch_t dispatch;
    if (yai_dispatch_init(&dispatch) != YAI_OK) return 1;
    if (dispatch.status != YAI_DISPATCH_PENDING) return 1;
    if (dispatch.family != YAI_CARRIER_FAMILY_UNKNOWN) return 1;
    if (require_string(dispatch.route, "") != 0) return 1;
    if (require_string(dispatch.reason, "") != 0) return 1;
    printf("dispatch:init ok\n");
    return 0;
}

int main(void) {
    if (check_carrier_family() != 0) return 1;
    if (check_gate_outcome() != 0) return 1;
    if (check_dispatch_status() != 0) return 1;
    if (check_receipt_guarantee() != 0) return 1;
    if (check_host_observation() != 0) return 1;
    if (check_dispatch_init() != 0) return 1;
    printf("carrier_families:listed\n");
    printf("no_execution:confirmed\n");
    return 0;
}
