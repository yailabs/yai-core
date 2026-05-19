#include <assert.h>
#include <stdio.h>

#include "yai/yai.h"

static void require_ok(yai_status_t status) {
    if (status != YAI_OK) {
        printf("unexpected status: %s\n", yai_status_string(status));
        assert(status == YAI_OK);
    }
}

int main(void) {
    yai_case_ref_t case_ref;
    yai_subject_ref_t actor_ref;
    yai_subject_ref_t subject_ref;
    yai_subject_binding_t binding;
    yai_op_attempt_t attempt;
    yai_control_decision_t decision;
    yai_effect_receipt_t receipt;
    yai_journal_t journal;
    yai_store_record_t record;
    yai_projection_t projection;

    require_ok(yai_case_ref_init(&case_ref, "case:newcore-test", "newcore-test", "open"));
    require_ok(yai_subject_ref_init(&actor_ref, "subject:agent-test", "agent", "local:test-agent"));
    require_ok(yai_subject_ref_init(&subject_ref, "subject:repo-test", "repository", "repo://newcore-test"));
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

    require_ok(yai_control_decide(&attempt, &decision));
    assert(decision.outcome == YAI_DECISION_REQUIRE_REVIEW);
    assert(yai_id_equal(&decision.attempt_id, &attempt.attempt_id));

    require_ok(yai_effect_receipt_from_decision("receipt:file-write-blocked",
                                                &attempt,
                                                &decision,
                                                &receipt));
    assert(receipt.status == YAI_RECEIPT_BLOCKED);
    assert(yai_id_equal(&receipt.attempt_id, &attempt.attempt_id));
    assert(yai_id_equal(&receipt.decision_id, &decision.decision_id));

    require_ok(yai_journal_init(&journal));

    require_ok(yai_store_record_init(&record,
                                     "record:case",
                                     &case_ref,
                                     YAI_RECORD_CASE,
                                     0,
                                     0,
                                     0,
                                     0,
                                     "case:newcore-test opened"));
    require_ok(yai_journal_append(&journal, &record));

    require_ok(yai_store_record_init(&record,
                                     "record:binding",
                                     &case_ref,
                                     YAI_RECORD_SUBJECT_BINDING,
                                     &subject_ref,
                                     0,
                                     0,
                                     0,
                                     "subject:repo-test bound"));
    require_ok(yai_journal_append(&journal, &record));

    require_ok(yai_store_record_init(&record,
                                     "record:attempt",
                                     &case_ref,
                                     YAI_RECORD_ATTEMPT,
                                     &subject_ref,
                                     &attempt.attempt_id,
                                     0,
                                     0,
                                     "op:file.write attempted"));
    require_ok(yai_journal_append(&journal, &record));

    require_ok(yai_store_record_init(&record,
                                     "record:decision",
                                     &case_ref,
                                     YAI_RECORD_DECISION,
                                     &subject_ref,
                                     &attempt.attempt_id,
                                     &decision.decision_id,
                                     0,
                                     "decision:require_review"));
    require_ok(yai_journal_append(&journal, &record));

    require_ok(yai_store_record_init(&record,
                                     "record:receipt",
                                     &case_ref,
                                     YAI_RECORD_RECEIPT,
                                     &subject_ref,
                                     &attempt.attempt_id,
                                     &decision.decision_id,
                                     &receipt.receipt_id,
                                     "receipt:blocked"));
    require_ok(yai_journal_append(&journal, &record));

    require_ok(yai_projection_build("projection:operator",
                                    &case_ref,
                                    YAI_PROJECTION_OPERATOR,
                                    &journal,
                                    &projection));
    assert(yai_journal_count(&journal) == 5);
    assert(projection.source_record_count > 0);

    printf("case:newcore-test\n");
    printf("subject:repo-test bound\n");
    printf("op:file.write attempted\n");
    printf("decision:%s\n", yai_decision_outcome_string(decision.outcome));
    printf("receipt:%s\n", yai_receipt_status_string(receipt.status));
    printf("store:records appended:%zu\n", yai_journal_count(&journal));
    printf("projection:%s summary built\n", yai_projection_consumer_string(projection.consumer_kind));

    yai_journal_free(&journal);
    return 0;
}
