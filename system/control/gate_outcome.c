/*
 * YAI - Gate outcome vocabulary
 *
 * Implements:
 *   String conversion for gate outcomes.
 *
 * This file owns:
 *   Control outcome vocabulary conversion.
 *
 * This file does not own:
 *   Decision authority, dispatch status or carrier execution.
 */

#include "yai/control/gate_outcome.h"

#include <string.h>

const char *yai_gate_outcome_to_string(yai_gate_outcome_t outcome) {
    switch (outcome) {
    case YAI_GATE_OUTCOME_ALLOW:
        return "allow";
    case YAI_GATE_OUTCOME_DENY:
        return "deny";
    case YAI_GATE_OUTCOME_DEFER:
        return "defer";
    case YAI_GATE_OUTCOME_OBSERVE_ONLY:
        return "observe_only";
    case YAI_GATE_OUTCOME_REQUIRE_REVIEW:
        return "require_review";
    case YAI_GATE_OUTCOME_REQUIRE_EVIDENCE:
        return "require_evidence";
    case YAI_GATE_OUTCOME_REQUIRE_REDACTION:
        return "require_redaction";
    case YAI_GATE_OUTCOME_ALLOW_WITH_CONSTRAINTS:
        return "allow_with_constraints";
    case YAI_GATE_OUTCOME_QUARANTINE:
        return "quarantine";
    case YAI_GATE_OUTCOME_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_gate_outcome_t yai_gate_outcome_from_string(const char *value) {
    if (value == 0) {
        return YAI_GATE_OUTCOME_UNKNOWN;
    }
    if (strcmp(value, "allow") == 0) return YAI_GATE_OUTCOME_ALLOW;
    if (strcmp(value, "deny") == 0) return YAI_GATE_OUTCOME_DENY;
    if (strcmp(value, "defer") == 0) return YAI_GATE_OUTCOME_DEFER;
    if (strcmp(value, "observe_only") == 0) return YAI_GATE_OUTCOME_OBSERVE_ONLY;
    if (strcmp(value, "require_review") == 0) return YAI_GATE_OUTCOME_REQUIRE_REVIEW;
    if (strcmp(value, "require_evidence") == 0) return YAI_GATE_OUTCOME_REQUIRE_EVIDENCE;
    if (strcmp(value, "require_redaction") == 0) return YAI_GATE_OUTCOME_REQUIRE_REDACTION;
    if (strcmp(value, "allow_with_constraints") == 0) return YAI_GATE_OUTCOME_ALLOW_WITH_CONSTRAINTS;
    if (strcmp(value, "quarantine") == 0) return YAI_GATE_OUTCOME_QUARANTINE;
    return YAI_GATE_OUTCOME_UNKNOWN;
}
