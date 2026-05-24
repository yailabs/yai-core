/*
 * YAI - Dispatch posture
 *
 * Purpose:
 *   Describes whether an op/control candidate is dispatchable.
 *
 * Ownership:
 *   Dispatch status enum, string conversion ABI and minimal dispatch struct
 *   initialization.
 *
 * Boundary:
 *   This file does not own a queue, scheduler, carrier execution or receipt
 *   status.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_DISPATCH_H
#define YAI_EFFECT_DISPATCH_H

#include "yai/base/error.h"
#include "yai/effect/carrier_family.h"

typedef enum yai_dispatch_status {
    YAI_DISPATCH_PENDING = 0,
    YAI_DISPATCH_ROUTABLE = 1,
    YAI_DISPATCH_DISPATCHED = 2,
    YAI_DISPATCH_BLOCKED = 3,
    YAI_DISPATCH_DEFERRED = 4,
    YAI_DISPATCH_FAILED = 5,
    YAI_DISPATCH_NOT_SUPPORTED = 6,
    YAI_DISPATCH_UNKNOWN = 7
} yai_dispatch_status_t;

typedef struct yai_dispatch {
    yai_dispatch_status_t status;
    yai_carrier_family_t family;
    char route[64];
    char reason[128];
} yai_dispatch_t;

const char *yai_dispatch_status_to_string(yai_dispatch_status_t status);
yai_dispatch_status_t yai_dispatch_status_from_string(const char *value);
yai_status_t yai_dispatch_init(yai_dispatch_t *dispatch);

#endif
