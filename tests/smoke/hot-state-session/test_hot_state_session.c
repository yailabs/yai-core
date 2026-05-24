#include "yai/hot/hot_state.h"

#include <stdio.h>
#include <string.h>

static int expect_status(int ok, const char *message) {
    if (!ok) {
        fprintf(stderr, "expected: %s\n", message);
        return 1;
    }
    return 0;
}

int main(void) {
    yai_hot_state_t state;
    yai_hot_snapshot_t snapshot;

    if (yai_hot_state_init(&state) != YAI_OK) {
        return expect_status(0, "hot state init");
    }
    printf("hot_state:initialized\n");

    if (yai_hot_state_set_case(&state, "case:spine25") != YAI_OK ||
        yai_hot_state_set_session(&state, "session:spine25") != YAI_OK ||
        yai_hot_state_set_case_world_loaded(&state) != YAI_OK ||
        yai_hot_state_set_context(&state, "case_context:spine25") != YAI_OK) {
        return expect_status(0, "case session/context lifecycle");
    }
    printf("case_session:active\n");
    printf("case_world:loaded\n");
    printf("case_context:active\n");

    if (yai_hot_state_mark_thread(&state, "thread:spine25") != YAI_OK ||
        yai_hot_state_mark_participant_view(&state, "frame:spine25") != YAI_OK ||
        yai_hot_state_mark_projection(&state, "projection:initial") != YAI_OK) {
        return expect_status(0, "thread participant projection setup");
    }

    if (yai_hot_state_mark_decision(&state, "decision:spine25") != YAI_OK) {
        return expect_status(0, "mark decision");
    }
    if (state.snapshot.projection_freshness != YAI_HOT_FRESHNESS_STALE ||
        state.snapshot.projection_stale_reason != YAI_HOT_STALE_NEW_DECISION_AFTER_PROJECTION) {
        return expect_status(0, "decision makes projection stale");
    }
    printf("decision:marked\n");
    printf("projection:stale reason=new_decision_after_projection\n");

    if (yai_hot_state_mark_receipt(&state, "receipt:spine25") != YAI_OK) {
        return expect_status(0, "mark receipt");
    }
    if (state.snapshot.projection_freshness != YAI_HOT_FRESHNESS_STALE ||
        state.snapshot.projection_stale_reason != YAI_HOT_STALE_NEW_RECEIPT_AFTER_PROJECTION) {
        return expect_status(0, "receipt makes projection stale");
    }
    printf("receipt:marked\n");
    printf("projection:stale reason=new_receipt_after_projection\n");

    if (yai_hot_state_mark_projection(&state, "projection:refreshed") != YAI_OK) {
        return expect_status(0, "projection refresh");
    }
    if (state.snapshot.projection_freshness != YAI_HOT_FRESHNESS_FRESH ||
        state.snapshot.projection_stale_reason != YAI_HOT_STALE_NONE) {
        return expect_status(0, "projection refresh makes state fresh");
    }
    printf("projection:refreshed fresh\n");

    if (yai_hot_state_snapshot(&state, &snapshot) != YAI_OK) {
        return expect_status(0, "snapshot copy");
    }
    if (strcmp(snapshot.case_session_status, "active") != 0 ||
        strcmp(snapshot.case_world_status, "loaded") != 0 ||
        strcmp(snapshot.case_context_status, "active") != 0 ||
        strcmp(snapshot.active_thread_id, "thread:spine25") != 0 ||
        strcmp(snapshot.participant_view_frame_id, "frame:spine25") != 0) {
        return expect_status(0, "session/context fields in snapshot");
    }
    printf("snapshot:session_context_fields present\n");
    return 0;
}
