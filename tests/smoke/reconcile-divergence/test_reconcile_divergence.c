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
    (void)mkdir("build/tmp/new7", 0777);
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

static void append_divergence(const yai_journal_file_t *journal_file,
                              const yai_divergence_t *divergence) {
    append_record(journal_file,
                  divergence->divergence_id.value,
                  &divergence->case_ref,
                  YAI_RECORD_DIVERGENCE,
                  &divergence->subject_ref,
                  &divergence->attempt_ref,
                  &divergence->decision_ref,
                  &divergence->receipt_ref,
                  divergence->summary);
}

static void append_reconciliation(const yai_journal_file_t *journal_file,
                                  const yai_reconciliation_t *reconciliation) {
    append_record(journal_file,
                  reconciliation->reconciliation_id.value,
                  &reconciliation->case_ref,
                  YAI_RECORD_RECONCILIATION,
                  &reconciliation->subject_ref,
                  0,
                  0,
                  0,
                  reconciliation->summary);
}

int main(void) {
    char run_dir[128];
    char journal_path[160];
    yai_case_ref_t case_ref;
    yai_subject_ref_t subject_ref;
    yai_id_t attempt_id;
    yai_id_t decision_id;
    yai_id_t receipt_id;
    yai_id_t missing_decision_receipt_id;
    yai_id_t no_decision;
    yai_journal_file_t journal_file;
    yai_journal_t reloaded_journal;
    yai_divergence_t divergence;
    yai_reconciliation_t reconciliation;
    yai_projection_t projection;
    int written = 0;

    written = snprintf(run_dir,
                       sizeof(run_dir),
                       "build/tmp/new7/reconcile-divergence-%ld",
                       (long)getpid());
    assert(written > 0 && (size_t)written < sizeof(run_dir));
    written = snprintf(journal_path, sizeof(journal_path), "%s/journal.jsonl", run_dir);
    assert(written > 0 && (size_t)written < sizeof(journal_path));

    make_tmp_dirs(run_dir);
    (void)unlink(journal_path);

    require_ok(yai_case_ref_init(&case_ref, "case:new7-reconcile", "new7-test", "open"));
    require_ok(yai_subject_ref_init(&subject_ref, "subject:repo-test", "repository", "repo://new7-test"));
    yai_id_set(&attempt_id, "op:dangerous-write");
    yai_id_set(&decision_id, "decision:deny-write");
    yai_id_set(&receipt_id, "receipt:executed-conflict");
    yai_id_set(&missing_decision_receipt_id, "receipt:without-decision");
    yai_id_set(&no_decision, "");

    require_ok(yai_journal_file_init(&journal_file, journal_path));
    append_record(&journal_file, "record:case", &case_ref, YAI_RECORD_CASE, 0, 0, 0, 0, "case:new7-reconcile opened");
    append_record(&journal_file, "record:binding", &case_ref, YAI_RECORD_SUBJECT_BINDING, &subject_ref, 0, 0, 0, "subject:repo-test bound");
    append_record(&journal_file, "record:attempt", &case_ref, YAI_RECORD_ATTEMPT, &subject_ref, &attempt_id, 0, 0, "op:fs.write mutative");
    append_record(&journal_file, "record:decision-deny", &case_ref, YAI_RECORD_DECISION, &subject_ref, &attempt_id, &decision_id, 0, "decision:deny");
    append_record(&journal_file, "record:receipt-executed", &case_ref, YAI_RECORD_RECEIPT, &subject_ref, &attempt_id, &decision_id, &receipt_id, "receipt:executed");
    append_record(&journal_file, "record:edge-decision-op", &case_ref, YAI_RECORD_GRAPH_EDGE, &subject_ref, &attempt_id, &decision_id, 0, "edge:decision_controls_op from:decision:deny-write to:op:dangerous-write");
    append_record(&journal_file, "record:edge-receipt-subject", &case_ref, YAI_RECORD_GRAPH_EDGE, &subject_ref, &attempt_id, &decision_id, &receipt_id, "edge:receipt_updates_subject from:receipt:executed-conflict to:subject:repo-test");

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_divergence_detect_first(&reloaded_journal, &divergence));
    assert(divergence.divergence_kind == YAI_DIVERGENCE_DENIED_BUT_EXECUTED);
    append_divergence(&journal_file, &divergence);
    require_ok(yai_reconciliation_from_divergence("reconciliation:denied-but-executed",
                                                  &divergence,
                                                  YAI_RECONCILE_REQUIRES_REVIEW,
                                                  &reconciliation));
    append_reconciliation(&journal_file, &reconciliation);
    yai_journal_free(&reloaded_journal);

    append_record(&journal_file,
                  "record:receipt-without-decision",
                  &case_ref,
                  YAI_RECORD_RECEIPT,
                  &subject_ref,
                  &attempt_id,
                  &no_decision,
                  &missing_decision_receipt_id,
                  "receipt:observed missing_decision");
    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_divergence_detect_first(&reloaded_journal, &divergence));
    if (divergence.divergence_kind != YAI_DIVERGENCE_RECEIPT_WITHOUT_DECISION) {
        yai_journal_free(&reloaded_journal);
        require_ok(yai_journal_init(&reloaded_journal));
        require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    }
    assert(divergence.divergence_kind == YAI_DIVERGENCE_RECEIPT_WITHOUT_DECISION);
    append_divergence(&journal_file, &divergence);
    require_ok(yai_reconciliation_from_divergence("reconciliation:receipt-without-decision",
                                                  &divergence,
                                                  YAI_RECONCILE_REQUIRES_REVIEW,
                                                  &reconciliation));
    append_reconciliation(&journal_file, &reconciliation);
    yai_journal_free(&reloaded_journal);

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_projection_build("projection:reconcile",
                                    &case_ref,
                                    YAI_PROJECTION_OPERATOR,
                                    &reloaded_journal,
                                    &projection));
    assert(projection.divergence_count == 2);
    assert(projection.reconciliation_count == 2);
    assert(projection.critical_divergence_count == 1);
    assert(projection.warning_divergence_count == 1);

    printf("divergence:denied_but_executed detected\n");
    printf("divergence:receipt_without_decision detected\n");
    printf("reconcile:records appended\n");
    printf("projection:reconcile summary built\n");

    yai_journal_free(&reloaded_journal);
    return 0;
}
