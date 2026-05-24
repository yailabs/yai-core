/*
 * YAI - Carrier skeleton registry
 *
 * Purpose:
 *   Exposes inspectable non-process carrier.v1 skeleton metadata.
 *
 * Ownership:
 *   Static skeleton registry for planned carrier families.
 *
 * Boundary:
 *   This file does not execute network, database, git, service, endpoint,
 *   socket, listener, model-provider or review effects.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_CARRIER_SKELETON_H
#define YAI_EFFECT_CARRIER_SKELETON_H

#include <stddef.h>

#include "yai/effect/carrier_coverage.h"
#include "yai/effect/carrier_family.h"

typedef struct yai_carrier_skeleton {
    yai_carrier_family_t carrier_family;
    const char *carrier_name;
    const char *contract_version;
    yai_carrier_adapter_status_t adapter_status;
    yai_carrier_adapter_status_t controlled_mode_status;
    yai_carrier_adapter_status_t observed_mode_status;
    yai_carrier_adapter_status_t imported_mode_status;
    int execution_available;
    int receipt_required;
    int supports_inspect;
    const char *non_execution_reason;
    const char *future_activation_wave;
} yai_carrier_skeleton_t;

const yai_carrier_skeleton_t *yai_carrier_skeleton_entries(size_t *count);
const yai_carrier_skeleton_t *yai_carrier_skeleton_for_family(yai_carrier_family_t family);

#endif
