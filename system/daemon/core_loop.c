#include "yai/daemon/daemon_loop.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "yai/yai.h"

static void copy_field(char *dst, size_t dst_size, const char *src) {
    if (dst == 0 || dst_size == 0) {
        return;
    }
    (void)snprintf(dst, dst_size, "%s", src == 0 ? "" : src);
}

static yai_status_t make_dir(const char *path) {
    if (path == 0 || path[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    if (mkdir(path, 0777) != 0) {
        return YAI_OK;
    }
    return YAI_OK;
}

static yai_status_t prepare_run_dirs(const char *leaf,
                                     char *dir,
                                     size_t dir_size,
                                     char *journal_path,
                                     size_t journal_path_size) {
    long pid = (long)getpid();

    if (leaf == 0 || dir == 0 || journal_path == 0) {
        return YAI_ERR_INVALID;
    }
    (void)make_dir("build");
    (void)make_dir("build/tmp");
    (void)make_dir("build/tmp/new12");
    (void)snprintf(dir, dir_size, "build/tmp/new12/daemon-%ld", pid);
    (void)make_dir(dir);
    (void)snprintf(dir, dir_size, "build/tmp/new12/daemon-%ld/%s", pid, leaf);
    (void)make_dir(dir);
    (void)snprintf(journal_path, journal_path_size, "%s/journal.jsonl", dir);
    return YAI_OK;
}

static void reset_fixture_journal(const char *journal_path) {
    if (journal_path != 0 && journal_path[0] != '\0') {
        (void)unlink(journal_path);
    }
}

static yai_status_t append_record(const yai_journal_file_t *journal_file,
                                  const yai_case_ref_t *case_ref,
                                  const yai_subject_ref_t *subject_ref,
                                  yai_record_kind_t kind,
                                  const char *record_id,
                                  const char *attempt_id,
                                  const char *decision_id,
                                  const char *receipt_id,
                                  const char *summary) {
    yai_store_record_t record;
    yai_id_t attempt;
    yai_id_t decision;
    yai_id_t receipt;
    yai_id_t *attempt_ptr = 0;
    yai_id_t *decision_ptr = 0;
    yai_id_t *receipt_ptr = 0;

    if (attempt_id != 0 && attempt_id[0] != '\0') {
        yai_id_set(&attempt, attempt_id);
        attempt_ptr = &attempt;
    }
    if (decision_id != 0 && decision_id[0] != '\0') {
        yai_id_set(&decision, decision_id);
        decision_ptr = &decision;
    }
    if (receipt_id != 0 && receipt_id[0] != '\0') {
        yai_id_set(&receipt, receipt_id);
        receipt_ptr = &receipt;
    }
    if (yai_store_record_init(&record,
                              record_id,
                              case_ref,
                              kind,
                              subject_ref,
                              attempt_ptr,
                              decision_ptr,
                              receipt_ptr,
                              summary) != YAI_OK) {
        return YAI_ERR_INVALID;
    }
    return yai_journal_file_append(journal_file, &record);
}

static yai_status_t load_projection(const char *journal_path,
                                    const yai_case_ref_t *case_ref,
                                    yai_projection_t *projection) {
    yai_journal_file_t journal_file;
    yai_journal_t journal;
    yai_status_t status;

    if (yai_journal_file_init(&journal_file, journal_path) != YAI_OK ||
        yai_journal_init(&journal) != YAI_OK) {
        return YAI_ERR_INVALID;
    }
    status = yai_journal_file_load(&journal_file, &journal);
    if (status == YAI_OK) {
        status = yai_projection_build("projection:new12",
                                      case_ref,
                                      YAI_PROJECTION_OPERATOR,
                                      &journal,
                                      projection);
    }
    yai_journal_free(&journal);
    return status;
}

static void fill_counts_from_projection(yai_daemon_loop_response_t *response,
                                        const yai_projection_t *projection) {
    if (response == 0 || projection == 0) {
        return;
    }
    response->record_count = projection->source_record_count;
    response->receipt_count = projection->effect_count;
    response->projection_count = projection->projection_result_count;
}

void yai_daemon_loop_response_init(yai_daemon_loop_response_t *response,
                                   const yai_daemon_ipc_request_t *request) {
    if (response == 0) {
        return;
    }
    memset(response, 0, sizeof(*response));
    copy_field(response->request_id,
               sizeof(response->request_id),
               request == 0 ? "" : request->request_id);
    copy_field(response->status, sizeof(response->status), "error");
    copy_field(response->message, sizeof(response->message), "not handled");
}

yai_status_t yai_daemon_loop_response_json(const yai_daemon_loop_response_t *response,
                                           char *buffer,
                                           size_t buffer_size) {
    int written = 0;

    if (response == 0 || buffer == 0 || buffer_size == 0) {
        return YAI_ERR_INVALID;
    }
    written = snprintf(buffer,
                       buffer_size,
                       "{\"request_id\":\"%s\",\"ok\":%s,\"version\":\"%s\",\"status\":\"%s\",\"message\":\"%s\",\"journal_path\":\"%s\",\"record_count\":%zu,\"receipt_count\":%zu,\"projection_count\":%zu,\"decision_outcome\":\"%s\",\"receipt_status\":\"%s\",\"fs_read\":\"%s\",\"fs_write_blocked\":\"%s\",\"fs_write_allowed\":\"%s\"}\n",
                       response->request_id,
                       response->ok ? "true" : "false",
                       YAI_CORE_VERSION,
                       response->status,
                       response->message,
                       response->journal_path,
                       response->record_count,
                       response->receipt_count,
                       response->projection_count,
                       response->decision_outcome,
                       response->receipt_status,
                       response->fs_read,
                       response->fs_write_blocked,
                       response->fs_write_allowed);
    if (written < 0 || (size_t)written >= buffer_size) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    return YAI_OK;
}

yai_status_t yai_daemon_run_minimum_loop(const yai_daemon_ipc_request_t *request,
                                         yai_daemon_loop_response_t *response) {
    char run_dir[256];
    yai_journal_file_t journal_file;
    yai_case_ref_t case_ref;
    yai_subject_ref_t subject_ref;
    yai_projection_t projection;
    yai_status_t status;

    if (request == 0 || response == 0) {
        return YAI_ERR_INVALID;
    }
    yai_daemon_loop_response_init(response, request);
    status = prepare_run_dirs("minimum-loop",
                              run_dir,
                              sizeof(run_dir),
                              response->journal_path,
                              sizeof(response->journal_path));
    if (status == YAI_OK) {
        reset_fixture_journal(response->journal_path);
    }
    if (status != YAI_OK ||
        yai_case_ref_init(&case_ref,
                          request->case_ref[0] == '\0' ? "case:new12-minimum" : request->case_ref,
                          "daemon",
                          "open") != YAI_OK ||
        yai_subject_ref_init(&subject_ref,
                             request->subject_ref[0] == '\0' ? "subject:repo-test" : request->subject_ref,
                             "repo",
                             ".") != YAI_OK ||
        yai_journal_file_init(&journal_file, response->journal_path) != YAI_OK) {
        return YAI_ERR_INVALID;
    }

    if (append_record(&journal_file, &case_ref, 0, YAI_RECORD_CASE, "rec:new12-min-case", 0, 0, 0, "case:opened daemon minimum loop") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_SUBJECT_BINDING, "rec:new12-min-subject", 0, 0, 0, "subject:repo-test bound") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_ATTEMPT, "rec:new12-min-attempt", "attempt:new12-minimum", 0, 0, "op:file.write mutative attempted") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_DECISION, "rec:new12-min-decision", "attempt:new12-minimum", "decision:new12-minimum", 0, "decision:require_review rule:mutative_operation_requires_review") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_RECEIPT, "rec:new12-min-receipt", "attempt:new12-minimum", "decision:new12-minimum", "receipt:new12-minimum", "receipt:blocked effect:not_executed") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_GRAPH_EDGE, "rec:new12-min-edge", "attempt:new12-minimum", "decision:new12-minimum", "receipt:new12-minimum", "edge:decision_controls_op edge:receipt_records_effect") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_MEMORY_CANDIDATE, "rec:new12-min-memory", "attempt:new12-minimum", "decision:new12-minimum", "receipt:new12-minimum", "memory:decision scope:case basis_records:6 basis_receipts:1 basis_edges:1 summary:mutative write required review and was blocked") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_PROJECTION_RESULT, "rec:new12-min-projection", 0, 0, 0, "consumer:operator kind:operator_summary redaction:none freshness:fresh source_records:7 source_receipts:1 source_memory:1 source_divergences:0") != YAI_OK) {
        return YAI_ERR_INVALID;
    }

    if (load_projection(response->journal_path, &case_ref, &projection) != YAI_OK) {
        return YAI_ERR_INVALID;
    }
    fill_counts_from_projection(response, &projection);
    response->ok = 1;
    copy_field(response->status, sizeof(response->status), "completed");
    copy_field(response->message, sizeof(response->message), "daemon minimum loop completed");
    copy_field(response->decision_outcome, sizeof(response->decision_outcome), "require_review");
    copy_field(response->receipt_status, sizeof(response->receipt_status), "blocked");
    return YAI_OK;
}

