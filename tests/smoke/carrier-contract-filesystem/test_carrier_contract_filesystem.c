/*
 * YAI - SPINE.33C carrier contract filesystem smoke
 *
 * Implements:
 *   Validation for carrier.v1 primitives and filesystem carrier receipt
 *   mapping.
 *
 * This file owns:
 *   No-extra-carrier-execution smoke coverage for filesystem as carrier.v1.
 *
 * This file does not own:
 *   Process, network, database, git or model carrier execution.
 */

#include "yai/yai.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

static void require_ok(yai_status_t status) {
    if (status != YAI_OK) {
        printf("unexpected status: %s\n", yai_status_string(status));
        assert(status == YAI_OK);
    }
}

static void make_tmp_dirs(const char *run_dir, const char *sandbox) {
    (void)mkdir("build", 0777);
    (void)mkdir("build/tmp", 0777);
    (void)mkdir("build/tmp/spine33c", 0777);
    (void)mkdir(run_dir, 0777);
    (void)mkdir(sandbox, 0777);
}

static void write_fixture(const char *path, const char *content) {
    FILE *file = fopen(path, "wb");
    assert(file != 0);
    assert(fwrite(content, 1, strlen(content), file) == strlen(content));
    assert(fclose(file) == 0);
}

static int check_outcome_roundtrip(void) {
    const yai_carrier_outcome_t values[] = {
        YAI_CARRIER_OUTCOME_EXECUTED,
        YAI_CARRIER_OUTCOME_BLOCKED,
        YAI_CARRIER_OUTCOME_DEFERRED,
        YAI_CARRIER_OUTCOME_OBSERVED,
        YAI_CARRIER_OUTCOME_FAILED,
        YAI_CARRIER_OUTCOME_QUARANTINED,
        YAI_CARRIER_OUTCOME_WAITING_OPERATOR,
        YAI_CARRIER_OUTCOME_WAITING_AGENT,
        YAI_CARRIER_OUTCOME_NOT_ATTEMPTED,
    };
    size_t i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i) {
        const char *name = yai_carrier_outcome_to_string(values[i]);
        if (yai_carrier_outcome_from_string(name) != values[i]) return 1;
    }
    if (yai_carrier_outcome_from_string("nope") != YAI_CARRIER_OUTCOME_UNKNOWN) return 1;
    printf("carrier_outcome:roundtrip ok\n");
    return 0;
}

static void make_allow_with_constraints_decision(const yai_op_attempt_t *attempt,
                                                 yai_control_decision_t *decision) {
    yai_id_set(&decision->decision_id, "decision:new12-fs-write");
    decision->case_ref = attempt->case_ref;
    decision->attempt_id = attempt->attempt_id;
    yai_id_set(&decision->basis_id, "basis:carrier-contract");
    yai_id_set(&decision->gate_id, "gate:carrier-contract");
    yai_id_set(&decision->rule_id, "rule:allow-with-constraints");
    yai_id_set(&decision->obligation_id, "");
    yai_id_set(&decision->receipt_requirement_id, "receipt-requirement:carrier-v1");
    decision->outcome = YAI_DECISION_ALLOW_WITH_CONSTRAINTS;
    decision->receipt_required = 1;
    (void)snprintf(decision->reason, sizeof(decision->reason), "carrier.v1 allow");
}

