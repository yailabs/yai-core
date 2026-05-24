/*
 * YAI - Carrier outcome
 *
 * Implements:
 *   Carrier outcome string conversion.
 *
 * This file owns:
 *   Controlled carrier outcome vocabulary.
 *
 * This file does not own:
 *   Gate outcomes, dispatch status, receipt guarantees or execution.
 */

#include "yai/effect/carrier_outcome.h"

#include <string.h>

const char *yai_carrier_outcome_to_string(yai_carrier_outcome_t outcome) {
    switch (outcome) {
    case YAI_CARRIER_OUTCOME_EXECUTED:
        return "executed";
    case YAI_CARRIER_OUTCOME_BLOCKED:
        return "blocked";
    case YAI_CARRIER_OUTCOME_DEFERRED:
        return "deferred";
    case YAI_CARRIER_OUTCOME_OBSERVED:
        return "observed";
    case YAI_CARRIER_OUTCOME_FAILED:
        return "failed";
    case YAI_CARRIER_OUTCOME_QUARANTINED:
        return "quarantined";
    case YAI_CARRIER_OUTCOME_WAITING_OPERATOR:
        return "waiting_operator";
    case YAI_CARRIER_OUTCOME_WAITING_AGENT:
        return "waiting_agent";
    case YAI_CARRIER_OUTCOME_NOT_ATTEMPTED:
        return "not_attempted";
    case YAI_CARRIER_OUTCOME_MISMATCH:
        return "mismatch";
    case YAI_CARRIER_OUTCOME_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_carrier_outcome_t yai_carrier_outcome_from_string(const char *value) {
    if (value == 0) return YAI_CARRIER_OUTCOME_UNKNOWN;
    if (strcmp(value, "executed") == 0) return YAI_CARRIER_OUTCOME_EXECUTED;
    if (strcmp(value, "blocked") == 0) return YAI_CARRIER_OUTCOME_BLOCKED;
    if (strcmp(value, "deferred") == 0) return YAI_CARRIER_OUTCOME_DEFERRED;
    if (strcmp(value, "observed") == 0) return YAI_CARRIER_OUTCOME_OBSERVED;
    if (strcmp(value, "failed") == 0) return YAI_CARRIER_OUTCOME_FAILED;
    if (strcmp(value, "quarantined") == 0) return YAI_CARRIER_OUTCOME_QUARANTINED;
    if (strcmp(value, "waiting_operator") == 0) return YAI_CARRIER_OUTCOME_WAITING_OPERATOR;
    if (strcmp(value, "waiting_agent") == 0) return YAI_CARRIER_OUTCOME_WAITING_AGENT;
    if (strcmp(value, "not_attempted") == 0) return YAI_CARRIER_OUTCOME_NOT_ATTEMPTED;
    if (strcmp(value, "mismatch") == 0) return YAI_CARRIER_OUTCOME_MISMATCH;
    return YAI_CARRIER_OUTCOME_UNKNOWN;
}
