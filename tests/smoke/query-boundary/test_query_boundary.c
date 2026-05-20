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
    (void)mkdir("build/tmp/new9", 0777);
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

static yai_query_result_t run_query(const yai_journal_t *journal,
                                    const char *query_id,
                                    const char *result_id,
                                    const yai_query_filter_t *filter) {
    yai_query_request_t request;
    yai_query_result_t result;
    require_ok(yai_query_request_init(&request, query_id, filter));
    require_ok(yai_query_scan(journal, &request, result_id, &result));
    return result;
}

static void append_query_result_record(const yai_journal_file_t *journal_file,
                                       const yai_case_ref_t *case_ref,
                                       const yai_query_result_t *result) {
    append_record(journal_file,
                  result->result_id.value,
                  case_ref,
                  YAI_RECORD_QUERY_RESULT,
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
    yai_query_filter_t filter;
    yai_query_result_t all_result;
    yai_query_result_t receipt_result;
    yai_query_result_t memory_result;
    yai_query_result_t case_result;
    yai_projection_t projection;
    int written = 0;

    written = snprintf(run_dir,
                       sizeof(run_dir),
                       "build/tmp/new9/query-boundary-%ld",
                       (long)getpid());
    assert(written > 0 && (size_t)written < sizeof(run_dir));
    written = snprintf(journal_path, sizeof(journal_path), "%s/journal.jsonl", run_dir);
    assert(written > 0 && (size_t)written < sizeof(journal_path));

    make_tmp_dirs(run_dir);
    (void)unlink(journal_path);

    require_ok(yai_case_ref_init(&case_ref, "case:new9-query", "new9-test", "open"));
    require_ok(yai_subject_ref_init(&subject_ref, "subject:repo-test", "repository", "repo://new9-test"));
    yai_id_set(&attempt_id, "op:fs-write-query");
    yai_id_set(&decision_id, "decision:require-review");
    yai_id_set(&receipt_id, "receipt:blocked-query");

    require_ok(yai_journal_file_init(&journal_file, journal_path));
    append_record(&journal_file, "record:case", &case_ref, YAI_RECORD_CASE, 0, 0, 0, 0, "case:new9-query opened");
    append_record(&journal_file, "record:binding", &case_ref, YAI_RECORD_SUBJECT_BINDING, &subject_ref, 0, 0, 0, "subject:repo-test bound");
    append_record(&journal_file, "record:attempt", &case_ref, YAI_RECORD_ATTEMPT, &subject_ref, &attempt_id, 0, 0, "op:fs.write mutative");
    append_record(&journal_file, "record:decision", &case_ref, YAI_RECORD_DECISION, &subject_ref, &attempt_id, &decision_id, 0, "decision:require_review");
    append_record(&journal_file, "record:receipt", &case_ref, YAI_RECORD_RECEIPT, &subject_ref, &attempt_id, &decision_id, &receipt_id, "receipt:blocked");
    append_record(&journal_file, "record:edge", &case_ref, YAI_RECORD_GRAPH_EDGE, &subject_ref, &attempt_id, &decision_id, &receipt_id, "edge:receipt_records_effect from:receipt:blocked-query to:op:fs-write-query");
    append_record(&journal_file, "record:memory-candidate", &case_ref, YAI_RECORD_MEMORY_CANDIDATE, &subject_ref, &attempt_id, &decision_id, &receipt_id, "memory:decision memory:memory-candidate basis_records:6");
    append_record(&journal_file, "projection-result:operator", &case_ref, YAI_RECORD_PROJECTION_RESULT, 0, 0, 0, 0, "projection_result:operator consumer:operator kind:operator_summary redaction:none");

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));

    require_ok(yai_query_filter_init(&filter));
    all_result = run_query(&reloaded_journal, "query:all", "query-result:all", &filter);
    assert(all_result.matched_count == 8);
    assert(all_result.returned_count == 8);

    require_ok(yai_query_filter_init(&filter));
    filter.has_record_kind = 1;
    filter.record_kind = YAI_RECORD_RECEIPT;
    receipt_result = run_query(&reloaded_journal, "query:receipt", "query-result:receipt", &filter);
    assert(receipt_result.matched_count == 1);

    require_ok(yai_query_filter_init(&filter));
    filter.has_record_kind = 1;
    filter.record_kind = YAI_RECORD_MEMORY_CANDIDATE;
    memory_result = run_query(&reloaded_journal, "query:memory", "query-result:memory", &filter);
    assert(memory_result.matched_count == 1);

    require_ok(yai_query_filter_init(&filter));
    filter.case_ref = case_ref;
    case_result = run_query(&reloaded_journal, "query:case", "query-result:case", &filter);
    assert(case_result.matched_count == 8);
    yai_journal_free(&reloaded_journal);

    append_query_result_record(&journal_file, &case_ref, &all_result);
    append_query_result_record(&journal_file, &case_ref, &receipt_result);
    append_query_result_record(&journal_file, &case_ref, &memory_result);
    append_query_result_record(&journal_file, &case_ref, &case_result);

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_projection_build("projection:query",
                                    &case_ref,
                                    YAI_PROJECTION_OPERATOR,
                                    &reloaded_journal,
                                    &projection));
    assert(projection.query_result_count == 4);

    printf("query:all matched:%zu\n", all_result.matched_count);
    printf("query:receipt matched:%zu\n", receipt_result.matched_count);
    printf("query:memory_candidate matched:%zu\n", memory_result.matched_count);
    printf("query:case matched:%zu\n", case_result.matched_count);
    printf("projection:query summary built\n");

    yai_journal_free(&reloaded_journal);
    return 0;
}
