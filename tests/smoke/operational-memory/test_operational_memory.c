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
    (void)mkdir("build/tmp/new6", 0777);
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

static void append_edge(const yai_journal_file_t *journal_file,
                        const char *record_id,
                        const yai_graph_edge_t *edge,
                        const yai_subject_ref_t *subject_ref,
                        const yai_id_t *attempt_id,
                        const yai_id_t *decision_id,
                        const yai_id_t *receipt_id) {
    char summary[256];
    int written = snprintf(summary,
                           sizeof(summary),
                           "edge:%s from:%.64s to:%.64s",
                           yai_graph_edge_kind_string(edge->edge_kind),
                           edge->from_ref.value,
                           edge->to_ref.value);
    assert(written > 0 && (size_t)written < sizeof(summary));
    append_record(journal_file,
                  record_id,
                  &edge->case_ref,
                  YAI_RECORD_GRAPH_EDGE,
                  subject_ref,
                  attempt_id,
                  decision_id,
                  receipt_id,
                  summary);
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
    yai_graph_edge_t edge;
    yai_journal_file_t journal_file;
    yai_journal_t reloaded_journal;
    yai_reconstruction_t reconstruction;
    yai_memory_candidate_t candidate;
    yai_projection_t projection;
    int written = 0;

    written = snprintf(run_dir,
                       sizeof(run_dir),
                       "build/tmp/new6/operational-memory-%ld",
                       (long)getpid());
    assert(written > 0 && (size_t)written < sizeof(run_dir));
    written = snprintf(journal_path, sizeof(journal_path), "%s/journal.jsonl", run_dir);
    assert(written > 0 && (size_t)written < sizeof(journal_path));

    make_tmp_dirs(run_dir);
    (void)unlink(journal_path);

    require_ok(yai_case_ref_init(&case_ref, "case:new6-memory", "new6-test", "open"));
    require_ok(yai_subject_ref_init(&actor_ref, "subject:agent-test", "agent", "local:test-agent"));
    require_ok(yai_subject_ref_init(&subject_ref, "subject:repo-test", "repository", "repo://new6-test"));
    require_ok(yai_subject_binding_init(&binding,
                                        "binding:repo-test",
                                        &case_ref,
                                        &subject_ref,
                                        YAI_BINDING_ATTACHED,
                                        YAI_CONTROL_ENFORCED,
                                        "local"));
    require_ok(yai_op_attempt_init(&attempt,
                                   "op:fs-write-memory",
                                   &case_ref,
                                   &actor_ref,
                                   &subject_ref,
                                   "fs.write",
                                   "internal",
                                   "local",
                                   "mutative",
                                   "write memory test"));
    require_ok(yai_control_decide(&attempt, &decision));
    assert(decision.outcome == YAI_DECISION_REQUIRE_REVIEW);
    require_ok(yai_effect_receipt_from_decision("receipt:fs-write-blocked",
                                                &attempt,
                                                &decision,
                                                &receipt));
    assert(receipt.status == YAI_RECEIPT_BLOCKED);

    require_ok(yai_journal_file_init(&journal_file, journal_path));

    append_record(&journal_file, "record:case", &case_ref, YAI_RECORD_CASE, 0, 0, 0, 0, "case:new6-memory opened");
    append_record(&journal_file, "record:binding", &case_ref, YAI_RECORD_SUBJECT_BINDING, &subject_ref, 0, 0, 0, "subject:repo-test bound");
    append_record(&journal_file, "record:attempt", &case_ref, YAI_RECORD_ATTEMPT, &subject_ref, &attempt.attempt_id, 0, 0, "op:fs.write mutative");
    append_record(&journal_file, "record:decision", &case_ref, YAI_RECORD_DECISION, &subject_ref, &attempt.attempt_id, &decision.decision_id, 0, "decision:require_review");
    append_record(&journal_file, "record:receipt", &case_ref, YAI_RECORD_RECEIPT, &subject_ref, &attempt.attempt_id, &decision.decision_id, &receipt.receipt_id, "receipt:blocked");

    require_ok(yai_graph_edge_init(&edge, "edge:case-subject", &case_ref, YAI_GRAPH_EDGE_CASE_BINDS_SUBJECT, case_ref.case_id.value, subject_ref.subject_id.value, "case binds subject"));
    append_edge(&journal_file, "record:edge-case-subject", &edge, &subject_ref, 0, 0, 0);
    require_ok(yai_graph_edge_init(&edge, "edge:op-subject", &case_ref, YAI_GRAPH_EDGE_OP_TARGETS_SUBJECT, attempt.attempt_id.value, subject_ref.subject_id.value, "op targets subject"));
    append_edge(&journal_file, "record:edge-op-subject", &edge, &subject_ref, &attempt.attempt_id, 0, 0);
    require_ok(yai_graph_edge_init(&edge, "edge:decision-op", &case_ref, YAI_GRAPH_EDGE_DECISION_CONTROLS_OP, decision.decision_id.value, attempt.attempt_id.value, "decision controls op"));
    append_edge(&journal_file, "record:edge-decision-op", &edge, &subject_ref, &attempt.attempt_id, &decision.decision_id, 0);
    require_ok(yai_graph_edge_init(&edge, "edge:receipt-effect", &case_ref, YAI_GRAPH_EDGE_RECEIPT_RECORDS_EFFECT, receipt.receipt_id.value, "effect:fs.write", "receipt records effect"));
    append_edge(&journal_file, "record:edge-receipt-effect", &edge, &subject_ref, &attempt.attempt_id, &decision.decision_id, &receipt.receipt_id);
    require_ok(yai_graph_edge_init(&edge, "edge:receipt-subject", &case_ref, YAI_GRAPH_EDGE_RECEIPT_UPDATES_SUBJECT, receipt.receipt_id.value, subject_ref.subject_id.value, "receipt updates subject"));
    append_edge(&journal_file, "record:edge-receipt-subject", &edge, &subject_ref, &attempt.attempt_id, &decision.decision_id, &receipt.receipt_id);

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_reconstruct_receipt_chain(&reloaded_journal, receipt.receipt_id.value, &reconstruction));
    require_ok(yai_memory_candidate_from_reconstruction(&reloaded_journal,
                                                        &reconstruction,
                                                        YAI_MEMORY_DECISION,
                                                        &candidate));
    assert(candidate.memory_kind == YAI_MEMORY_DECISION);
    assert(candidate.basis_record_count == yai_journal_count(&reloaded_journal));
    assert(candidate.basis_receipt_count == 1);
    assert(candidate.basis_edge_count == 5);
    yai_journal_free(&reloaded_journal);

    append_record(&journal_file,
                  "record:memory-candidate",
                  &candidate.case_ref,
                  YAI_RECORD_MEMORY_CANDIDATE,
                  &candidate.subject_ref,
                  &attempt.attempt_id,
                  &candidate.decision_ref,
                  &candidate.receipt_ref,
                  candidate.summary);

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_projection_build("projection:memory",
                                    &case_ref,
                                    YAI_PROJECTION_OPERATOR,
                                    &reloaded_journal,
                                    &projection));
    assert(projection.memory_candidate_count == 1);
    assert(projection.decision_memory_candidate_count == 1);

    printf("memory:candidate derived\n");
    printf("memory:kind %s\n", yai_memory_kind_string(candidate.memory_kind));
    printf("memory:basis records:%zu receipts:%zu edges:%zu\n",
           candidate.basis_record_count,
           candidate.basis_receipt_count,
           candidate.basis_edge_count);
    printf("projection:memory summary built\n");

    yai_journal_free(&reloaded_journal);
    return 0;
}
