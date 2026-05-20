#include <assert.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#include "yai/yai.h"

static void require_ok(yai_status_t status) {
    if (status != YAI_OK) {
        printf("unexpected status: %s\n", yai_status_string(status));
        assert(status == YAI_OK);
    }
}

static void make_tmp_dirs(const char *run_dir) {
    (void)mkdir("build", 0777);
    (void)mkdir("build/tmp", 0777);
    (void)mkdir("build/tmp/new3", 0777);
    (void)mkdir(run_dir, 0777);
}

static void append_record(const yai_journal_file_t *journal_file,
                          const char *record_id,
                          const yai_case_ref_t *case_ref,
                          yai_record_kind_t record_kind,
                          const yai_subject_ref_t *subject_ref,
                          const yai_id_t *attempt_id,
                          const yai_id_t *decision_id,
                          const yai_id_t *receipt_id,
                          const char *summary) {
    yai_store_record_t record;
    require_ok(yai_store_record_init(&record,
                                     record_id,
                                     case_ref,
                                     record_kind,
                                     subject_ref,
                                     attempt_id,
                                     decision_id,
                                     receipt_id,
                                     summary));
    require_ok(yai_journal_file_append(journal_file, &record));
}

int main(void) {
    char run_dir[128];
    char journal_path[160];
    yai_case_ref_t case_ref;
    yai_subject_ref_t actor_ref;
    yai_subject_ref_t subject_ref;
    yai_subject_binding_t binding;
    yai_op_attempt_t attempt;
    yai_policy_rule_t rule;
    yai_gate_result_t gate;
    yai_decision_basis_t basis;
    yai_control_decision_t decision;
    yai_obligation_t obligation;
    yai_receipt_requirement_t receipt_requirement;
    yai_effect_receipt_t receipt;
    yai_journal_file_t journal_file;
    yai_journal_t reloaded_journal;
    yai_projection_t projection;
    char basis_summary[256];
    char decision_summary[256];
    char obligation_summary[256];
    char receipt_requirement_summary[256];
    int written = 0;

    written = snprintf(run_dir,
                       sizeof(run_dir),
                       "build/tmp/new3/control-gate-%ld",
                       (long)getpid());
    assert(written > 0 && (size_t)written < sizeof(run_dir));
    written = snprintf(journal_path, sizeof(journal_path), "%s/journal.jsonl", run_dir);
    assert(written > 0 && (size_t)written < sizeof(journal_path));

    make_tmp_dirs(run_dir);
    (void)unlink(journal_path);

    require_ok(yai_case_ref_init(&case_ref, "case:new3-control", "new3-test", "open"));
    require_ok(yai_subject_ref_init(&actor_ref, "subject:agent-test", "agent", "local:test-agent"));
    require_ok(yai_subject_ref_init(&subject_ref, "subject:repo-test", "repository", "repo://new3-test"));
    require_ok(yai_subject_binding_init(&binding,
                                        "binding:repo-test",
                                        &case_ref,
                                        &subject_ref,
                                        YAI_BINDING_ATTACHED,
                                        YAI_CONTROL_ADVISORY,
                                        "local"));
    require_ok(yai_op_attempt_init(&attempt,
                                   "op:file-write-test",
                                   &case_ref,
                                   &actor_ref,
                                   &subject_ref,
                                   "file.write",
                                   "internal",
                                   "local",
                                   "mutative",
                                   "write test file"));

    require_ok(yai_policy_rule_for_attempt(&attempt, &rule));
    assert(rule.condition_kind == YAI_POLICY_CONDITION_MUTATIVE_OPERATION);
    assert(rule.effect == YAI_POLICY_EFFECT_REQUIRE_REVIEW);

    require_ok(yai_gate_evaluate_attempt(&attempt, &rule, &gate));
    assert(gate.gate_kind == YAI_GATE_OPERATION);
    assert(gate.result == YAI_GATE_REQUIRE_REVIEW);

    require_ok(yai_decision_basis_from_gate("basis:mutative-operation",
                                            &rule,
                                            &gate,
                                            &basis));
    require_ok(yai_control_decide_from_gate(&gate, &basis, &decision));
    assert(decision.outcome == YAI_DECISION_REQUIRE_REVIEW);

    require_ok(yai_obligation_from_decision("obligation:operator-review",
                                            &decision,
                                            &rule,
                                            &obligation));
    assert(obligation.obligation_kind == YAI_OBLIGATION_OPERATOR_REVIEW);
    decision.obligation_id = obligation.obligation_id;

    require_ok(yai_receipt_requirement_from_decision("receipt-requirement:blocked",
                                                     &decision,
                                                     &receipt_requirement));
    assert(receipt_requirement.requirement_kind == YAI_RECEIPT_REQUIREMENT_BLOCKED_RECEIPT);
    decision.receipt_requirement_id = receipt_requirement.requirement_id;

    require_ok(yai_effect_receipt_from_decision("receipt:file-write-blocked",
                                                &attempt,
                                                &decision,
                                                &receipt));
    assert(receipt.status == YAI_RECEIPT_BLOCKED);

    require_ok(yai_journal_file_init(&journal_file, journal_path));

    append_record(&journal_file,
                  "record:case",
                  &case_ref,
                  YAI_RECORD_CASE,
                  0,
                  0,
                  0,
                  0,
                  "case:new3-control opened");
    append_record(&journal_file,
                  "record:binding",
                  &case_ref,
                  YAI_RECORD_SUBJECT_BINDING,
                  &subject_ref,
                  0,
                  0,
                  0,
                  "subject:repo-test bound");
    append_record(&journal_file,
                  "record:attempt",
                  &case_ref,
                  YAI_RECORD_ATTEMPT,
                  &subject_ref,
                  &attempt.attempt_id,
                  0,
                  0,
                  "op:file.write mutative");
    append_record(&journal_file,
                  "record:policy-rule",
                  &case_ref,
                  YAI_RECORD_POLICY_RULE,
                  &subject_ref,
                  &attempt.attempt_id,
                  0,
                  0,
                  "rule:mutative_operation_requires_review condition:mutative_operation effect:require_review");
    append_record(&journal_file,
                  "record:gate-result",
                  &case_ref,
                  YAI_RECORD_GATE_RESULT,
                  &subject_ref,
                  &attempt.attempt_id,
                  0,
                  0,
                  "gate:operation result:require_review rule:mutative_operation_requires_review");
    (void)snprintf(basis_summary,
                   sizeof(basis_summary),
                   "basis:mutative_operation_requires_review gate:%.20s rule:%.20s",
                   gate.gate_id.value,
                   rule.rule_id.value);
    append_record(&journal_file,
                  "record:decision-basis",
                  &case_ref,
                  YAI_RECORD_DECISION_BASIS,
                  &subject_ref,
                  &attempt.attempt_id,
                  &decision.decision_id,
                  0,
                  basis_summary);
    (void)snprintf(decision_summary,
                   sizeof(decision_summary),
                   "decision:%s basis:%.20s gate:%.20s rule:%.20s obligation:%.20s receipt_requirement:%.20s",
                   yai_decision_outcome_string(decision.outcome),
                   decision.basis_id.value,
                   decision.gate_id.value,
                   decision.rule_id.value,
                   decision.obligation_id.value,
                   decision.receipt_requirement_id.value);
    append_record(&journal_file,
                  "record:decision",
                  &case_ref,
                  YAI_RECORD_DECISION,
                  &subject_ref,
                  &attempt.attempt_id,
                  &decision.decision_id,
                  0,
                  decision_summary);
    (void)snprintf(obligation_summary,
                   sizeof(obligation_summary),
                   "obligation:%s status:%s rule:%s",
                   yai_obligation_kind_string(obligation.obligation_kind),
                   yai_obligation_status_string(obligation.status),
                   obligation.rule_id.value);
    append_record(&journal_file,
                  "record:obligation",
                  &case_ref,
                  YAI_RECORD_OBLIGATION,
                  &subject_ref,
                  &attempt.attempt_id,
                  &decision.decision_id,
                  0,
                  obligation_summary);
    (void)snprintf(receipt_requirement_summary,
                   sizeof(receipt_requirement_summary),
                   "receipt_requirement:%s required:%d",
                   yai_receipt_requirement_kind_string(receipt_requirement.requirement_kind),
                   receipt_requirement.required);
    append_record(&journal_file,
                  "record:receipt-requirement",
                  &case_ref,
                  YAI_RECORD_RECEIPT_REQUIREMENT,
                  &subject_ref,
                  &attempt.attempt_id,
                  &decision.decision_id,
                  0,
                  receipt_requirement_summary);
    append_record(&journal_file,
                  "record:receipt",
                  &case_ref,
                  YAI_RECORD_RECEIPT,
                  &subject_ref,
                  &attempt.attempt_id,
                  &decision.decision_id,
                  &receipt.receipt_id,
                  "receipt:blocked");

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    assert(yai_journal_count(&reloaded_journal) == 10);

    require_ok(yai_projection_build("projection:control",
                                    &case_ref,
                                    YAI_PROJECTION_OPERATOR,
                                    &reloaded_journal,
                                    &projection));
    assert(projection.source_record_count == 10);
    assert(projection.decision_count == 1);
    assert(projection.policy_rule_count == 1);
    assert(projection.gate_count == 1);
    assert(projection.obligation_count == 1);
    assert(projection.receipt_requirement_count == 1);

    printf("case:new3-control\n");
    printf("subject:repo-test bound\n");
    printf("op:file.write mutative\n");
    printf("rule:mutative_operation_requires_review\n");
    printf("gate:operation %s\n", yai_gate_result_string(gate.result));
    printf("decision:%s\n", yai_decision_outcome_string(decision.outcome));
    printf("obligation:%s\n", yai_obligation_kind_string(obligation.obligation_kind));
    printf("receipt_requirement:%s\n", yai_receipt_requirement_kind_string(receipt_requirement.requirement_kind));
    printf("records appended:10\n");
    printf("records reloaded:%zu\n", yai_journal_count(&reloaded_journal));
    printf("projection:control summary built\n");

    yai_journal_free(&reloaded_journal);
    return 0;
}
