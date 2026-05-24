/*
 * YAI - Carrier outcome
 *
 * Purpose:
 *   Names the outcome of a controlled carrier realization.
 *
 * Ownership:
 *   Carrier outcome enum and string conversion ABI.
 *
 * Boundary:
 *   This file does not own gate outcomes, dispatch status or receipt guarantee
 *   modes.
 *
 * Status:
 *   active
 */

#ifndef YAI_EFFECT_CARRIER_OUTCOME_H
#define YAI_EFFECT_CARRIER_OUTCOME_H

typedef enum yai_carrier_outcome {
    YAI_CARRIER_OUTCOME_EXECUTED = 0,
    YAI_CARRIER_OUTCOME_BLOCKED = 1,
    YAI_CARRIER_OUTCOME_DEFERRED = 2,
    YAI_CARRIER_OUTCOME_OBSERVED = 3,
    YAI_CARRIER_OUTCOME_FAILED = 4,
    YAI_CARRIER_OUTCOME_QUARANTINED = 5,
    YAI_CARRIER_OUTCOME_WAITING_OPERATOR = 6,
    YAI_CARRIER_OUTCOME_WAITING_AGENT = 7,
    YAI_CARRIER_OUTCOME_NOT_ATTEMPTED = 8,
    YAI_CARRIER_OUTCOME_MISMATCH = 9,
    YAI_CARRIER_OUTCOME_UNKNOWN = 10
} yai_carrier_outcome_t;

const char *yai_carrier_outcome_to_string(yai_carrier_outcome_t outcome);
yai_carrier_outcome_t yai_carrier_outcome_from_string(const char *value);

#endif