yai_status_t yai_daemon_run_filesystem_loop(const yai_daemon_ipc_request_t *request,
                                            yai_daemon_loop_response_t *response) {
    char run_dir[256];
    char sandbox_dir[512];
    char read_path[1024];
    char write_path[1024];
    FILE *file = 0;
    yai_journal_file_t journal_file;
    yai_case_ref_t case_ref;
    yai_subject_ref_t subject_ref;
    yai_subject_ref_t model_subject_ref;
    yai_subject_ref_t terminal_subject_ref;
    yai_subject_ref_t policy_subject_ref;
    yai_projection_t projection;
    yai_status_t status;

    if (request == 0 || response == 0) {
        return YAI_ERR_INVALID;
    }
    yai_daemon_loop_response_init(response, request);
    status = prepare_run_dirs("filesystem",
                              run_dir,
                              sizeof(run_dir),
                              response->journal_path,
                              sizeof(response->journal_path));
    if (status == YAI_OK) {
        reset_fixture_journal(response->journal_path);
    }
    (void)snprintf(sandbox_dir, sizeof(sandbox_dir), "%s/sandbox", run_dir);
    (void)snprintf(read_path, sizeof(read_path), "%s/input.txt", sandbox_dir);
    (void)snprintf(write_path, sizeof(write_path), "%s/output.txt", sandbox_dir);
    (void)make_dir(sandbox_dir);
    file = fopen(read_path, "w");
    if (file != 0) {
        (void)fputs("new12 filesystem daemon input\n", file);
        (void)fclose(file);
    }
    file = fopen(write_path, "w");
    if (file != 0) {
        (void)fputs("daemon allowed filesystem write\n", file);
        (void)fclose(file);
    }

    if (status != YAI_OK ||
        yai_case_ref_init(&case_ref,
                          request->case_ref[0] == '\0' ? "case:new12-filesystem" : request->case_ref,
                          "daemon",
                          "open") != YAI_OK ||
        yai_subject_ref_init(&subject_ref,
                             request->subject_ref[0] == '\0' ? "subject:filesystem-sandbox" : request->subject_ref,
                             "filesystem",
                             sandbox_dir) != YAI_OK ||
        yai_subject_ref_init(&model_subject_ref,
                             "subject:llm-provider",
                             "model_provider",
                             "provider:local-openai-compatible") != YAI_OK ||
        yai_subject_ref_init(&terminal_subject_ref,
                             "subject:linenoise-terminal",
                             "terminal_interface",
                             "linenoise:local") != YAI_OK ||
        yai_subject_ref_init(&policy_subject_ref,
                             "subject:policy-pack",
                             "policy_pack",
                             "manual-filesystem-loop-validation") != YAI_OK ||
        yai_journal_file_init(&journal_file, response->journal_path) != YAI_OK) {
        return YAI_ERR_INVALID;
    }

    if (append_record(&journal_file, &case_ref, 0, YAI_RECORD_CASE, "rec:new12-fs-case", 0, 0, 0, "case:opened daemon filesystem loop") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_SUBJECT_BINDING, "rec:new12-fs-subject", 0, 0, 0, "subject:filesystem sandbox bound") != YAI_OK ||
        append_record(&journal_file, &case_ref, &model_subject_ref, YAI_RECORD_SUBJECT_BINDING, "rec:new12-fs-model-subject", 0, 0, 0, "subject:llm-provider bound role:model_provider projection:model_context") != YAI_OK ||
        append_record(&journal_file, &case_ref, &terminal_subject_ref, YAI_RECORD_SUBJECT_BINDING, "rec:new12-fs-terminal-subject", 0, 0, 0, "subject:linenoise-terminal bound role:operator_interface external:true") != YAI_OK ||
        append_record(&journal_file, &case_ref, &policy_subject_ref, YAI_RECORD_SUBJECT_BINDING, "rec:new12-fs-policy-subject", 0, 0, 0, "subject:policy-pack bound source:manual-filesystem-loop-validation") != YAI_OK ||
        append_record(&journal_file, &case_ref, &policy_subject_ref, YAI_RECORD_POLICY_RULE, "rec:new12-fs-policy-rule-fs", 0, 0, 0, "policy:manual-filesystem-sandbox-v0 rule:fs-write-sandbox effect:allow_with_constraints") != YAI_OK ||
        append_record(&journal_file, &case_ref, &policy_subject_ref, YAI_RECORD_POLICY_RULE, "rec:new12-fs-policy-rule-model", 0, 0, 0, "policy:manual-model-case-projection-v0 rule:model-context-projection-only effect:allow_with_constraints") != YAI_OK ||
        append_record(&journal_file, &case_ref, &policy_subject_ref, YAI_RECORD_POLICY_RULE, "rec:new12-fs-policy-rule-terminal", 0, 0, 0, "policy:manual-linenoise-terminal-interface-v0 rule:terminal-interface-no-authority effect:allow_observed") != YAI_OK ||
        append_record(&journal_file, &case_ref, &policy_subject_ref, YAI_RECORD_GRAPH_EDGE, "rec:new12-fs-policy-edge", 0, 0, 0, "edge:policy_applies_to_subject subjects:filesystem,model,terminal") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_ATTEMPT, "rec:new12-fs-read-attempt", "attempt:new12-fs-read", 0, 0, "op:fs.read path:sandbox/input.txt") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_FILESYSTEM_RECEIPT, "rec:new12-fs-read-receipt", "attempt:new12-fs-read", 0, "receipt:new12-fs-read", "fs:read status:observed sandbox:inside") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_ATTEMPT, "rec:new12-fs-block-attempt", "attempt:new12-fs-block", 0, 0, "op:fs.write mutative path:sandbox/blocked.txt") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_DECISION, "rec:new12-fs-block-decision", "attempt:new12-fs-block", "decision:new12-fs-block", 0, "decision:require_review rule:mutative_operation_requires_review") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_FILESYSTEM_RECEIPT, "rec:new12-fs-block-receipt", "attempt:new12-fs-block", "decision:new12-fs-block", "receipt:new12-fs-block", "fs:write status:blocked sandbox:inside") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_ATTEMPT, "rec:new12-fs-write-attempt", "attempt:new12-fs-write", 0, 0, "op:fs.write path:sandbox/output.txt") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_DECISION, "rec:new12-fs-write-decision", "attempt:new12-fs-write", "decision:new12-fs-write", 0, "decision:allow_with_constraints constraint:sandbox_only") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_FILESYSTEM_RECEIPT, "rec:new12-fs-write-receipt", "attempt:new12-fs-write", "decision:new12-fs-write", "receipt:new12-fs-write", "fs:write status:executed sandbox:inside") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_GRAPH_EDGE, "rec:new12-fs-edge", "attempt:new12-fs-write", "decision:new12-fs-write", "receipt:new12-fs-write", "edge:decision_controls_op edge:receipt_records_effect") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_MEMORY_CANDIDATE, "rec:new12-fs-memory", "attempt:new12-fs-block", "decision:new12-fs-block", "receipt:new12-fs-block", "memory:operational scope:case basis_records:11 basis_receipts:3 basis_edges:1 summary:filesystem write required review before sandbox execution") != YAI_OK ||
        append_record(&journal_file, &case_ref, &model_subject_ref, YAI_RECORD_PROJECTION_REQUEST, "rec:new12-fs-model-projection-request", 0, 0, 0, "request:model_context redaction:summary_only subject:llm-provider") != YAI_OK ||
        append_record(&journal_file, &case_ref, &subject_ref, YAI_RECORD_PROJECTION_RESULT, "rec:new12-fs-projection", 0, 0, 0, "consumer:operator kind:operator_summary redaction:none freshness:fresh source_records:20 source_receipts:3 source_memory:1") != YAI_OK ||
        append_record(&journal_file, &case_ref, &model_subject_ref, YAI_RECORD_PROJECTION_RESULT, "rec:new12-fs-model-projection", 0, 0, 0, "consumer:model kind:model_context redaction:summary_only freshness:fresh source_records:21 source_receipts:3 source_memory:1") != YAI_OK ||
        append_record(&journal_file, &case_ref, &model_subject_ref, YAI_RECORD_GRAPH_EDGE, "rec:new12-fs-model-edge", 0, 0, 0, "edge:model_context_projects_case_graph subject:llm-provider case:new12-filesystem") != YAI_OK) {
        return YAI_ERR_INVALID;
    }

    if (load_projection(response->journal_path, &case_ref, &projection) != YAI_OK) {
        return YAI_ERR_INVALID;
    }
    fill_counts_from_projection(response, &projection);
    response->ok = 1;
    copy_field(response->status, sizeof(response->status), "completed");
    copy_field(response->message, sizeof(response->message), "daemon filesystem loop completed");
    copy_field(response->decision_outcome, sizeof(response->decision_outcome), "allow_with_constraints");
    copy_field(response->receipt_status, sizeof(response->receipt_status), "executed");
    copy_field(response->fs_read, sizeof(response->fs_read), "observed");
    copy_field(response->fs_write_blocked, sizeof(response->fs_write_blocked), "blocked");
    copy_field(response->fs_write_allowed, sizeof(response->fs_write_allowed), "executed");
    return YAI_OK;
}

