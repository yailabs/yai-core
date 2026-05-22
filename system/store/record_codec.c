#include "yai/store/record_codec.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

static yai_status_t json_escape(char *dst, size_t dst_size, const char *src) {
    size_t written = 0;
    const char *cursor = src == 0 ? "" : src;

    if (dst == 0 || dst_size == 0) {
        return YAI_ERR_INVALID;
    }

    while (*cursor != '\0') {
        const char *replacement = 0;
        char literal[2] = {0, 0};
        size_t replacement_len = 0;

        if (*cursor == '\\') {
            replacement = "\\\\";
        } else if (*cursor == '"') {
            replacement = "\\\"";
        } else if (*cursor == '\n') {
            replacement = "\\n";
        } else if (*cursor == '\r') {
            replacement = "\\r";
        } else {
            literal[0] = *cursor;
            replacement = literal;
        }

        replacement_len = strlen(replacement);
        if (written + replacement_len + 1 > dst_size) {
            return YAI_ERR_BUFFER_TOO_SMALL;
        }
        (void)memcpy(dst + written, replacement, replacement_len);
        written += replacement_len;
        cursor += 1;
    }

    dst[written] = '\0';
    return YAI_OK;
}

static yai_status_t json_extract_string(const char *line,
                                        const char *key,
                                        char *dst,
                                        size_t dst_size) {
    char pattern[64];
    const char *cursor = 0;
    size_t written = 0;

    if (line == 0 || key == 0 || dst == 0 || dst_size == 0) {
        return YAI_ERR_INVALID;
    }

    (void)snprintf(pattern, sizeof(pattern), "\"%s\":\"", key);
    cursor = strstr(line, pattern);
    if (cursor == 0) {
        dst[0] = '\0';
        return YAI_OK;
    }
    cursor += strlen(pattern);

    while (*cursor != '\0' && *cursor != '"') {
        char value = *cursor;
        if (*cursor == '\\') {
            cursor += 1;
            if (*cursor == '\0') {
                return YAI_ERR_INVALID;
            }
            if (*cursor == 'n') {
                value = '\n';
            } else if (*cursor == 'r') {
                value = '\r';
            } else {
                value = *cursor;
            }
        }

        if (written + 1 >= dst_size) {
            return YAI_ERR_BUFFER_TOO_SMALL;
        }
        dst[written] = value;
        written += 1;
        cursor += 1;
    }

    dst[written] = '\0';
    return YAI_OK;
}

yai_status_t yai_store_record_encode_jsonl(const yai_store_record_t *record,
                                           char *buffer,
                                           size_t buffer_size) {
    char case_ref[128];
    char case_kind[64];
    char case_status[64];
    char record_id[128];
    char record_kind[64];
    char subject_ref[128];
    char subject_kind[64];
    char subject_locator[256];
    char attempt_id[128];
    char decision_id[128];
    char receipt_id[128];
    char summary[256];
    int result = 0;

    if (record == 0 || buffer == 0 || buffer_size == 0) {
        return YAI_ERR_INVALID;
    }

    if (json_escape(record_id, sizeof(record_id), record->record_id.value) != YAI_OK ||
        json_escape(case_ref, sizeof(case_ref), record->case_ref.case_id.value) != YAI_OK ||
        json_escape(case_kind, sizeof(case_kind), record->case_ref.case_kind) != YAI_OK ||
        json_escape(case_status, sizeof(case_status), record->case_ref.status) != YAI_OK ||
        json_escape(record_kind, sizeof(record_kind), yai_record_kind_string(record->record_kind)) != YAI_OK ||
        json_escape(subject_ref, sizeof(subject_ref), record->subject_ref.subject_id.value) != YAI_OK ||
        json_escape(subject_kind, sizeof(subject_kind), record->subject_ref.subject_kind) != YAI_OK ||
        json_escape(subject_locator, sizeof(subject_locator), record->subject_ref.locator) != YAI_OK ||
        json_escape(attempt_id, sizeof(attempt_id), record->attempt_id.value) != YAI_OK ||
        json_escape(decision_id, sizeof(decision_id), record->decision_id.value) != YAI_OK ||
        json_escape(receipt_id, sizeof(receipt_id), record->receipt_id.value) != YAI_OK ||
        json_escape(summary, sizeof(summary), record->summary) != YAI_OK) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }

    result = snprintf(buffer,
                      buffer_size,
                      "{\"schema\":\"yai.store.record.v0\","
                      "\"record_id\":\"%s\","
                      "\"case_ref\":\"%s\","
                      "\"case_kind\":\"%s\","
                      "\"case_status\":\"%s\","
                      "\"record_kind\":\"%s\","
                      "\"subject_ref\":\"%s\","
                      "\"subject_kind\":\"%s\","
                      "\"subject_locator\":\"%s\","
                      "\"attempt_id\":\"%s\","
                      "\"decision_id\":\"%s\","
                      "\"receipt_id\":\"%s\","
                      "\"summary\":\"%s\"}\n",
                      record_id,
                      case_ref,
                      case_kind,
                      case_status,
                      record_kind,
                      subject_ref,
                      subject_kind,
                      subject_locator,
                      attempt_id,
                      decision_id,
                      receipt_id,
                      summary);
    if (result < 0 || (size_t)result >= buffer_size) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }

    return YAI_OK;
}

