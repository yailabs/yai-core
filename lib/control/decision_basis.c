#include "yai/control/decision_basis.h"

#include <stdio.h>

yai_status_t yai_decision_basis_from_gate(const char *basis_id,
                                          const yai_policy_rule_t *rule,
                                          const yai_gate_result_t *gate,
                                          yai_decision_basis_t *basis) {
    if (basis == 0 || basis_id == 0 || basis_id[0] == '\0' ||
        rule == 0 || gate == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&basis->basis_id, basis_id);
    basis->case_ref = gate->case_ref;
    basis->attempt_id = gate->attempt_id;
    basis->gate_id = gate->gate_id;
    basis->rule_id = rule->rule_id;
    basis->gate_count = 1;
    basis->policy_rule_count = 1;
    basis->evidence_count = 0;
    (void)snprintf(basis->summary,
                   sizeof(basis->summary),
                   "basis:%s rule:%s gate:%s result:%s",
                   rule->rule_id.value,
                   yai_policy_condition_kind_string(rule->condition_kind),
                   yai_gate_kind_string(gate->gate_kind),
                   yai_gate_result_string(gate->result));
    return YAI_OK;
}
