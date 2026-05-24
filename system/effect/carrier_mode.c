/*
 * YAI - Carrier mode
 *
 * Implements:
 *   Carrier mode string conversion.
 *
 * This file owns:
 *   controlled/observed/imported vocabulary conversion.
 *
 * This file does not own:
 *   Carrier execution, outcomes or dispatch status.
 */

#include "yai/effect/carrier_mode.h"

#include <string.h>

const char *yai_carrier_mode_to_string(yai_carrier_mode_t mode) {
    switch (mode) {
    case YAI_CARRIER_MODE_CONTROLLED:
        return "controlled";
    case YAI_CARRIER_MODE_OBSERVED:
        return "observed";
    case YAI_CARRIER_MODE_IMPORTED:
        return "imported";
    case YAI_CARRIER_MODE_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_carrier_mode_t yai_carrier_mode_from_string(const char *value) {
    if (value == 0) return YAI_CARRIER_MODE_UNKNOWN;
    if (strcmp(value, "controlled") == 0) return YAI_CARRIER_MODE_CONTROLLED;
    if (strcmp(value, "observed") == 0) return YAI_CARRIER_MODE_OBSERVED;
    if (strcmp(value, "imported") == 0) return YAI_CARRIER_MODE_IMPORTED;
    return YAI_CARRIER_MODE_UNKNOWN;
}
