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
    (void)mkdir("build/tmp/new2", 0777);
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
    yai_control_decision_t decision;
    yai_effect_receipt_t receipt;
    yai_journal_file_t journal_file;
    yai_journal_t reloaded_journal;
    yai_subject_state_t subject_state;
    yai_projection_t projection;
    int written = 0;

    written = snprintf(run_dir,
                       sizeof(run_dir),
                       "build/tmp/new2/persistent-journal-%ld",
                       (long)getpid());
    assert(written > 0 && (size_t)written < sizeof(run_dir));
    written = snprintf(journal_path, sizeof(journal_path), "%s/journal.jsonl", run_dir);
    assert(written > 0 && (size_t)written < sizeof(journal_path));

    make_tmp_dirs(run_dir);
    (void)unlink(journal_path);

    require_ok(yai_case_ref_init(&case_ref, "case:new2-persistent", "new2-test", "open"));
    require_ok(yai_subject_ref_init(&actor_ref, "subject:agent-test", "agent", "local:test-agent"));
    require_ok(yai_subject_ref_init(&subject_ref, "subject:repo-test", "repository", "repo://new2-test"));
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
                  "case:new2-persistent opened");
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
                  "op:file.write attempted");
    append_record(&journal_file,
                  "record:decision",
                  &case_ref,
                  YAI_RECORD_DECISION,
                  &subject_ref,
                  &attempt.attempt_id,
                  &decision.decision_id,
                  0,
                  "decision:require_review");
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
    assert(yai_journal_count(&reloaded_journal) == 5);

    require_ok(yai_subject_state_from_journal(&case_ref,
                                              &subject_ref,
                                              &reloaded_journal,
                                              &subject_state));
    assert(subject_state.state_kind == YAI_SUBJECT_STATE_OBSERVED);
    assert(yai_id_equal(&subject_state.last_receipt_id, &receipt.receipt_id));
    assert(yai_id_equal(&subject_state.last_decision_id, &decision.decision_id));

    require_ok(yai_projection_build("projection:persistent-operator",
                                    &case_ref,
                                    YAI_PROJECTION_OPERATOR,
                                    &reloaded_journal,
                                    &projection));
    assert(projection.source_record_count == 5);

    printf("journal:file-backed\n");
    printf("case:new2-persistent\n");
    printf("subject:repo-test state:%s\n", yai_subject_state_kind_string(subject_state.state_kind));
    printf("records appended:5\n");
    printf("records reloaded:%zu\n", yai_journal_count(&reloaded_journal));
    printf("projection:persistent summary built\n");

    yai_journal_free(&reloaded_journal);
    return 0;
}
