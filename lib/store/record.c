#include "yai/store/record.h"

#include <stddef.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_store_record_init(yai_store_record_t *record,
                                   const char *record_id,
                                   const yai_case_ref_t *case_ref,
                                   yai_record_kind_t record_kind,
                                   const yai_subject_ref_t *subject_ref,
                                   const yai_id_t *attempt_id,
                                   const yai_id_t *decision_id,
                                   const yai_id_t *receipt_id,
                                   const char *summary) {
    if (record == 0 || record_id == 0 || record_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref)) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&record->record_id, record_id);
    record->case_ref = *case_ref;
    record->record_kind = record_kind;

    if (subject_ref != 0) {
        record->subject_ref = *subject_ref;
    } else {
        (void)yai_subject_ref_init(&record->subject_ref, "subject:none", "external_system", "none");
    }

    if (attempt_id != 0) {
        record->attempt_id = *attempt_id;
    } else {
        yai_id_set(&record->attempt_id, "");
    }

    if (decision_id != 0) {
        record->decision_id = *decision_id;
    } else {
        yai_id_set(&record->decision_id, "");
    }

    if (receipt_id != 0) {
        record->receipt_id = *receipt_id;
    } else {
        yai_id_set(&record->receipt_id, "");
    }

    yai_copy_string(record->summary, sizeof(record->summary), summary);
    return YAI_OK;
}

const char *yai_record_kind_string(yai_record_kind_t kind) {
    switch (kind) {
    case YAI_RECORD_CASE:
        return "case";
    case YAI_RECORD_SUBJECT_BINDING:
        return "subject_binding";
    case YAI_RECORD_ATTEMPT:
        return "attempt";
    case YAI_RECORD_DECISION:
        return "decision";
    case YAI_RECORD_RECEIPT:
        return "receipt";
    case YAI_RECORD_PROJECTION:
        return "projection";
    default:
        return "unknown";
    }
}

yai_status_t yai_record_kind_from_string(const char *value,
                                         yai_record_kind_t *kind) {
    if (value == 0 || kind == 0) {
        return YAI_ERR_INVALID;
    }

    if (strcmp(value, "case") == 0) {
        *kind = YAI_RECORD_CASE;
    } else if (strcmp(value, "subject_binding") == 0) {
        *kind = YAI_RECORD_SUBJECT_BINDING;
    } else if (strcmp(value, "attempt") == 0) {
        *kind = YAI_RECORD_ATTEMPT;
    } else if (strcmp(value, "decision") == 0) {
        *kind = YAI_RECORD_DECISION;
    } else if (strcmp(value, "receipt") == 0) {
        *kind = YAI_RECORD_RECEIPT;
    } else if (strcmp(value, "projection") == 0) {
        *kind = YAI_RECORD_PROJECTION;
    } else {
        return YAI_ERR_INVALID;
    }

    return YAI_OK;
}
