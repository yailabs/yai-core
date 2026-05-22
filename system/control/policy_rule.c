#include "yai/control/policy_rule.h"

#include <stddef.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);
int yai_string_contains(const char *haystack, const char *needle);

static int yai_attempt_is_read_like_new3(const yai_op_attempt_t *attempt) {
    return yai_string_contains(attempt->effect_class, "read") ||
           yai_string_contains(attempt->effect_class, "observe") ||
           yai_string_contains(attempt->mutability, "read") ||
           yai_string_contains(attempt->mutability, "immutable") ||
           yai_string_contains(attempt->mutability, "none");
}

yai_status_t yai_policy_rule_init(yai_policy_rule_t *rule,
                                  const char *rule_id,
                                  const yai_case_ref_t *case_ref,
                                  yai_policy_scope_kind_t scope_kind,
                                  const char *effect_class,
                                  yai_policy_condition_kind_t condition_kind,
                                  yai_policy_effect_t effect,
                                  yai_failure_mode_t failure_mode,
                                  const char *summary) {
    if (rule == 0 || rule_id == 0 || rule_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref)) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&rule->rule_id, rule_id);
    rule->case_ref = *case_ref;
    rule->scope_kind = scope_kind;
    yai_copy_string(rule->effect_class, sizeof(rule->effect_class), effect_class);
    rule->condition_kind = condition_kind;
    rule->effect = effect;
    rule->failure_mode = failure_mode;
    yai_copy_string(rule->summary, sizeof(rule->summary), summary);
    return YAI_OK;
}

yai_status_t yai_policy_rule_for_attempt(const yai_op_attempt_t *attempt,
                                         yai_policy_rule_t *rule) {
    if (attempt == 0 || rule == 0 || yai_case_ref_is_empty(&attempt->case_ref)) {
        return YAI_ERR_INVALID;
    }

    if (yai_subject_ref_is_empty(&attempt->target_subject_ref)) {
        return yai_policy_rule_init(rule,
                                    "rule:missing_subject_deny",
                                    &attempt->case_ref,
                                    YAI_POLICY_SCOPE_SUBJECT,
                                    attempt->effect_class,
                                    YAI_POLICY_CONDITION_MISSING_SUBJECT,
                                    YAI_POLICY_EFFECT_DENY,
                                    YAI_FAILURE_FAIL_CLOSED,
                                    "missing_subject -> deny");
    }

    if (yai_attempt_is_read_like_new3(attempt)) {
        return yai_policy_rule_init(rule,
                                    "rule:read_like_operation_allow",
                                    &attempt->case_ref,
                                    YAI_POLICY_SCOPE_OPERATION,
                                    attempt->effect_class,
                                    YAI_POLICY_CONDITION_READ_LIKE_OPERATION,
                                    YAI_POLICY_EFFECT_ALLOW,
                                    YAI_FAILURE_REQUIRE_REVIEW,
                                    "read_like_operation -> allow");
    }

    return yai_policy_rule_init(rule,
                                "rule:mutative_operation_requires_review",
                                &attempt->case_ref,
                                YAI_POLICY_SCOPE_OPERATION,
                                attempt->effect_class,
                                YAI_POLICY_CONDITION_MUTATIVE_OPERATION,
                                YAI_POLICY_EFFECT_REQUIRE_REVIEW,
                                YAI_FAILURE_REQUIRE_REVIEW,
                                "mutative_operation -> require_review");
}

const char *yai_policy_scope_kind_string(yai_policy_scope_kind_t kind) {
    switch (kind) {
    case YAI_POLICY_SCOPE_CASE:
        return "case";
    case YAI_POLICY_SCOPE_SUBJECT:
        return "subject";
    case YAI_POLICY_SCOPE_ACTOR:
        return "actor";
    case YAI_POLICY_SCOPE_OPERATION:
        return "operation";
    case YAI_POLICY_SCOPE_CARRIER:
        return "carrier";
    case YAI_POLICY_SCOPE_PROJECTION:
        return "projection";
    case YAI_POLICY_SCOPE_RETENTION:
        return "retention";
    default:
        return "unknown";
    }
}

const char *yai_policy_condition_kind_string(yai_policy_condition_kind_t kind) {
    switch (kind) {
    case YAI_POLICY_CONDITION_ALWAYS:
        return "always";
    case YAI_POLICY_CONDITION_MUTATIVE_OPERATION:
        return "mutative_operation";
    case YAI_POLICY_CONDITION_READ_LIKE_OPERATION:
        return "read_like_operation";
    case YAI_POLICY_CONDITION_EXTERNAL_LOCALITY:
        return "external_locality";
    case YAI_POLICY_CONDITION_MISSING_SUBJECT:
        return "missing_subject";
    default:
        return "unknown";
    }
}

const char *yai_policy_effect_string(yai_policy_effect_t effect) {
    switch (effect) {
    case YAI_POLICY_EFFECT_ALLOW:
        return "allow";
    case YAI_POLICY_EFFECT_DENY:
        return "deny";
    case YAI_POLICY_EFFECT_REQUIRE_REVIEW:
        return "require_review";
    case YAI_POLICY_EFFECT_REQUIRE_EVIDENCE:
        return "require_evidence";
    case YAI_POLICY_EFFECT_REQUIRE_RECEIPT:
        return "require_receipt";
    case YAI_POLICY_EFFECT_OBSERVE_ONLY:
        return "observe_only";
    default:
        return "unknown";
    }
}
