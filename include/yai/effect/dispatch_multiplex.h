/*
 * YAI - Dispatch multiplex
 *
 * Purpose:
 *   Routes carrier families to dispatch plans without executing effects.
 *
 * Ownership:
 *   Minimal multiplex route helper ABI.
 *
 * Boundary:
 *   This file does not own a queue, worker pool, carrier implementation or
 *   decision authority.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_DISPATCH_MULTIPLEX_H
#define YAI_EFFECT_DISPATCH_MULTIPLEX_H

#include "yai/effect/dispatch_plan.h"

yai_status_t yai_dispatch_multiplex_route(
    yai_carrier_family_t family,
    yai_dispatch_plan_t *out
);

#endif
