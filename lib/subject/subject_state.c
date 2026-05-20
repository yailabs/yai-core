#include "yai/subject/subject_state.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

static void yai_subject_state_copy_summary_hash(yai_subject_state_t *state,
                                                const char *summary) {
    const char *cursor = 0;
    size_t index = 0;

    if (state == 0 || summary == 0) {
        return;
    }

    cursor = strstr(summary, "after_hash:");
    if (cursor == 0) {
        cursor = strstr(summary, "hash:");
    }
    if (cursor == 0) {
        return;
    }
    cursor = strchr(cursor, ':');
    if (cursor == 0) {
        return;
    }
    cursor += 1;
    while (cursor[index] != '\0' &&
           cursor[index] != ' ' &&
           index + 1 < sizeof(state->content_or_status_hash)) {
        state->content_or_status_hash[index] = cursor[index];
        index += 1;
    }
    state->content_or_status_hash[index] = '\0';
}

yai_status_t yai_subject_state_init(yai_subject_state_t *state,
                                    const yai_case_ref_t *case_ref,
                                    const yai_subject_ref_t *subject_ref) {
    if (state == 0 || yai_case_ref_is_empty(case_ref) || yai_subject_ref_is_empty(subject_ref)) {
        return YAI_ERR_INVALID;
    }
    state->case_ref = *case_ref;
    state->subject_ref = *subject_ref;
    state->state_kind = YAI_SUBJECT_STATE_UNKNOWN;
    yai_id_set(&state->last_receipt_id, "");
    yai_id_set(&state->last_decision_id, "");
    yai_copy_string(state->last_observed_at, sizeof(state->last_observed_at), "");
    yai_copy_string(state->content_or_status_hash, sizeof(state->content_or_status_hash), "");
    yai_copy_string(state->summary, sizeof(state->summary), "subject state unknown");
    return YAI_OK;
}

yai_status_t yai_subject_state_apply_record(yai_subject_state_t *state,
                                            const yai_store_record_t *record) {
    if (state == 0 || record == 0) {
        return YAI_ERR_INVALID;
    }

    if (!yai_id_equal(&state->case_ref.case_id, &record->case_ref.case_id) ||
        !yai_id_equal(&state->subject_ref.subject_id, &record->subject_ref.subject_id)) {
        return YAI_OK;
    }

    if (record->record_kind == YAI_RECORD_SUBJECT_BINDING) {
        state->state_kind = YAI_SUBJECT_STATE_BOUND;
        (void)snprintf(state->summary,
                       sizeof(state->summary),
                       "%s bound",
                       state->subject_ref.subject_id.value);
    } else if (record->record_kind == YAI_RECORD_RECEIPT ||
               record->record_kind == YAI_RECORD_EFFECT_RECEIPT ||
               record->record_kind == YAI_RECORD_FILESYSTEM_RECEIPT) {
        state->state_kind = YAI_SUBJECT_STATE_OBSERVED;
        state->last_receipt_id = record->receipt_id;
        state->last_decision_id = record->decision_id;
        yai_copy_string(state->last_observed_at,
                        sizeof(state->last_observed_at),
                        record->record_id.value);
        yai_subject_state_copy_summary_hash(state, record->summary);
        yai_copy_string(state->summary, sizeof(state->summary), record->summary);
    }

    return YAI_OK;
}

yai_status_t yai_subject_state_from_journal(const yai_case_ref_t *case_ref,
                                            const yai_subject_ref_t *subject_ref,
                                            const yai_journal_t *journal,
                                            yai_subject_state_t *state) {
    size_t index = 0;
    yai_status_t status = YAI_OK;

    if (journal == 0) {
        return YAI_ERR_INVALID;
    }

    status = yai_subject_state_init(state, case_ref, subject_ref);
    if (status != YAI_OK) {
        return status;
    }

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        status = yai_subject_state_apply_record(state, record);
        if (status != YAI_OK) {
            return status;
        }
    }

    return YAI_OK;
}

const char *yai_subject_state_kind_string(yai_subject_state_kind_t kind) {
    switch (kind) {
    case YAI_SUBJECT_STATE_BOUND:
        return "bound";
    case YAI_SUBJECT_STATE_OBSERVED:
        return "observed";
    case YAI_SUBJECT_STATE_CONTROLLED:
        return "controlled";
    case YAI_SUBJECT_STATE_LOST:
        return "lost";
    case YAI_SUBJECT_STATE_CONFLICTED:
        return "conflicted";
    case YAI_SUBJECT_STATE_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}
