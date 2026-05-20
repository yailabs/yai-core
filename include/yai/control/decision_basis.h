#ifndef YAI_CONTROL_DECISION_BASIS_H
#define YAI_CONTROL_DECISION_BASIS_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/control/gate.h"
#include "yai/control/policy_rule.h"

typedef struct yai_decision_basis {
    yai_id_t basis_id;
    yai_case_ref_t case_ref;
    yai_id_t attempt_id;
    yai_id_t gate_id;
    yai_id_t rule_id;
    size_t gate_count;
    size_t policy_rule_count;
    size_t evidence_count;
    char summary[128];
} yai_decision_basis_t;

yai_status_t yai_decision_basis_from_gate(const char *basis_id,
                                          const yai_policy_rule_t *rule,
                                          const yai_gate_result_t *gate,
                                          yai_decision_basis_t *basis);

#endif
