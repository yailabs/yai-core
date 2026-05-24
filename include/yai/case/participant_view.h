#ifndef YAI_CASE_PARTICIPANT_VIEW_H
#define YAI_CASE_PARTICIPANT_VIEW_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/case/interaction_thread.h"
#include "yai/projection/projection.h"
#include "yai/store/journal.h"

typedef struct yai_participant_view_frame {
    yai_id_t frame_id;
    yai_case_ref_t case_ref;
    yai_id_t thread_id;
    yai_id_t projection_id;
    yai_id_t previous_frame_id;
    yai_id_t delta_since_frame_id;
    size_t included_turn_count;
    size_t included_memory_count;
    size_t included_receipt_count;
    char redaction_posture[32];
    char freshness[32];
    char freshness_source[32];
    char stale_reason[64];
    char freshness_policy[32];
    char summary[256];
} yai_participant_view_frame_t;

yai_status_t yai_participant_view_frame_build(yai_participant_view_frame_t *frame,
                                              const char *frame_id,
                                              const yai_case_ref_t *case_ref,
                                              const yai_interaction_thread_t *thread,
                                              const yai_projection_t *projection,
                                              const yai_journal_t *journal,
                                              const char *previous_frame_id);

#endif
