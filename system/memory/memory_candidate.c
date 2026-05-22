#include "yai/memory/memory_candidate.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

static int is_receipt_record(yai_record_kind_t kind) {
    return kind == YAI_RECORD_RECEIPT ||
           kind == YAI_RECORD_EFFECT_RECEIPT ||
           kind == YAI_RECORD_FILESYSTEM_RECEIPT;
}

yai_status_t yai_memory_candidate_init(yai_memory_candidate_t *candidate,
                                       const char *memory_id,
                                       const yai_case_ref_t *case_ref,
                                       yai_memory_kind_t memory_kind,
                                       yai_memory_scope_kind_t scope_kind,
                                       size_t basis_record_count,
                                       size_t basis_receipt_count,
                                       size_t basis_edge_count,
                                       const yai_subject_ref_t *subject_ref,
                                       const yai_id_t *decision_ref,
                                       const yai_id_t *receipt_ref,
                                       yai_memory_freshness_t freshness,
                                       yai_memory_confidence_t confidence,
                                       const char *summary) {
    if (candidate == 0 || memory_id == 0 || memory_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref) || subject_ref == 0 ||
        decision_ref == 0 || receipt_ref == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&candidate->memory_id, memory_id);
    candidate->case_ref = *case_ref;
    candidate->memory_kind = memory_kind;
    candidate->scope_kind = scope_kind;
    candidate->basis_record_count = basis_record_count;
    candidate->basis_receipt_count = basis_receipt_count;
    candidate->basis_edge_count = basis_edge_count;
    candidate->subject_ref = *subject_ref;
    candidate->decision_ref = *decision_ref;
    candidate->receipt_ref = *receipt_ref;
    candidate->freshness = freshness;
    candidate->confidence = confidence;
    yai_copy_string(candidate->summary, sizeof(candidate->summary), summary);
    return YAI_OK;
}

yai_status_t yai_memory_candidate_from_reconstruction(const yai_journal_t *journal,
                                                      const yai_reconstruction_t *reconstruction,
                                                      yai_memory_kind_t memory_kind,
                                                      yai_memory_candidate_t *candidate) {
    size_t index = 0;
    size_t receipt_count = 0;
    yai_case_ref_t case_ref;
    yai_subject_ref_t subject_ref;
    yai_id_t decision_ref;
    yai_id_t receipt_ref;
    char summary[256];
    int found_receipt = 0;
    int written = 0;

    if (journal == 0 || reconstruction == 0 || candidate == 0 ||
        reconstruction->receipt_id.value[0] == '\0') {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&decision_ref, reconstruction->decision_id.value);
    yai_id_set(&receipt_ref, reconstruction->receipt_id.value);

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        if (record == 0) {
            continue;
        }
        if (is_receipt_record(record->record_kind)) {
            receipt_count += 1;
        }
        if (strcmp(record->receipt_id.value, reconstruction->receipt_id.value) == 0) {
            case_ref = record->case_ref;
            subject_ref = record->subject_ref;
            if (record->decision_id.value[0] != '\0') {
                decision_ref = record->decision_id;
            }
            found_receipt = 1;
        }
    }

    if (!found_receipt) {
        return YAI_ERR_NOT_FOUND;
    }

    written = snprintf(summary,
                       sizeof(summary),
                       "memory:%s scope:subject freshness:fresh confidence:high basis_records:%zu basis_receipts:%zu basis_edges:%zu subject:%.40s decision:%.40s receipt:%.40s mutative_write_required_review_blocked",
                       yai_memory_kind_string(memory_kind),
                       yai_journal_count(journal),
                       receipt_count,
                       reconstruction->edge_count,
                       subject_ref.subject_id.value,
                       decision_ref.value,
                       receipt_ref.value);
    if (written < 0 || (size_t)written >= sizeof(summary)) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }

    return yai_memory_candidate_init(candidate,
                                     "memory:new6-candidate",
                                     &case_ref,
                                     memory_kind,
                                     YAI_MEMORY_SCOPE_SUBJECT,
                                     yai_journal_count(journal),
                                     receipt_count,
                                     reconstruction->edge_count,
                                     &subject_ref,
                                     &decision_ref,
                                     &receipt_ref,
                                     YAI_MEMORY_FRESH,
                                     YAI_MEMORY_CONFIDENCE_HIGH,
                                     summary);
}
