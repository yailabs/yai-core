#include "yai/index/query_filter.h"

#include <stdio.h>
#include <string.h>

static int text_is_empty(const char *value) {
    return value == 0 || value[0] == '\0';
}

static int summary_contains_id(const yai_store_record_t *record,
                               const char *prefix,
                               const yai_id_t *id) {
    char pattern[96];

    if (record == 0 || prefix == 0 || id == 0 || text_is_empty(id->value)) {
        return 1;
    }
    (void)snprintf(pattern, sizeof(pattern), "%s:%s", prefix, id->value);
    return strstr(record->summary, pattern) != 0 || strcmp(record->record_id.value, id->value) == 0;
}

yai_status_t yai_query_filter_init(yai_query_filter_t *filter) {
    if (filter == 0) {
        return YAI_ERR_INVALID;
    }
    (void)memset(filter, 0, sizeof(*filter));
    filter->limit = 0;
    filter->include_summary = 1;
    return YAI_OK;
}

int yai_query_filter_matches(const yai_query_filter_t *filter,
                             const yai_store_record_t *record) {
    if (filter == 0 || record == 0) {
        return 0;
    }
    if (!yai_case_ref_is_empty(&filter->case_ref) &&
        strcmp(filter->case_ref.case_id.value, record->case_ref.case_id.value) != 0) {
        return 0;
    }
    if (filter->has_record_kind && filter->record_kind != record->record_kind) {
        return 0;
    }
    if (!yai_subject_ref_is_empty(&filter->subject_ref) &&
        strcmp(filter->subject_ref.subject_id.value, record->subject_ref.subject_id.value) != 0) {
        return 0;
    }
    if (!text_is_empty(filter->decision_id.value) &&
        strcmp(filter->decision_id.value, record->decision_id.value) != 0) {
        return 0;
    }
    if (!text_is_empty(filter->receipt_id.value) &&
        strcmp(filter->receipt_id.value, record->receipt_id.value) != 0) {
        return 0;
    }
    if (!summary_contains_id(record, "memory", &filter->memory_id)) {
        return 0;
    }
    if (!summary_contains_id(record, "projection", &filter->projection_id)) {
        return 0;
    }
    return 1;
}
