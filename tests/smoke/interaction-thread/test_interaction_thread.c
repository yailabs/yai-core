#include "yai/yai.h"

#include <assert.h>
#include <stdio.h>

static void require_ok(yai_status_t status) {
    assert(status == YAI_OK);
}

static void append_record(yai_journal_t *journal,
                          const char *record_id,
                          const yai_case_ref_t *case_ref,
                          yai_record_kind_t kind,
                          const char *summary) {
    yai_store_record_t record;
    require_ok(yai_store_record_init(&record,
                                     record_id,
                                     case_ref,
                                     kind,
                                     0,
                                     0,
                                     0,
                                     0,
                                     summary));
    require_ok(yai_journal_append(journal, &record));
}

int main(void) {
    yai_case_ref_t case_ref;
    yai_journal_t journal;
    yai_interaction_thread_t default_thread;
    yai_interaction_thread_t clean_thread;
    yai_projection_t projection;
    yai_participant_view_frame_t frame;

    require_ok(yai_case_ref_init(&case_ref, "case:new18c-thread", "operational_case", "open"));
    require_ok(yai_journal_init(&journal));

    append_record(&journal, "case-domain:fs", &case_ref, YAI_RECORD_CASE_DOMAIN, "case_domain:filesystem_sandbox");
    append_record(&journal, "case-attachment:fs", &case_ref, YAI_RECORD_CASE_ATTACHMENT, "case_attachment:filesystem_sandbox");
    append_record(&journal, "case-binding:fs", &case_ref, YAI_RECORD_CASE_BINDING, "case_binding:filesystem_sandbox");
    append_record(&journal, "thread:default", &case_ref, YAI_RECORD_INTERACTION_THREAD, "interaction_thread:default thread_id:thread:default state:active label:default");

    require_ok(yai_interaction_thread_init(&default_thread, "thread:default", &case_ref, "default"));
    require_ok(yai_interaction_thread_materialize(&default_thread, &journal));
    assert(yai_interaction_thread_is_active(&default_thread));
    printf("interaction_thread:default active\n");

    append_record(&journal, "turn:1", &case_ref, YAI_RECORD_INTERACTION_TURN, "interaction_turn:1 thread_id:thread:default role:user_model transcript:preview");
    require_ok(yai_interaction_thread_materialize(&default_thread, &journal));
    assert(default_thread.turn_count == 1);
    printf("interaction_turn:appended\n");

    require_ok(yai_projection_build("projection:new18c", &case_ref, YAI_PROJECTION_MODEL, &journal, &projection));
    require_ok(yai_participant_view_frame_build(&frame, "frame:1", &case_ref, &default_thread, &projection, &journal, ""));
    assert(frame.included_turn_count == 1);
    printf("participant_view:frame built\n");

    append_record(&journal, "thread:clean", &case_ref, YAI_RECORD_INTERACTION_THREAD, "interaction_thread:clean thread_id:thread:clean state:active label:clean");
    require_ok(yai_interaction_thread_init(&clean_thread, "thread:clean", &case_ref, "clean"));
    require_ok(yai_interaction_thread_materialize(&clean_thread, &journal));
    assert(clean_thread.turn_count == 0);
    printf("interaction_thread:new active\n");

    require_ok(yai_participant_view_frame_build(&frame, "frame:2", &case_ref, &clean_thread, &projection, &journal, "frame:1"));
    assert(frame.included_turn_count == 0);
    printf("participant_view:empty thread\n");

    require_ok(yai_interaction_thread_materialize(&default_thread, &journal));
    assert(default_thread.turn_count == 1);
    printf("interaction_thread:restored previous\n");
    printf("journal:audit retained\n");

    yai_journal_free(&journal);
    return 0;
}
