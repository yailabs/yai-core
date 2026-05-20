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
    (void)mkdir("build/tmp/new5", 0777);
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

    (void)snprintf(summary,
                   sizeof(summary),
                   "edge:%s from:%.64s to:%.64s",
                   yai_graph_edge_kind_string(edge->edge_kind),
                   edge->from_ref.value,
                   edge->to_ref.value);
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
    yai_graph_t graph;
    yai_reconstruction_t reconstruction;
    yai_projection_t projection;
    int written = 0;

    written = snprintf(run_dir,
                       sizeof(run_dir),
                       "build/tmp/new5/graph-reconstruction-%ld",
                       (long)getpid());
    assert(written > 0 && (size_t)written < sizeof(run_dir));
    written = snprintf(journal_path, sizeof(journal_path), "%s/journal.jsonl", run_dir);
    assert(written > 0 && (size_t)written < sizeof(journal_path));

    make_tmp_dirs(run_dir);
    (void)unlink(journal_path);

    require_ok(yai_case_ref_init(&case_ref, "case:new5-graph", "new5-test", "open"));
    require_ok(yai_subject_ref_init(&actor_ref, "subject:agent-test", "agent", "local:test-agent"));
    require_ok(yai_subject_ref_init(&subject_ref, "subject:repo-test", "repository", "repo://new5-test"));
    require_ok(yai_subject_binding_init(&binding,
                                        "binding:repo-test",
                                        &case_ref,
                                        &subject_ref,
                                        YAI_BINDING_ATTACHED,
                                        YAI_CONTROL_ENFORCED,
                                        "local"));
    require_ok(yai_op_attempt_init(&attempt,
                                   "op:fs-write-graph",
                                   &case_ref,
                                   &actor_ref,
                                   &subject_ref,
                                   "fs.write",
                                   "internal",
                                   "local",
                                   "mutative",
                                   "write graph test"));
    require_ok(yai_control_decide(&attempt, &decision));
    assert(decision.outcome == YAI_DECISION_REQUIRE_REVIEW);
    require_ok(yai_effect_receipt_from_decision("receipt:fs-write-blocked",
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
                  "case:new5-graph opened");
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
                  "op:fs.write mutative");
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

    require_ok(yai_graph_edge_init(&edge,
                                   "edge:case-subject",
                                   &case_ref,
                                   YAI_GRAPH_EDGE_CASE_BINDS_SUBJECT,
                                   case_ref.case_id.value,
                                   subject_ref.subject_id.value,
                                   "case binds subject"));
    append_edge(&journal_file, "record:edge-case-subject", &edge, &subject_ref, 0, 0, 0);

    require_ok(yai_graph_edge_init(&edge,
                                   "edge:op-subject",
                                   &case_ref,
                                   YAI_GRAPH_EDGE_OP_TARGETS_SUBJECT,
                                   attempt.attempt_id.value,
                                   subject_ref.subject_id.value,
                                   "op targets subject"));
    append_edge(&journal_file, "record:edge-op-subject", &edge, &subject_ref, &attempt.attempt_id, 0, 0);

    require_ok(yai_graph_edge_init(&edge,
                                   "edge:decision-op",
                                   &case_ref,
                                   YAI_GRAPH_EDGE_DECISION_CONTROLS_OP,
                                   decision.decision_id.value,
                                   attempt.attempt_id.value,
                                   "decision controls op"));
    append_edge(&journal_file, "record:edge-decision-op", &edge, &subject_ref, &attempt.attempt_id, &decision.decision_id, 0);

    require_ok(yai_graph_edge_init(&edge,
                                   "edge:receipt-effect",
                                   &case_ref,
                                   YAI_GRAPH_EDGE_RECEIPT_RECORDS_EFFECT,
                                   receipt.receipt_id.value,
                                   "effect:fs.write",
                                   "receipt records effect"));
    append_edge(&journal_file, "record:edge-receipt-effect", &edge, &subject_ref, &attempt.attempt_id, &decision.decision_id, &receipt.receipt_id);

    require_ok(yai_graph_edge_init(&edge,
                                   "edge:receipt-subject",
                                   &case_ref,
                                   YAI_GRAPH_EDGE_RECEIPT_UPDATES_SUBJECT,
                                   receipt.receipt_id.value,
                                   subject_ref.subject_id.value,
                                   "receipt updates subject"));
    append_edge(&journal_file, "record:edge-receipt-subject", &edge, &subject_ref, &attempt.attempt_id, &decision.decision_id, &receipt.receipt_id);

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_graph_from_journal(&reloaded_journal, &graph));
    assert(yai_graph_count(&graph) == 5);
    assert(yai_graph_count_kind(&graph, YAI_GRAPH_EDGE_CASE_BINDS_SUBJECT) == 1);
    assert(yai_graph_count_kind(&graph, YAI_GRAPH_EDGE_OP_TARGETS_SUBJECT) == 1);
    assert(yai_graph_count_kind(&graph, YAI_GRAPH_EDGE_DECISION_CONTROLS_OP) == 1);
    assert(yai_graph_count_kind(&graph, YAI_GRAPH_EDGE_RECEIPT_RECORDS_EFFECT) == 1);
    assert(yai_graph_count_kind(&graph, YAI_GRAPH_EDGE_RECEIPT_UPDATES_SUBJECT) == 1);
    yai_graph_free(&graph);

    require_ok(yai_reconstruct_receipt_chain(&reloaded_journal,
                                             receipt.receipt_id.value,
                                             &reconstruction));
    assert(yai_id_equal(&reconstruction.case_id, &case_ref.case_id));
    assert(yai_id_equal(&reconstruction.attempt_id, &attempt.attempt_id));
    assert(yai_id_equal(&reconstruction.decision_id, &decision.decision_id));
    assert(yai_id_equal(&reconstruction.subject_id, &subject_ref.subject_id));

    append_record(&journal_file,
                  "record:reconstruction",
                  &case_ref,
                  YAI_RECORD_RECONSTRUCTION,
                  &subject_ref,
                  &attempt.attempt_id,
                  &decision.decision_id,
                  &receipt.receipt_id,
                  "reconstruct:receipt chain built");
    yai_journal_free(&reloaded_journal);

    require_ok(yai_journal_init(&reloaded_journal));
    require_ok(yai_journal_file_load(&journal_file, &reloaded_journal));
    require_ok(yai_projection_build("projection:graph",
                                    &case_ref,
                                    YAI_PROJECTION_OPERATOR,
                                    &reloaded_journal,
                                    &projection));
    assert(projection.graph_edge_count == 5);
    assert(projection.reconstruction_count == 1);

    printf("graph:edges appended:5\n");
    printf("reconstruct:receipt chain built\n");
    printf("chain:case->op->decision->receipt->subject\n");
    printf("projection:graph summary built\n");

    yai_journal_free(&reloaded_journal);
    return 0;
}
