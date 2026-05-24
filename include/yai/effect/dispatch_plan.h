/*
 * YAI - Dispatch plan
 *
 * Purpose:
 *   Describes where a carrier-family operation would be routed.
 *
 * Ownership:
 *   Inspectable dispatch plan struct and plan initialization ABI.
 *
 * Boundary:
 *   This file does not execute carriers, enqueue work or attest receipts.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_DISPATCH_PLAN_H
#define YAI_EFFECT_DISPATCH_PLAN_H

#include <stdbool.h>

#include "yai/base/error.h"
#include "yai/effect/carrier_family.h"
#include "yai/effect/dispatch.h"
#include "yai/effect/dispatch_lane.h"

typedef struct yai_dispatch_plan {
    char dispatch_ref[64];
    yai_carrier_family_t carrier_family;
    yai_dispatch_lane_t lane;
    yai_dispatch_status_t dispatch_status;
    const char *lane_status;
    yai_ordering_policy_t ordering_policy;
    yai_capacity_policy_t capacity_policy;
    yai_lock_policy_t lock_policy;
    yai_timeout_policy_t timeout_policy;
    yai_retry_policy_t retry_policy;
    const char *receipt_requirement;
    const char *failure_mode;
    bool execution_performed;
} yai_dispatch_plan_t;

yai_status_t yai_dispatch_plan_init(yai_dispatch_plan_t *plan);
yai_status_t yai_dispatch_plan_for_carrier_family(
    yai_carrier_family_t family,
    yai_dispatch_plan_t *out
);

#endif
