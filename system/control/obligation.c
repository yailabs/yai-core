#include "yai/control/obligation.h"

#include <stdio.h>

yai_status_t yai_obligation_from_decision(const char *obligation_id,
                                          const yai_control_decision_t *decision,
                                          const yai_policy_rule_t *rule,
                                          yai_obligation_t *obligation) {
    if (obligation == 0 || obligation_id == 0 || obligation_id[0] == '\0' ||
        decision == 0 || rule == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&obligation->obligation_id, obligation_id);
    obligation->case_ref = decision->case_ref;
    obligation->decision_id = decision->decision_id;
    obligation->rule_id = rule->rule_id;
    obligation->status = YAI_OBLIGATION_OPEN;

    if (decision->outcome == YAI_DECISION_REQUIRE_REVIEW) {
        obligation->obligation_kind = YAI_OBLIGATION_OPERATOR_REVIEW;
    } else if (decision->receipt_required) {
        obligation->obligation_kind = YAI_OBLIGATION_PRODUCE_RECEIPT;
    } else {
        obligation->obligation_kind = YAI_OBLIGATION_FOLLOW_UP_OBSERVATION;
    }

    (void)snprintf(obligation->summary,
                   sizeof(obligation->summary),
                   "%s required for %s",
                   yai_obligation_kind_string(obligation->obligation_kind),
                   decision->decision_id.value);
    return YAI_OK;
}

const char *yai_obligation_kind_string(yai_obligation_kind_t kind) {
    switch (kind) {
    case YAI_OBLIGATION_OPERATOR_REVIEW:
        return "operator_review";
    case YAI_OBLIGATION_ATTACH_EVIDENCE:
        return "attach_evidence";
    case YAI_OBLIGATION_PRODUCE_RECEIPT:
        return "produce_receipt";
    case YAI_OBLIGATION_REDACT_BEFORE_PROJECTION:
        return "redact_before_projection";
    case YAI_OBLIGATION_FOLLOW_UP_OBSERVATION:
        return "follow_up_observation";
    default:
        return "unknown";
    }
}

const char *yai_obligation_status_string(yai_obligation_status_t status) {
    switch (status) {
    case YAI_OBLIGATION_OPEN:
        return "open";
    case YAI_OBLIGATION_FULFILLED:
        return "fulfilled";
    case YAI_OBLIGATION_CANCELLED:
        return "cancelled";
    default:
        return "unknown";
    }
}
