#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "yai/yai.h"

static void require_ok(yai_status_t status) {
    if (status != YAI_OK) {
        printf("unexpected status: %s\n", yai_status_string(status));
        assert(status == YAI_OK);
    }
}

static void make_tmp_dirs(const char *run_dir, const char *sandbox) {
    (void)mkdir("build", 0777);
    (void)mkdir("build/tmp", 0777);
    (void)mkdir("build/tmp/new4", 0777);
    (void)mkdir(run_dir, 0777);
    (void)mkdir(sandbox, 0777);
}

static void write_fixture(const char *path, const char *content) {
    FILE *file = fopen(path, "wb");
    assert(file != 0);
    assert(fwrite(content, 1, strlen(content), file) == strlen(content));
    assert(fclose(file) == 0);
}

static void assert_file_content(const char *path, const char *expected) {
    char buffer[128];
    FILE *file = fopen(path, "rb");
    size_t count = 0;

    assert(file != 0);
    count = fread(buffer, 1, sizeof(buffer) - 1, file);
    assert(fclose(file) == 0);
    buffer[count] = '\0';
    assert(strcmp(buffer, expected) == 0);
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

static void append_filesystem_receipt(const yai_journal_file_t *journal_file,
                                      const char *record_id,
                                      const yai_effect_receipt_t *receipt) {
    char summary[128];

    (void)snprintf(summary,
                   sizeof(summary),
                   "effect:%s status:%s bytes:%zu after_hash:%s",
                   yai_effect_kind_string(receipt->effect_kind),
                   yai_receipt_status_string(receipt->status),
                   receipt->bytes_affected,
                   receipt->after_hash);
    append_record(journal_file,
                  record_id,
                  &receipt->case_ref,
                  YAI_RECORD_FILESYSTEM_RECEIPT,
                  &receipt->target_subject_ref,
                  &receipt->attempt_id,
                  &receipt->decision_id,
                  &receipt->receipt_id,
                  summary);
}

static void make_allow_with_constraints_decision(const yai_op_attempt_t *attempt,
                                                 yai_control_decision_t *decision) {
    yai_id_set(&decision->decision_id, "decision:allow-write");
    decision->case_ref = attempt->case_ref;
    decision->attempt_id = attempt->attempt_id;
    yai_id_set(&decision->basis_id, "basis:manual-carrier-test");
    yai_id_set(&decision->gate_id, "gate:carrier");
    yai_id_set(&decision->rule_id, "rule:manual-allow-with-constraints");
    yai_id_set(&decision->obligation_id, "");
    yai_id_set(&decision->receipt_requirement_id, "receipt-requirement:carrier");
    decision->outcome = YAI_DECISION_ALLOW_WITH_CONSTRAINTS;
    decision->receipt_required = 1;
    (void)snprintf(decision->reason,
                   sizeof(decision->reason),
                   "manual carrier test allow_with_constraints");
}

int main(void) {
    char run_dir[128];
    char sandbox[160];
    char journal_path[192];
    char input_path[192];
    char blocked_path[192];
    char output_path[192];
    yai_case_ref_t case_ref;
    yai_subject_ref_t actor_ref;
    yai_subject_ref_t file_subject_ref;
    yai_subject_binding_t binding;
    yai_op_attempt_t read_attempt;
    yai_op_attempt_t blocked_write_attempt;
    yai_op_attempt_t allowed_write_attempt;
    yai_control_decision_t read_decision;
    yai_control_decision_t blocked_write_decision;
    yai_control_decision_t allowed_write_decision;
    yai_effect_receipt_t read_receipt;
    yai_effect_receipt_t blocked_write_receipt;
    yai_effect_receipt_t allowed_write_receipt;
    yai_journal_file_t journal_file;
    yai_journal_t reloaded_journal;
    yai_subject_state_t subject_state;
    yai_projection_t projection;
    char subject_state_summary[128];
    int written = 0;

    written = snprintf(run_dir,
                       sizeof(run_dir),
                       "build/tmp/new4/filesystem-carrier-%ld",
                       (long)getpid());
    assert(written > 0 && (size_t)written < sizeof(run_dir));
    written = snprintf(sandbox, sizeof(sandbox), "%s/sandbox", run_dir);
    assert(written > 0 && (size_t)written < sizeof(sandbox));
    written = snprintf(journal_path, sizeof(journal_path), "%s/journal.jsonl", run_dir);
    assert(written > 0 && (size_t)written < sizeof(journal_path));
    written = snprintf(input_path, sizeof(input_path), "%s/input.txt", sandbox);
    assert(written > 0 && (size_t)written < sizeof(input_path));
    written = snprintf(blocked_path, sizeof(blocked_path), "%s/blocked.txt", sandbox);
    assert(written > 0 && (size_t)written < sizeof(blocked_path));
    written = snprintf(output_path, sizeof(output_path), "%s/output.txt", sandbox);
    assert(written > 0 && (size_t)written < sizeof(output_path));

    make_tmp_dirs(run_dir, sandbox);
    (void)unlink(journal_path);
    (void)unlink(input_path);
    (void)unlink(blocked_path);
    (void)unlink(output_path);
    write_fixture(input_path, "hello filesystem\n");

    require_ok(yai_case_ref_init(&case_ref, "case:new4-filesystem", "new4-test", "open"));
    require_ok(yai_subject_ref_init(&actor_ref, "subject:agent-test", "agent", "local:test-agent"));
    require_ok(yai_subject_ref_init(&file_subject_ref, "subject:file-input", "filesystem", input_path));
    require_ok(yai_subject_binding_init(&binding,
                                        "binding:file-input",
                                        &case_ref,
                                        &file_subject_ref,
                                        YAI_BINDING_ATTACHED,
                                        YAI_CONTROL_ENFORCED,
                                        "local"));

    require_ok(yai_journal_file_init(&journal_file, journal_path));
    append_record(&journal_file,
                  "record:case",
                  &case_ref,
                  YAI_RECORD_CASE,
                  0,
                  0,
                  0,
                  0,
                  "case:new4-filesystem opened");
    append_record(&journal_file,
                  "record:binding",
                  &case_ref,
                  YAI_RECORD_SUBJECT_BINDING,
                  &file_subject_ref,
                  0,
                  0,
                  0,
                  "subject:file-input bound");

    require_ok(yai_op_attempt_init(&read_attempt,
                                   "op:fs-read",
                                   &case_ref,
                                   &actor_ref,
                                   &file_subject_ref,
                                   "fs.read",
                                   "internal",
                                   "local",
                                   "read",
                                   "read input file"));
    require_ok(yai_control_decide(&read_attempt, &read_decision));
    assert(read_decision.outcome == YAI_DECISION_ALLOW);
    require_ok(yai_filesystem_carrier_read("receipt:fs-read",
                                           &read_attempt,
                                           &read_decision,
                                           sandbox,
                                           input_path,
                                           &read_receipt));
    assert(read_receipt.status == YAI_RECEIPT_OBSERVED);
    assert(read_receipt.bytes_affected > 0);

    append_record(&journal_file,
                  "record:read-attempt",
                  &case_ref,
                  YAI_RECORD_ATTEMPT,
                  &file_subject_ref,
                  &read_attempt.attempt_id,
                  0,
                  0,
                  "op:fs.read read-like");
    append_record(&journal_file,
                  "record:read-decision",
                  &case_ref,
                  YAI_RECORD_DECISION,
                  &file_subject_ref,
                  &read_attempt.attempt_id,
                  &read_decision.decision_id,
                  0,
                  "decision:allow effect:fs.read");
    append_filesystem_receipt(&journal_file, "record:fs-read-receipt", &read_receipt);

    require_ok(yai_op_attempt_init(&blocked_write_attempt,
                                   "op:fs-write-blocked",
                                   &case_ref,
                                   &actor_ref,
                                   &file_subject_ref,
                                   "fs.write",
                                   "internal",
                                   "local",
                                   "mutative",
                                   "write blocked file"));
    require_ok(yai_control_decide(&blocked_write_attempt, &blocked_write_decision));
    assert(blocked_write_decision.outcome == YAI_DECISION_REQUIRE_REVIEW);
    require_ok(yai_filesystem_carrier_write("receipt:fs-write-blocked",
                                            &blocked_write_attempt,
                                            &blocked_write_decision,
                                            sandbox,
                                            blocked_path,
                                            "blocked content",
                                            &blocked_write_receipt));
    assert(blocked_write_receipt.status == YAI_RECEIPT_BLOCKED);
    assert(access(blocked_path, F_OK) != 0);

    append_record(&journal_file,
                  "record:blocked-write-attempt",
                  &case_ref,
                  YAI_RECORD_ATTEMPT,
                  &file_subject_ref,
                  &blocked_write_attempt.attempt_id,
                  0,
                  0,
                  "op:fs.write mutative");
    append_record(&journal_file,
                  "record:blocked-write-decision",
                  &case_ref,
                  YAI_RECORD_DECISION,
                  &file_subject_ref,
                  &blocked_write_attempt.attempt_id,
                  &blocked_write_decision.decision_id,
                  0,
                  "decision:require_review effect:fs.write");
    append_filesystem_receipt(&journal_file, "record:fs-write-blocked-receipt", &blocked_write_receipt);

    require_ok(yai_op_attempt_init(&allowed_write_attempt,
                                   "op:fs-write-allowed",
                                   &case_ref,
                                   &actor_ref,
                                   &file_subject_ref,
                                   "fs.write",
                                   "internal",
                                   "local",
                                   "mutative",
                                   "write output file"));
    make_allow_with_constraints_decision(&allowed_write_attempt, &allowed_write_decision);
    require_ok(yai_filesystem_carrier_write("receipt:fs-write-executed",
                                            &allowed_write_attempt,
                                            &allowed_write_decision,
                                            sandbox,
                                            output_path,
                                            "allowed content\n",
                                            &allowed_write_receipt));
    assert(allowed_write_receipt.status == YAI_RECEIPT_EXECUTED);
    assert(strcmp(allowed_write_receipt.before_hash, "none") == 0);
    assert(allowed_write_receipt.after_hash[0] != '\0');
    assert_file_content(output_path, "allowed content\n");

    append_record(&journal_file,
                  "record:allowed-write-attempt",
                  &case_ref,
                  YAI_RECORD_ATTEMPT,
                  &file_subject_ref,
                  &allowed_write_attempt.attempt_id,
                  0,
                  0,
                  "op:fs.write mutative");
    append_record(&journal_file,
                  "record:allowed-write-decision",
                  &case_ref,
                  YAI_RECORD_DECISION,
                  &file_subject_ref,
                  &allowed_write_attempt.attempt_id,
                  &allowed_write_decision.decision_id,
                  0,
                  "decision:allow_with_constraints effect:fs.write");
    append_filesystem_receipt(&journal_file, "record:fs-write-executed-receipt", &allowed_write_receipt);

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_subject_state_from_journal(&case_ref,
                                              &file_subject_ref,
                                              &reloaded_journal,
                                              &subject_state));
    assert(subject_state.state_kind == YAI_SUBJECT_STATE_OBSERVED);
    assert(yai_id_equal(&subject_state.last_receipt_id, &allowed_write_receipt.receipt_id));
    assert(subject_state.content_or_status_hash[0] != '\0');
    yai_journal_free(&reloaded_journal);

    (void)snprintf(subject_state_summary,
                   sizeof(subject_state_summary),
                   "state:%s after_hash:%s",
                   yai_subject_state_kind_string(subject_state.state_kind),
                   subject_state.content_or_status_hash);
    append_record(&journal_file,
                  "record:subject-state",
                  &case_ref,
                  YAI_RECORD_SUBJECT_STATE,
                  &file_subject_ref,
                  0,
                  &allowed_write_decision.decision_id,
                  &allowed_write_receipt.receipt_id,
                  subject_state_summary);

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_projection_build("projection:filesystem",
                                    &case_ref,
                                    YAI_PROJECTION_OPERATOR,
                                    &reloaded_journal,
                                    &projection));
    assert(projection.filesystem_receipt_count == 3);
    assert(projection.subject_state_count == 1);
    assert(projection.effect_count == 3);

    printf("fs:read allowed\n");
    printf("fs:write require_review blocked\n");
    printf("fs:write allow_with_constraints executed\n");
    printf("receipt:filesystem hashes recorded\n");
    printf("records reloaded:%zu\n", yai_journal_count(&reloaded_journal));
    printf("projection:filesystem summary built\n");

    yai_journal_free(&reloaded_journal);
    return 0;
}
