#include "yai/yai.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static int expect_status(yai_status_t status, const char *label) {
    if (status != YAI_OK) {
        fprintf(stderr, "%s failed: %s\n", label, yai_status_string(status));
        return 1;
    }
    return 0;
}

int main(void) {
    yai_hot_state_t state;
    yai_hot_snapshot_t snapshot;

    if (expect_status(yai_hot_state_init(&state), "hot init") ||
        expect_status(yai_hot_state_set_case(&state, "case:spine23-hot"), "case set") ||
        expect_status(yai_hot_state_set_session(&state, "session:spine23-hot"), "session set") ||
        expect_status(yai_hot_state_set_context(&state, "context:spine23-hot"), "context set")) {
        return 1;
    }
    printf("hot_state:initialized\n");
    printf("case_session:active\n");
    printf("case_context:active\n");

    if (expect_status(yai_hot_state_mark_projection(&state, "projection:initial"),
                      "projection mark") ||
        expect_status(yai_hot_state_snapshot(&state, &snapshot), "projection snapshot")) {
        return 1;
    }
    if (snapshot.projection_freshness != YAI_HOT_FRESHNESS_FRESH) {
        fprintf(stderr, "projection was not fresh\n");
        return 1;
    }
    printf("projection:fresh\n");

    if (expect_status(yai_hot_state_mark_decision(&state, "decision:spine23"), "decision mark") ||
        expect_status(yai_hot_state_mark_receipt(&state, "receipt:spine23"), "receipt mark") ||
        expect_status(yai_hot_state_invalidate_projection(
                          &state,
                          YAI_HOT_STALE_NEW_RECEIPT_AFTER_PROJECTION),
                      "projection invalidate") ||
        expect_status(yai_hot_state_snapshot(&state, &snapshot), "stale snapshot")) {
        return 1;
    }
    if (snapshot.projection_freshness != YAI_HOT_FRESHNESS_STALE ||
        snapshot.projection_stale_reason != YAI_HOT_STALE_NEW_RECEIPT_AFTER_PROJECTION) {
        fprintf(stderr, "projection stale state mismatch\n");
        return 1;
    }
    printf("receipt:marked\n");
    printf("projection:stale\n");
    printf("stale_reason:%s\n", yai_hot_stale_reason_string(snapshot.projection_stale_reason));

    if (expect_status(yai_hot_state_mark_projection(&state, "projection:refreshed"),
                      "projection refresh") ||
        expect_status(yai_hot_state_snapshot(&state, &snapshot), "refresh snapshot")) {
        return 1;
    }
    if (snapshot.projection_freshness != YAI_HOT_FRESHNESS_FRESH ||
        strcmp(snapshot.current_projection_id, "projection:refreshed") != 0) {
        fprintf(stderr, "projection refresh mismatch\n");
        return 1;
    }
    printf("projection:refreshed\n");

    (void)mkdir("build", 0777);
    (void)mkdir("build/tmp", 0777);
    if (expect_status(yai_hot_snapshot_write_json(&snapshot,
                                                  "build/tmp/spine23-hot-state.json"),
                      "snapshot write")) {
        return 1;
    }
    printf("snapshot:written\n");
    return 0;
}
