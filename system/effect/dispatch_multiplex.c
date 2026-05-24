/*
 * YAI - Dispatch multiplex
 *
 * Implements:
 *   Carrier-family route helper for dispatch planning.
 *
 * This file owns:
 *   The SPINE.33B no-execution multiplex entrypoint.
 *
 * This file does not own:
 *   Carrier queues, workers, execution, receipt generation or host probes.
 */

#include "yai/effect/dispatch_multiplex.h"

yai_status_t yai_dispatch_multiplex_route(
    yai_carrier_family_t family,
    yai_dispatch_plan_t *out
) {
    return yai_dispatch_plan_for_carrier_family(family, out);
}
