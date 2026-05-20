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
    case YAI_RECORD_POLICY_RULE:
        return "policy_rule";
    case YAI_RECORD_GATE_RESULT:
        return "gate_result";
    case YAI_RECORD_DECISION_BASIS:
        return "decision_basis";
    case YAI_RECORD_OBLIGATION:
        return "obligation";
    case YAI_RECORD_RECEIPT_REQUIREMENT:
        return "receipt_requirement";
    case YAI_RECORD_CARRIER_REQUEST:
        return "carrier_request";
    case YAI_RECORD_EFFECT_RECEIPT:
        return "effect_receipt";
    case YAI_RECORD_FILESYSTEM_RECEIPT:
        return "filesystem_receipt";
    case YAI_RECORD_SUBJECT_STATE:
        return "subject_state";
    case YAI_RECORD_GRAPH_EDGE:
        return "graph_edge";
    case YAI_RECORD_RECONSTRUCTION:
        return "reconstruction";
    case YAI_RECORD_MEMORY_CANDIDATE:
        return "memory_candidate";
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
    } else if (strcmp(value, "policy_rule") == 0) {
        *kind = YAI_RECORD_POLICY_RULE;
    } else if (strcmp(value, "gate_result") == 0) {
        *kind = YAI_RECORD_GATE_RESULT;
    } else if (strcmp(value, "decision_basis") == 0) {
        *kind = YAI_RECORD_DECISION_BASIS;
    } else if (strcmp(value, "obligation") == 0) {
        *kind = YAI_RECORD_OBLIGATION;
    } else if (strcmp(value, "receipt_requirement") == 0) {
        *kind = YAI_RECORD_RECEIPT_REQUIREMENT;
    } else if (strcmp(value, "carrier_request") == 0) {
        *kind = YAI_RECORD_CARRIER_REQUEST;
    } else if (strcmp(value, "effect_receipt") == 0) {
        *kind = YAI_RECORD_EFFECT_RECEIPT;
    } else if (strcmp(value, "filesystem_receipt") == 0) {
        *kind = YAI_RECORD_FILESYSTEM_RECEIPT;
    } else if (strcmp(value, "subject_state") == 0) {
        *kind = YAI_RECORD_SUBJECT_STATE;
    } else if (strcmp(value, "graph_edge") == 0) {
        *kind = YAI_RECORD_GRAPH_EDGE;
    } else if (strcmp(value, "reconstruction") == 0) {
        *kind = YAI_RECORD_RECONSTRUCTION;
    } else if (strcmp(value, "memory_candidate") == 0) {
        *kind = YAI_RECORD_MEMORY_CANDIDATE;
    } else {
        return YAI_ERR_INVALID;
    }

    return YAI_OK;
}
