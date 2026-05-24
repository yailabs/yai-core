/*
 * YAI - Gate outcome vocabulary
 *
 * Purpose:
 *   Names control outcomes before any carrier dispatch happens.
 *
 * Ownership:
 *   Gate outcome enum and string conversion ABI.
 *
 * Boundary:
 *   Gate outcome is not carrier execution status; decision is not execution.
 *
 * Status:
 *   active
 */

#ifndef YAI_CONTROL_GATE_OUTCOME_H
#define YAI_CONTROL_GATE_OUTCOME_H

typedef enum yai_gate_outcome {
    YAI_GATE_OUTCOME_ALLOW = 0,
    YAI_GATE_OUTCOME_DENY = 1,
    YAI_GATE_OUTCOME_DEFER = 2,
    YAI_GATE_OUTCOME_OBSERVE_ONLY = 3,
    YAI_GATE_OUTCOME_REQUIRE_REVIEW = 4,
    YAI_GATE_OUTCOME_REQUIRE_EVIDENCE = 5,
    YAI_GATE_OUTCOME_REQUIRE_REDACTION = 6,
    YAI_GATE_OUTCOME_ALLOW_WITH_CONSTRAINTS = 7,
    YAI_GATE_OUTCOME_QUARANTINE = 8,
    YAI_GATE_OUTCOME_UNKNOWN = 9
} yai_gate_outcome_t;

const char *yai_gate_outcome_to_string(yai_gate_outcome_t outcome);
yai_gate_outcome_t yai_gate_outcome_from_string(const char *value);

#endif
