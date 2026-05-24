/*
 * YAI - Carrier coverage
 *
 * Purpose:
 *   Exposes the static carrier family/mode/outcome coverage matrix.
 *
 * Ownership:
 *   Carrier coverage matrix ABI for SPINE.33F.
 *
 * Boundary:
 *   Coverage is visibility, not execution readiness or carrier implementation.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_CARRIER_COVERAGE_H
#define YAI_EFFECT_CARRIER_COVERAGE_H

#include <stddef.h>

#include "yai/effect/carrier_family.h"
#include "yai/effect/carrier_mode.h"

typedef enum yai_carrier_adapter_status {
    YAI_CARRIER_ADAPTER_ACTIVE_MINIMAL = 0,
    YAI_CARRIER_ADAPTER_SKELETON = 1,
    YAI_CARRIER_ADAPTER_PLANNED = 2,
    YAI_CARRIER_ADAPTER_UNSUPPORTED = 3,
    YAI_CARRIER_ADAPTER_DEFERRED = 4,
    YAI_CARRIER_ADAPTER_UNKNOWN = 5
} yai_carrier_adapter_status_t;

typedef struct yai_carrier_coverage {
    yai_carrier_family_t family;
    yai_carrier_adapter_status_t controlled_status;
    yai_carrier_adapter_status_t observed_status;
    yai_carrier_adapter_status_t imported_status;
    const char *execution_available;
    const char *execution_scope;
    int receipt_required;
    const char *carrier_contract;
    const char *supported_outcomes;
    const char *notes;
} yai_carrier_coverage_t;

const char *yai_carrier_adapter_status_to_string(yai_carrier_adapter_status_t status);
yai_carrier_adapter_status_t yai_carrier_adapter_status_from_string(const char *value);
const yai_carrier_coverage_t *yai_carrier_coverage_entries(size_t *count);
const yai_carrier_coverage_t *yai_carrier_coverage_for_family(yai_carrier_family_t family);

#endif
