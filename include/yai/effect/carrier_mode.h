/*
 * YAI - Carrier mode
 *
 * Purpose:
 *   Names how YAI relates to an effect-capable carrier family.
 *
 * Ownership:
 *   Carrier mode enum and string conversion ABI.
 *
 * Boundary:
 *   Mode is not outcome, carrier family or dispatch status.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_CARRIER_MODE_H
#define YAI_EFFECT_CARRIER_MODE_H

typedef enum yai_carrier_mode {
    YAI_CARRIER_MODE_CONTROLLED = 0,
    YAI_CARRIER_MODE_OBSERVED = 1,
    YAI_CARRIER_MODE_IMPORTED = 2,
    YAI_CARRIER_MODE_UNKNOWN = 3
} yai_carrier_mode_t;

const char *yai_carrier_mode_to_string(yai_carrier_mode_t mode);
yai_carrier_mode_t yai_carrier_mode_from_string(const char *value);

#endif
