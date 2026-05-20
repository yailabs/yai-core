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
    (void)mkdir("build/tmp/new8", 0777);
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

static void append_projection_request(const yai_journal_file_t *journal_file,
                                      const yai_projection_request_t *request) {
    char summary[128];
    int written = snprintf(summary,
                           sizeof(summary),
                           "projection_request:%.32s consumer:%s kind:%s scope:%.24s",
                           request->request_id.value,
                           yai_projection_consumer_string(request->consumer_kind),
                           yai_projection_kind_string(request->projection_kind),
                           request->scope);
    assert(written > 0 && (size_t)written < sizeof(summary));
    append_record(journal_file,
                  request->request_id.value,
                  &request->case_ref,
                  YAI_RECORD_PROJECTION_REQUEST,
                  0,
                  0,
                  0,
                  0,
                  summary);
}

static void append_projection_result(const yai_journal_file_t *journal_file,
                                     const char *record_id,
                                     const yai_projection_result_t *result) {
    append_record(journal_file,
                  record_id,
                  &result->case_ref,
                  YAI_RECORD_PROJECTION_RESULT,
                  0,
                  0,
                  0,
                  0,
                  result->summary);
}

int main(void) {
    char run_dir[128];
    char journal_path[160];
    yai_case_ref_t case_ref;
    yai_subject_ref_t subject_ref;
    yai_id_t attempt_id;
    yai_id_t decision_id;
    yai_id_t receipt_id;
    yai_journal_file_t journal_file;
    yai_journal_t reloaded_journal;
    yai_projection_request_t operator_request;
    yai_projection_request_t model_request;
    yai_projection_result_t operator_result;
    yai_projection_result_t model_result;
    yai_projection_t projection;
    int written = 0;

    written = snprintf(run_dir,
                       sizeof(run_dir),
                       "build/tmp/new8/projection-hardening-%ld",
                       (long)getpid());
    assert(written > 0 && (size_t)written < sizeof(run_dir));
    written = snprintf(journal_path, sizeof(journal_path), "%s/journal.jsonl", run_dir);
    assert(written > 0 && (size_t)written < sizeof(journal_path));

    make_tmp_dirs(run_dir);
    (void)unlink(journal_path);

    require_ok(yai_case_ref_init(&case_ref, "case:new8-projection", "new8-test", "open"));
    require_ok(yai_subject_ref_init(&subject_ref, "subject:repo-test", "repository", "repo://new8-test"));
    yai_id_set(&attempt_id, "op:fs-write-projection");
    yai_id_set(&decision_id, "decision:require-review");
    yai_id_set(&receipt_id, "receipt:blocked-projection");

    require_ok(yai_journal_file_init(&journal_file, journal_path));
    append_record(&journal_file, "record:case", &case_ref, YAI_RECORD_CASE, 0, 0, 0, 0, "case:new8-projection opened");
    append_record(&journal_file, "record:binding", &case_ref, YAI_RECORD_SUBJECT_BINDING, &subject_ref, 0, 0, 0, "subject:repo-test bound");
    append_record(&journal_file, "record:attempt", &case_ref, YAI_RECORD_ATTEMPT, &subject_ref, &attempt_id, 0, 0, "op:fs.write mutative");
    append_record(&journal_file, "record:decision", &case_ref, YAI_RECORD_DECISION, &subject_ref, &attempt_id, &decision_id, 0, "decision:require_review");
    append_record(&journal_file, "record:receipt", &case_ref, YAI_RECORD_RECEIPT, &subject_ref, &attempt_id, &decision_id, &receipt_id, "receipt:blocked");
    append_record(&journal_file, "record:edge", &case_ref, YAI_RECORD_GRAPH_EDGE, &subject_ref, &attempt_id, &decision_id, &receipt_id, "edge:receipt_updates_subject from:receipt:blocked-projection to:subject:repo-test");
    append_record(&journal_file, "record:memory-candidate", &case_ref, YAI_RECORD_MEMORY_CANDIDATE, &subject_ref, &attempt_id, &decision_id, &receipt_id, "memory:decision basis_records:6 basis_receipts:1 basis_edges:1");
    append_record(&journal_file, "record:divergence-warning", &case_ref, YAI_RECORD_DIVERGENCE, &subject_ref, &attempt_id, &decision_id, &receipt_id, "divergence:memory_without_basis severity:warning");

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_projection_request_init(&operator_request,
                                           "projection-request:operator",
                                           &case_ref,
                                           YAI_PROJECTION_OPERATOR,
                                           YAI_PROJECTION_KIND_OPERATOR_SUMMARY,
                                           "case",
                                           "operator projection request"));
    require_ok(yai_projection_result_from_request(&operator_request,
                                                  &reloaded_journal,
                                                  &operator_result));
    assert(operator_result.redaction_posture == YAI_REDACTION_NONE);
    assert(operator_result.source_record_count == yai_journal_count(&reloaded_journal));
    assert(operator_result.source_receipt_count == 1);
    assert(operator_result.source_memory_count == 1);
    assert(operator_result.source_divergence_count == 1);
    append_projection_request(&journal_file, &operator_request);
    append_projection_result(&journal_file, "projection-result:operator", &operator_result);

    require_ok(yai_projection_request_init(&model_request,
                                           "projection-request:model",
                                           &case_ref,
                                           YAI_PROJECTION_MODEL,
                                           YAI_PROJECTION_KIND_MODEL_CONTEXT,
                                           "case",
                                           "model projection request"));
    require_ok(yai_projection_result_from_request(&model_request,
                                                  &reloaded_journal,
                                                  &model_result));
    assert(model_result.redaction_posture == YAI_REDACTION_SUMMARY_ONLY);
    append_projection_request(&journal_file, &model_request);
    append_projection_result(&journal_file, "projection-result:model", &model_result);
    yai_journal_free(&reloaded_journal);

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_projection_build("projection:inspect",
                                    &case_ref,
                                    YAI_PROJECTION_OPERATOR,
                                    &reloaded_journal,
                                    &projection));
    assert(projection.projection_request_count == 2);
    assert(projection.projection_result_count == 2);
    assert(projection.operator_projection_count == 2);
    assert(projection.model_projection_count == 2);
    assert(projection.limited_projection_count == 1);

    printf("projection:operator summary full\n");
    printf("projection:model summary_only\n");
    printf("projection:provenance counts recorded\n");
    printf("projection:redaction posture recorded\n");

    yai_journal_free(&reloaded_journal);
    return 0;
}
