#include "yai/reconcile/divergence.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

static int summary_has(const yai_store_record_t *record, const char *needle) {
    return record != 0 && needle != 0 && strstr(record->summary, needle) != 0;
}

static int is_receipt_kind(yai_record_kind_t kind) {
    return kind == YAI_RECORD_RECEIPT ||
           kind == YAI_RECORD_EFFECT_RECEIPT ||
           kind == YAI_RECORD_FILESYSTEM_RECEIPT;
}

const char *yai_divergence_kind_string(yai_divergence_kind_t kind) {
    switch (kind) {
    case YAI_DIVERGENCE_DENIED_BUT_EXECUTED:
        return "denied_but_executed";
    case YAI_DIVERGENCE_RECEIPT_WITHOUT_DECISION:
        return "receipt_without_decision";
    case YAI_DIVERGENCE_CLAIM_WITHOUT_RECEIPT:
        return "claim_without_receipt";
    case YAI_DIVERGENCE_MISSING_TARGET_SUBJECT:
        return "missing_target_subject";
    case YAI_DIVERGENCE_STALE_SUBJECT_STATE:
        return "stale_subject_state";
    case YAI_DIVERGENCE_MEMORY_WITHOUT_BASIS:
        return "memory_without_basis";
    case YAI_DIVERGENCE_GRAPH_CHAIN_MISSING:
        return "graph_chain_missing";
    default:
        return "unknown";
    }
}

yai_status_t yai_divergence_kind_from_string(const char *value,
                                             yai_divergence_kind_t *kind) {
    if (value == 0 || kind == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "denied_but_executed") == 0) {
        *kind = YAI_DIVERGENCE_DENIED_BUT_EXECUTED;
    } else if (strcmp(value, "receipt_without_decision") == 0) {
        *kind = YAI_DIVERGENCE_RECEIPT_WITHOUT_DECISION;
    } else if (strcmp(value, "claim_without_receipt") == 0) {
        *kind = YAI_DIVERGENCE_CLAIM_WITHOUT_RECEIPT;
    } else if (strcmp(value, "missing_target_subject") == 0) {
        *kind = YAI_DIVERGENCE_MISSING_TARGET_SUBJECT;
    } else if (strcmp(value, "stale_subject_state") == 0) {
        *kind = YAI_DIVERGENCE_STALE_SUBJECT_STATE;
    } else if (strcmp(value, "memory_without_basis") == 0) {
        *kind = YAI_DIVERGENCE_MEMORY_WITHOUT_BASIS;
    } else if (strcmp(value, "graph_chain_missing") == 0) {
        *kind = YAI_DIVERGENCE_GRAPH_CHAIN_MISSING;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}

const char *yai_divergence_severity_string(yai_divergence_severity_t severity) {
    switch (severity) {
    case YAI_DIVERGENCE_INFO:
        return "info";
    case YAI_DIVERGENCE_WARNING:
        return "warning";
    case YAI_DIVERGENCE_ERROR:
        return "error";
    case YAI_DIVERGENCE_CRITICAL:
        return "critical";
    default:
        return "unknown";
    }
}

yai_status_t yai_divergence_severity_from_string(const char *value,
                                                 yai_divergence_severity_t *severity) {
    if (value == 0 || severity == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "info") == 0) {
        *severity = YAI_DIVERGENCE_INFO;
    } else if (strcmp(value, "warning") == 0) {
        *severity = YAI_DIVERGENCE_WARNING;
    } else if (strcmp(value, "error") == 0) {
        *severity = YAI_DIVERGENCE_ERROR;
    } else if (strcmp(value, "critical") == 0) {
        *severity = YAI_DIVERGENCE_CRITICAL;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}

yai_status_t yai_divergence_init(yai_divergence_t *divergence,
                                 const char *divergence_id,
                                 const yai_case_ref_t *case_ref,
                                 yai_divergence_kind_t divergence_kind,
                                 yai_divergence_severity_t severity,
                                 const yai_subject_ref_t *subject_ref,
                                 const yai_id_t *attempt_ref,
                                 const yai_id_t *decision_ref,
                                 const yai_id_t *receipt_ref,
                                 const char *summary) {
    if (divergence == 0 || divergence_id == 0 || divergence_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref) || subject_ref == 0 ||
        attempt_ref == 0 || decision_ref == 0 || receipt_ref == 0) {
        return YAI_ERR_INVALID;
    }
    yai_id_set(&divergence->divergence_id, divergence_id);
    divergence->case_ref = *case_ref;
    divergence->divergence_kind = divergence_kind;
    divergence->severity = severity;
    divergence->subject_ref = *subject_ref;
    divergence->attempt_ref = *attempt_ref;
    divergence->decision_ref = *decision_ref;
    divergence->receipt_ref = *receipt_ref;
    yai_copy_string(divergence->summary, sizeof(divergence->summary), summary);
    return YAI_OK;
}

