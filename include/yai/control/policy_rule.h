#ifndef YAI_CONTROL_POLICY_RULE_H
#define YAI_CONTROL_POLICY_RULE_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/control/failure_mode.h"
#include "yai/op/attempt.h"

typedef enum yai_policy_scope_kind {
    YAI_POLICY_SCOPE_CASE = 0,
    YAI_POLICY_SCOPE_SUBJECT = 1,
    YAI_POLICY_SCOPE_ACTOR = 2,
    YAI_POLICY_SCOPE_OPERATION = 3,
    YAI_POLICY_SCOPE_CARRIER = 4,
    YAI_POLICY_SCOPE_PROJECTION = 5,
    YAI_POLICY_SCOPE_RETENTION = 6
} yai_policy_scope_kind_t;

typedef enum yai_policy_condition_kind {
    YAI_POLICY_CONDITION_ALWAYS = 0,
    YAI_POLICY_CONDITION_MUTATIVE_OPERATION = 1,
    YAI_POLICY_CONDITION_READ_LIKE_OPERATION = 2,
    YAI_POLICY_CONDITION_EXTERNAL_LOCALITY = 3,
    YAI_POLICY_CONDITION_MISSING_SUBJECT = 4
} yai_policy_condition_kind_t;

typedef enum yai_policy_effect {
    YAI_POLICY_EFFECT_ALLOW = 0,
    YAI_POLICY_EFFECT_DENY = 1,
    YAI_POLICY_EFFECT_REQUIRE_REVIEW = 2,
    YAI_POLICY_EFFECT_REQUIRE_EVIDENCE = 3,
    YAI_POLICY_EFFECT_REQUIRE_RECEIPT = 4,
    YAI_POLICY_EFFECT_OBSERVE_ONLY = 5
} yai_policy_effect_t;

typedef struct yai_policy_rule {
    yai_id_t rule_id;
    yai_case_ref_t case_ref;
    yai_policy_scope_kind_t scope_kind;
    char effect_class[32];
    yai_policy_condition_kind_t condition_kind;
    yai_policy_effect_t effect;
    yai_failure_mode_t failure_mode;
    char summary[128];
} yai_policy_rule_t;

yai_status_t yai_policy_rule_init(yai_policy_rule_t *rule,
                                  const char *rule_id,
                                  const yai_case_ref_t *case_ref,
                                  yai_policy_scope_kind_t scope_kind,
                                  const char *effect_class,
                                  yai_policy_condition_kind_t condition_kind,
                                  yai_policy_effect_t effect,
                                  yai_failure_mode_t failure_mode,
                                  const char *summary);
yai_status_t yai_policy_rule_for_attempt(const yai_op_attempt_t *attempt,
                                         yai_policy_rule_t *rule);
const char *yai_policy_scope_kind_string(yai_policy_scope_kind_t kind);
const char *yai_policy_condition_kind_string(yai_policy_condition_kind_t kind);
const char *yai_policy_effect_string(yai_policy_effect_t effect);

#endif
