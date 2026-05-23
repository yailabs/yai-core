#include "yai/case/participant_view.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

static int record_matches_case(const yai_store_record_t *record,
                               const yai_case_ref_t *case_ref) {
    return record != 0 && case_ref != 0 &&
           strcmp(record->case_ref.case_id.value, case_ref->case_id.value) == 0;
}

yai_status_t yai_participant_view_frame_build(yai_participant_view_frame_t *frame,
                                              const char *frame_id,
                                              const yai_case_ref_t *case_ref,
                                              const yai_interaction_thread_t *thread,
                                              const yai_projection_t *projection,
                                              const yai_journal_t *journal,
                                              const char *previous_frame_id) {
    size_t index = 0;

    if (frame == 0 || frame_id == 0 || frame_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref) || thread == 0 || projection == 0 || journal == 0) {
        return YAI_ERR_INVALID;
    }

    memset(frame, 0, sizeof(*frame));
    yai_id_set(&frame->frame_id, frame_id);
    frame->case_ref = *case_ref;
    frame->thread_id = thread->thread_id;
    frame->projection_id = projection->projection_id;
    yai_id_set(&frame->previous_frame_id, previous_frame_id == 0 ? "" : previous_frame_id);
    yai_id_set(&frame->delta_since_frame_id, previous_frame_id == 0 ? "" : previous_frame_id);
    frame->included_turn_count = thread->turn_count;
    frame->included_memory_count = projection->memory_candidate_count;

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        if (!record_matches_case(record, case_ref)) {
            continue;
        }
        if (record->record_kind == YAI_RECORD_RECEIPT ||
            record->record_kind == YAI_RECORD_EFFECT_RECEIPT ||
            record->record_kind == YAI_RECORD_FILESYSTEM_RECEIPT) {
            frame->included_receipt_count += 1;
        }
    }

    yai_copy_string(frame->redaction_posture, sizeof(frame->redaction_posture), "summary_only");
    yai_copy_string(frame->freshness, sizeof(frame->freshness), "fresh");
    yai_copy_string(frame->summary,
                    sizeof(frame->summary),
                    "participant_view_frame redaction:summary_only freshness:fresh");
    return YAI_OK;
}