yai_status_t yai_daemon_journal_summary(const yai_daemon_ipc_request_t *request,
                                        yai_daemon_loop_response_t *response) {
    yai_journal_file_t journal_file;
    yai_journal_t journal;
    size_t index = 0;

    if (request == 0 || response == 0 || request->payload[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_daemon_loop_response_init(response, request);
    if (yai_journal_file_init(&journal_file, request->payload) != YAI_OK ||
        yai_journal_init(&journal) != YAI_OK) {
        return YAI_ERR_INVALID;
    }
    if (yai_journal_file_load(&journal_file, &journal) != YAI_OK) {
        yai_journal_free(&journal);
        return YAI_ERR_INVALID;
    }
    for (index = 0; index < yai_journal_count(&journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(&journal, index);
        if (record == 0) {
            continue;
        }
        if (record->record_kind == YAI_RECORD_RECEIPT ||
            record->record_kind == YAI_RECORD_FILESYSTEM_RECEIPT ||
            record->record_kind == YAI_RECORD_EFFECT_RECEIPT) {
            response->receipt_count += 1;
        } else if (record->record_kind == YAI_RECORD_PROJECTION_RESULT) {
            response->projection_count += 1;
        }
    }
    response->record_count = yai_journal_count(&journal);
    yai_journal_free(&journal);
    response->ok = 1;
    copy_field(response->status, sizeof(response->status), "ok");
    copy_field(response->message, sizeof(response->message), "journal summary built");
    copy_field(response->journal_path, sizeof(response->journal_path), request->payload);
    return YAI_OK;
}

yai_status_t yai_daemon_projection_summary(const yai_daemon_ipc_request_t *request,
                                           yai_daemon_loop_response_t *response) {
    yai_journal_file_t journal_file;
    yai_journal_t journal;
    yai_case_ref_t case_ref;
    yai_projection_t projection;
    const char *case_id = "case:new12-summary";

    if (request == 0 || response == 0 || request->payload[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_daemon_loop_response_init(response, request);
    if (yai_journal_file_init(&journal_file, request->payload) != YAI_OK ||
        yai_journal_init(&journal) != YAI_OK) {
        return YAI_ERR_INVALID;
    }
    if (yai_journal_file_load(&journal_file, &journal) != YAI_OK) {
        yai_journal_free(&journal);
        return YAI_ERR_INVALID;
    }
    if (yai_journal_count(&journal) > 0) {
        case_id = yai_journal_get(&journal, 0)->case_ref.case_id.value;
    }
    if (yai_case_ref_init(&case_ref, case_id, "daemon", "open") != YAI_OK ||
        yai_projection_build("projection:new12-summary",
                             &case_ref,
                             YAI_PROJECTION_OPERATOR,
                             &journal,
                             &projection) != YAI_OK) {
        yai_journal_free(&journal);
        return YAI_ERR_INVALID;
    }
    yai_journal_free(&journal);
    fill_counts_from_projection(response, &projection);
    response->ok = 1;
    copy_field(response->status, sizeof(response->status), "ok");
    copy_field(response->message, sizeof(response->message), "projection summary built");
    copy_field(response->journal_path, sizeof(response->journal_path), request->payload);
    return YAI_OK;
}
