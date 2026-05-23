#include "yai/case/interaction_thread.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

const char *yai_interaction_thread_state_string(yai_interaction_thread_state_t state) {
    switch (state) {
    case YAI_INTERACTION_THREAD_ACTIVE:
        return "active";
    case YAI_INTERACTION_THREAD_ARCHIVED:
        return "archived";
    case YAI_INTERACTION_THREAD_DETACHED:
        return "detached";
    default:
        return "unknown";
    }
}

yai_status_t yai_interaction_thread_init(yai_interaction_thread_t *thread,
                                         const char *thread_id,
                                         const yai_case_ref_t *case_ref,
                                         const char *label) {
    if (thread == 0 || thread_id == 0 || thread_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref)) {
        return YAI_ERR_INVALID;
    }
    memset(thread, 0, sizeof(*thread));
    yai_id_set(&thread->thread_id, thread_id);
    thread->case_ref = *case_ref;
    thread->state = YAI_INTERACTION_THREAD_ACTIVE;
    yai_copy_string(thread->label, sizeof(thread->label), label == 0 ? "default" : label);
    thread->turn_count = 0;
    return YAI_OK;
}

static int record_matches_thread(const yai_store_record_t *record,
                                 const yai_interaction_thread_t *thread) {
    char pattern[96];

    if (record == 0 || thread == 0) {
        return 0;
    }
    if (strcmp(record->case_ref.case_id.value, thread->case_ref.case_id.value) != 0) {
        return 0;
    }
    (void)snprintf(pattern, sizeof(pattern), "thread_id:%s", thread->thread_id.value);
    return strstr(record->summary, pattern) != 0;
}

yai_status_t yai_interaction_thread_materialize(yai_interaction_thread_t *thread,
                                                const yai_journal_t *journal) {
    size_t index = 0;

    if (thread == 0 || journal == 0) {
        return YAI_ERR_INVALID;
    }

    thread->turn_count = 0;
    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        if (!record_matches_thread(record, thread)) {
            continue;
        }
        if (record->record_kind == YAI_RECORD_INTERACTION_THREAD) {
            if (strstr(record->summary, "state:archived") != 0) {
                thread->state = YAI_INTERACTION_THREAD_ARCHIVED;
            } else if (strstr(record->summary, "state:detached") != 0) {
                thread->state = YAI_INTERACTION_THREAD_DETACHED;
            } else if (strstr(record->summary, "state:active") != 0) {
                thread->state = YAI_INTERACTION_THREAD_ACTIVE;
            }
        } else if (record->record_kind == YAI_RECORD_INTERACTION_TURN) {
            thread->turn_count += 1;
        }
    }
    return YAI_OK;
}

int yai_interaction_thread_is_active(const yai_interaction_thread_t *thread) {
    return thread != 0 && thread->state == YAI_INTERACTION_THREAD_ACTIVE;
}