int main(void) {
    char run_dir[128];
    char sandbox[160];
    char input_path[192];
    char blocked_path[192];
    char output_path[192];
    yai_case_ref_t case_ref;
    yai_subject_ref_t actor_ref;
    yai_subject_ref_t file_subject_ref;
    yai_op_attempt_t read_attempt;
    yai_op_attempt_t blocked_write_attempt;
    yai_op_attempt_t allowed_write_attempt;
    yai_control_decision_t read_decision;
    yai_control_decision_t blocked_write_decision;
    yai_control_decision_t allowed_write_decision;
    yai_effect_receipt_t read_receipt;
    yai_effect_receipt_t blocked_write_receipt;
    yai_effect_receipt_t allowed_write_receipt;
    yai_carrier_receipt_t carrier_receipt;
    int written = 0;

    assert(strcmp(yai_carrier_contract_version(), "carrier.v1") == 0);
    assert(strcmp(yai_filesystem_carrier_contract_version(), "carrier.v1") == 0);
    assert(strcmp(yai_carrier_contract_phase_to_string(YAI_CARRIER_PHASE_PRE_STATE_OBSERVATION), "pre_state_observation") == 0);
    printf("carrier_contract:v1\n");
    if (check_outcome_roundtrip() != 0) return 1;
    require_ok(yai_carrier_receipt_init(&carrier_receipt));
    assert(carrier_receipt.carrier_family == YAI_CARRIER_FAMILY_UNKNOWN);
    printf("carrier_receipt:init ok\n");

    written = snprintf(run_dir, sizeof(run_dir), "build/tmp/spine33c/filesystem-%ld", (long)getpid());
    assert(written > 0 && (size_t)written < sizeof(run_dir));
    written = snprintf(sandbox, sizeof(sandbox), "%s/sandbox", run_dir);
    assert(written > 0 && (size_t)written < sizeof(sandbox));
    written = snprintf(input_path, sizeof(input_path), "%s/input.txt", sandbox);
    assert(written > 0 && (size_t)written < sizeof(input_path));
    written = snprintf(blocked_path, sizeof(blocked_path), "%s/blocked.txt", sandbox);
    assert(written > 0 && (size_t)written < sizeof(blocked_path));
    written = snprintf(output_path, sizeof(output_path), "%s/output.txt", sandbox);
    assert(written > 0 && (size_t)written < sizeof(output_path));

    make_tmp_dirs(run_dir, sandbox);
    (void)unlink(input_path);
    (void)unlink(blocked_path);
    (void)unlink(output_path);
    write_fixture(input_path, "hello carrier.v1\n");

    require_ok(yai_case_ref_init(&case_ref, "case:new12-filesystem", "new12", "open"));
    require_ok(yai_subject_ref_init(&actor_ref, "subject:agent-test", "agent", "local:test-agent"));
    require_ok(yai_subject_ref_init(&file_subject_ref, "subject:filesystem-sandbox", "filesystem", input_path));

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
    require_ok(yai_filesystem_carrier_read("receipt:new12-fs-read", &read_attempt, &read_decision, sandbox, input_path, &read_receipt));
    require_ok(yai_filesystem_carrier_receipt_from_effect_receipt(&read_receipt, &carrier_receipt));
    assert(carrier_receipt.carrier_family == YAI_CARRIER_FAMILY_FILESYSTEM);
    assert(carrier_receipt.outcome == YAI_CARRIER_OUTCOME_OBSERVED);
    assert(carrier_receipt.guarantee_mode == YAI_RECEIPT_GUARANTEE_INTERPOSED);
    assert(carrier_receipt.pre_state_observed);
    assert(carrier_receipt.evidence_captured);

    require_ok(yai_op_attempt_init(&blocked_write_attempt,
                                   "op:fs-write-block",
                                   &case_ref,
                                   &actor_ref,
                                   &file_subject_ref,
                                   "fs.write",
                                   "internal",
                                   "local",
                                   "mutative",
                                   "blocked write"));
    require_ok(yai_control_decide(&blocked_write_attempt, &blocked_write_decision));
    require_ok(yai_filesystem_carrier_write("receipt:new12-fs-block", &blocked_write_attempt, &blocked_write_decision, sandbox, blocked_path, "blocked", &blocked_write_receipt));
    require_ok(yai_filesystem_carrier_receipt_from_effect_receipt(&blocked_write_receipt, &carrier_receipt));
    assert(carrier_receipt.outcome == YAI_CARRIER_OUTCOME_BLOCKED);
    assert(access(blocked_path, F_OK) != 0);

    require_ok(yai_op_attempt_init(&allowed_write_attempt,
                                   "op:fs-write-exec",
                                   &case_ref,
                                   &actor_ref,
                                   &file_subject_ref,
                                   "fs.write",
                                   "internal",
                                   "local",
                                   "mutative",
                                   "allowed write"));
    make_allow_with_constraints_decision(&allowed_write_attempt, &allowed_write_decision);
    require_ok(yai_filesystem_carrier_write("receipt:new12-fs-write", &allowed_write_attempt, &allowed_write_decision, sandbox, output_path, "allowed\n", &allowed_write_receipt));
    require_ok(yai_filesystem_carrier_receipt_from_effect_receipt(&allowed_write_receipt, &carrier_receipt));
    assert(carrier_receipt.outcome == YAI_CARRIER_OUTCOME_EXECUTED);
    assert(carrier_receipt.post_state_observed);
    assert(carrier_receipt.receipt_validated);
    assert(carrier_receipt.residue_recorded);

    printf("filesystem:carrier.v1 active_minimal\n");
    printf("filesystem:read observed\n");
    printf("filesystem:write decision_required\n");
    printf("filesystem:receipt mapping ok\n");
    printf("no_extra_carrier_execution:confirmed\n");
    return 0;
}