yai_status_t yai_store_record_decode_jsonl(const char *line,
                                           yai_store_record_t *record) {
    char schema[64];
    char record_id[64];
    char case_ref[64];
    char case_kind[32];
    char case_status[32];
    char record_kind_value[32];
    char subject_ref_value[64];
    char subject_kind[32];
    char subject_locator[128];
    char attempt_id_value[64];
    char decision_id_value[64];
    char receipt_id_value[64];
    char summary[128];
    yai_case_ref_t case_ref_value;
    yai_subject_ref_t subject_ref;
    yai_id_t attempt_id;
    yai_id_t decision_id;
    yai_id_t receipt_id;
    yai_record_kind_t record_kind;

    if (line == 0 || record == 0) {
        return YAI_ERR_INVALID;
    }

    if (json_extract_string(line, "schema", schema, sizeof(schema)) != YAI_OK ||
        json_extract_string(line, "record_id", record_id, sizeof(record_id)) != YAI_OK ||
        json_extract_string(line, "case_ref", case_ref, sizeof(case_ref)) != YAI_OK ||
        json_extract_string(line, "case_kind", case_kind, sizeof(case_kind)) != YAI_OK ||
        json_extract_string(line, "case_status", case_status, sizeof(case_status)) != YAI_OK ||
        json_extract_string(line, "record_kind", record_kind_value, sizeof(record_kind_value)) != YAI_OK ||
        json_extract_string(line, "subject_ref", subject_ref_value, sizeof(subject_ref_value)) != YAI_OK ||
        json_extract_string(line, "subject_kind", subject_kind, sizeof(subject_kind)) != YAI_OK ||
        json_extract_string(line, "subject_locator", subject_locator, sizeof(subject_locator)) != YAI_OK ||
        json_extract_string(line, "attempt_id", attempt_id_value, sizeof(attempt_id_value)) != YAI_OK ||
        json_extract_string(line, "decision_id", decision_id_value, sizeof(decision_id_value)) != YAI_OK ||
        json_extract_string(line, "receipt_id", receipt_id_value, sizeof(receipt_id_value)) != YAI_OK ||
        json_extract_string(line, "summary", summary, sizeof(summary)) != YAI_OK) {
        return YAI_ERR_INVALID;
    }

    if (strcmp(schema, "yai.store.record.v0") != 0 ||
        record_id[0] == '\0' ||
        case_ref[0] == '\0' ||
        record_kind_value[0] == '\0') {
        return YAI_ERR_INVALID;
    }

    if (subject_ref_value[0] == '\0') {
        yai_copy_string(subject_ref_value, sizeof(subject_ref_value), "subject:none");
    }
    if (subject_kind[0] == '\0') {
        yai_copy_string(subject_kind, sizeof(subject_kind), "external_system");
    }
    if (subject_locator[0] == '\0') {
        yai_copy_string(subject_locator, sizeof(subject_locator), "none");
    }

    if (case_kind[0] == '\0') {
        yai_copy_string(case_kind, sizeof(case_kind), "case");
    }
    if (case_status[0] == '\0') {
        yai_copy_string(case_status, sizeof(case_status), "open");
    }

    if (yai_record_kind_from_string(record_kind_value, &record_kind) != YAI_OK ||
        yai_case_ref_init(&case_ref_value, case_ref, case_kind, case_status) != YAI_OK ||
        yai_subject_ref_init(&subject_ref, subject_ref_value, subject_kind, subject_locator) != YAI_OK) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&attempt_id, attempt_id_value);
    yai_id_set(&decision_id, decision_id_value);
    yai_id_set(&receipt_id, receipt_id_value);

    return yai_store_record_init(record,
                                 record_id,
                                 &case_ref_value,
                                 record_kind,
                                 &subject_ref,
                                 &attempt_id,
                                 &decision_id,
                                 &receipt_id,
                                 summary);
}
