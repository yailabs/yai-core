#ifndef YAI_CASE_INTERACTION_THREAD_H
#define YAI_CASE_INTERACTION_THREAD_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/store/journal.h"

typedef enum yai_interaction_thread_state {
    YAI_INTERACTION_THREAD_ACTIVE = 0,
    YAI_INTERACTION_THREAD_ARCHIVED = 1,
    YAI_INTERACTION_THREAD_DETACHED = 2
} yai_interaction_thread_state_t;

typedef struct yai_interaction_thread {
    yai_id_t thread_id;
    yai_case_ref_t case_ref;
    yai_interaction_thread_state_t state;
    char label[64];
    size_t turn_count;
} yai_interaction_thread_t;

const char *yai_interaction_thread_state_string(yai_interaction_thread_state_t state);

yai_status_t yai_interaction_thread_init(yai_interaction_thread_t *thread,
                                         const char *thread_id,
                                         const yai_case_ref_t *case_ref,
                                         const char *label);

yai_status_t yai_interaction_thread_materialize(yai_interaction_thread_t *thread,
                                                const yai_journal_t *journal);

int yai_interaction_thread_is_active(const yai_interaction_thread_t *thread);

#endif