static yai_status_t make_divergence(yai_divergence_t *divergence,
                                    const char *id,
                                    const yai_store_record_t *record,
                                    yai_divergence_kind_t kind,
                                    yai_divergence_severity_t severity) {
    char summary[256];
    int written = snprintf(summary,
                           sizeof(summary),
                           "divergence:%s severity:%s attempt:%.32s decision:%.32s receipt:%.32s",
                           yai_divergence_kind_string(kind),
                           yai_divergence_severity_string(severity),
                           record->attempt_id.value,
                           record->decision_id.value,
                           record->receipt_id.value);
    if (written < 0 || (size_t)written >= sizeof(summary)) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    return yai_divergence_init(divergence,
                               id,
                               &record->case_ref,
                               kind,
                               severity,
                               &record->subject_ref,
                               &record->attempt_id,
                               &record->decision_id,
                               &record->receipt_id,
                               summary);
}

yai_status_t yai_divergence_detect_first(const yai_journal_t *journal,
                                         yai_divergence_t *divergence) {
    size_t index = 0;
    size_t inner = 0;

    if (journal == 0 || divergence == 0) {
        return YAI_ERR_INVALID;
    }

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *receipt = yai_journal_get(journal, index);
        if (receipt == 0 || !is_receipt_kind(receipt->record_kind)) {
            continue;
        }
        if (receipt->decision_id.value[0] == '\0') {
            return make_divergence(divergence,
                                   "divergence:receipt-without-decision",
                                   receipt,
                                   YAI_DIVERGENCE_RECEIPT_WITHOUT_DECISION,
                                   YAI_DIVERGENCE_WARNING);
        }
    }

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *receipt = yai_journal_get(journal, index);
        if (receipt == 0 || !is_receipt_kind(receipt->record_kind)) {
            continue;
        }
        if (!summary_has(receipt, "executed")) {
            continue;
        }
        for (inner = 0; inner < yai_journal_count(journal); inner += 1) {
            const yai_store_record_t *decision = yai_journal_get(journal, inner);
            if (decision == 0 || decision->record_kind != YAI_RECORD_DECISION) {
                continue;
            }
            if (strcmp(decision->decision_id.value, receipt->decision_id.value) == 0 &&
                summary_has(decision, "decision:deny")) {
                return make_divergence(divergence,
                                       "divergence:denied-but-executed",
                                       receipt,
                                       YAI_DIVERGENCE_DENIED_BUT_EXECUTED,
                                       YAI_DIVERGENCE_CRITICAL);
            }
        }
    }

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        if (record == 0) {
            continue;
        }
        if (record->record_kind == YAI_RECORD_ATTEMPT &&
            strcmp(record->subject_ref.subject_id.value, "subject:none") == 0) {
            return make_divergence(divergence,
                                   "divergence:missing-target-subject",
                                   record,
                                   YAI_DIVERGENCE_MISSING_TARGET_SUBJECT,
                                   YAI_DIVERGENCE_ERROR);
        }
        if (record->record_kind == YAI_RECORD_SUBJECT_STATE &&
            summary_has(record, "stale")) {
            return make_divergence(divergence,
                                   "divergence:stale-subject-state",
                                   record,
                                   YAI_DIVERGENCE_STALE_SUBJECT_STATE,
                                   YAI_DIVERGENCE_WARNING);
        }
        if (record->record_kind == YAI_RECORD_MEMORY_CANDIDATE &&
            (summary_has(record, "basis_records:0") ||
             summary_has(record, "basis_receipts:0") ||
             summary_has(record, "basis_edges:0"))) {
            return make_divergence(divergence,
                                   "divergence:memory-without-basis",
                                   record,
                                   YAI_DIVERGENCE_MEMORY_WITHOUT_BASIS,
                                   YAI_DIVERGENCE_WARNING);
        }
    }

    return YAI_ERR_NOT_FOUND;
}
