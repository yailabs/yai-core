#include "yai/control/decision.h"

#include <stddef.h>
#include <stdio.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_control_decide(const yai_op_attempt_t *attempt,
                                yai_control_decision_t *decision) {
    yai_policy_rule_t rule;
    yai_gate_result_t gate;
    yai_decision_basis_t basis;
    yai_status_t status = YAI_OK;

    if (attempt == 0 || decision == 0) {
        return YAI_ERR_INVALID;
    }

    status = yai_policy_rule_for_attempt(attempt, &rule);
    if (status != YAI_OK) {
        return status;
    }
    status = yai_gate_evaluate_attempt(attempt, &rule, &gate);
    if (status != YAI_OK) {
        return status;
    }
    status = yai_decision_basis_from_gate("basis:control-gate", &rule, &gate, &basis);
    if (status != YAI_OK) {
        return status;
    }
    return yai_control_decide_from_gate(&gate, &basis, decision);
}

yai_status_t yai_control_decide_from_gate(const yai_gate_result_t *gate,
                                          const yai_decision_basis_t *basis,
                                          yai_control_decision_t *decision) {
    if (gate == 0 || basis == 0 || decision == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&decision->decision_id, "decision:new3");
    decision->case_ref = gate->case_ref;
    decision->attempt_id = gate->attempt_id;
    decision->basis_id = basis->basis_id;
    decision->gate_id = gate->gate_id;
    decision->rule_id = basis->rule_id;
    yai_id_set(&decision->obligation_id, "");
    yai_id_set(&decision->receipt_requirement_id, "");
    decision->receipt_required = 1;

    if (gate->gate_kind == YAI_GATE_CASE && gate->result == YAI_GATE_BLOCK) {
        decision->outcome = YAI_DECISION_DENY;
        yai_copy_string(decision->reason, sizeof(decision->reason), "case gate blocked");
        return YAI_OK;
    }

    if (gate->gate_kind == YAI_GATE_SUBJECT && gate->result == YAI_GATE_BLOCK) {
        decision->outcome = YAI_DECISION_DENY;
        yai_copy_string(decision->reason, sizeof(decision->reason), "subject gate blocked");
        return YAI_OK;
    }

    if (gate->result == YAI_GATE_PASS) {
        decision->outcome = YAI_DECISION_ALLOW;
        (void)snprintf(decision->reason,
                       sizeof(decision->reason),
                       "gate:%s pass rule:%s",
                       yai_gate_kind_string(gate->gate_kind),
                       basis->rule_id.value);
        return YAI_OK;
    }

    if (gate->result == YAI_GATE_REQUIRE_EVIDENCE) {
        decision->outcome = YAI_DECISION_REQUIRE_EVIDENCE;
    } else if (gate->result == YAI_GATE_DEFER ||
               gate->result == YAI_GATE_NOT_APPLICABLE) {
        decision->outcome = YAI_DECISION_DEFER;
    } else if (gate->result == YAI_GATE_BLOCK) {
        decision->outcome = YAI_DECISION_DENY;
    } else {
        decision->outcome = YAI_DECISION_REQUIRE_REVIEW;
    }

    (void)snprintf(decision->reason,
                   sizeof(decision->reason),
                   "gate:%s %s rule:%s",
                   yai_gate_kind_string(gate->gate_kind),
                   yai_gate_result_string(gate->result),
                   basis->rule_id.value);
    return YAI_OK;
}

const char *yai_decision_outcome_string(yai_decision_outcome_t outcome) {
    switch (outcome) {
    case YAI_DECISION_ALLOW:
        return "allow";
    case YAI_DECISION_DENY:
        return "deny";
    case YAI_DECISION_DEFER:
        return "defer";
    case YAI_DECISION_OBSERVE_ONLY:
        return "observe_only";
    case YAI_DECISION_REQUIRE_REVIEW:
        return "require_review";
    case YAI_DECISION_REQUIRE_EVIDENCE:
        return "require_evidence";
    case YAI_DECISION_REQUIRE_REDACTION:
        return "require_redaction";
    case YAI_DECISION_ALLOW_WITH_CONSTRAINTS:
        return "allow_with_constraints";
    default:
        return "unknown";
    }
}
