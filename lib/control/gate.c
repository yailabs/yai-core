#include "yai/control/gate.h"

#include <stdio.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_gate_evaluate_attempt(const yai_op_attempt_t *attempt,
                                       const yai_policy_rule_t *rule,
                                       yai_gate_result_t *gate) {
    if (attempt == 0 || rule == 0 || gate == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&gate->gate_id, "gate:operation");
    gate->case_ref = attempt->case_ref;
    gate->attempt_id = attempt->attempt_id;
    gate->rule_id = rule->rule_id;
    gate->gate_kind = YAI_GATE_OPERATION;

    if (yai_case_ref_is_empty(&attempt->case_ref)) {
        yai_id_set(&gate->gate_id, "gate:case");
        gate->gate_kind = YAI_GATE_CASE;
        gate->result = YAI_GATE_BLOCK;
        yai_copy_string(gate->reason, sizeof(gate->reason), "case missing");
        return YAI_OK;
    }

    if (yai_subject_ref_is_empty(&attempt->target_subject_ref)) {
        yai_id_set(&gate->gate_id, "gate:subject");
        gate->gate_kind = YAI_GATE_SUBJECT;
        gate->result = YAI_GATE_BLOCK;
        yai_copy_string(gate->reason, sizeof(gate->reason), "target subject missing");
        return YAI_OK;
    }

    if (rule->effect == YAI_POLICY_EFFECT_DENY) {
        gate->result = YAI_GATE_BLOCK;
    } else if (rule->effect == YAI_POLICY_EFFECT_REQUIRE_REVIEW) {
        gate->result = YAI_GATE_REQUIRE_REVIEW;
    } else if (rule->effect == YAI_POLICY_EFFECT_REQUIRE_EVIDENCE) {
        gate->result = YAI_GATE_REQUIRE_EVIDENCE;
    } else if (rule->effect == YAI_POLICY_EFFECT_ALLOW ||
               rule->effect == YAI_POLICY_EFFECT_OBSERVE_ONLY ||
               rule->effect == YAI_POLICY_EFFECT_REQUIRE_RECEIPT) {
        gate->result = YAI_GATE_PASS;
    } else {
        gate->result = YAI_GATE_DEFER;
    }

    (void)snprintf(gate->reason,
                   sizeof(gate->reason),
                   "%s -> %s",
                   yai_policy_condition_kind_string(rule->condition_kind),
                   yai_policy_effect_string(rule->effect));
    return YAI_OK;
}

const char *yai_gate_kind_string(yai_gate_kind_t kind) {
    switch (kind) {
    case YAI_GATE_CASE:
        return "case";
    case YAI_GATE_SUBJECT:
        return "subject";
    case YAI_GATE_ACTOR:
        return "actor";
    case YAI_GATE_OPERATION:
        return "operation";
    case YAI_GATE_CARRIER:
        return "carrier";
    case YAI_GATE_PROJECTION:
        return "projection";
    case YAI_GATE_RETENTION:
        return "retention";
    default:
        return "unknown";
    }
}

const char *yai_gate_result_string(yai_gate_result_kind_t result) {
    switch (result) {
    case YAI_GATE_PASS:
        return "pass";
    case YAI_GATE_BLOCK:
        return "block";
    case YAI_GATE_REQUIRE_REVIEW:
        return "require_review";
    case YAI_GATE_REQUIRE_EVIDENCE:
        return "require_evidence";
    case YAI_GATE_NOT_APPLICABLE:
        return "not_applicable";
    case YAI_GATE_DEFER:
        return "defer";
    default:
        return "unknown";
    }
}
