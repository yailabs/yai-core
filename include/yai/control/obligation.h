#ifndef YAI_CONTROL_OBLIGATION_H
#define YAI_CONTROL_OBLIGATION_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/control/decision.h"
#include "yai/control/policy_rule.h"

typedef enum yai_obligation_kind {
    YAI_OBLIGATION_OPERATOR_REVIEW = 0,
    YAI_OBLIGATION_ATTACH_EVIDENCE = 1,
    YAI_OBLIGATION_PRODUCE_RECEIPT = 2,
    YAI_OBLIGATION_REDACT_BEFORE_PROJECTION = 3,
    YAI_OBLIGATION_FOLLOW_UP_OBSERVATION = 4
} yai_obligation_kind_t;

typedef enum yai_obligation_status {
    YAI_OBLIGATION_OPEN = 0,
    YAI_OBLIGATION_FULFILLED = 1,
    YAI_OBLIGATION_CANCELLED = 2
} yai_obligation_status_t;

typedef struct yai_obligation {
    yai_id_t obligation_id;
    yai_case_ref_t case_ref;
    yai_id_t decision_id;
    yai_id_t rule_id;
    yai_obligation_kind_t obligation_kind;
    yai_obligation_status_t status;
    char summary[128];
} yai_obligation_t;

yai_status_t yai_obligation_from_decision(const char *obligation_id,
                                          const yai_control_decision_t *decision,
                                          const yai_policy_rule_t *rule,
                                          yai_obligation_t *obligation);
const char *yai_obligation_kind_string(yai_obligation_kind_t kind);
const char *yai_obligation_status_string(yai_obligation_status_t status);

#endif
