#include "yai/projection/projection_result.h"

#include <stdio.h>

static int is_receipt_kind(yai_record_kind_t kind) {
    return kind == YAI_RECORD_RECEIPT ||
           kind == YAI_RECORD_EFFECT_RECEIPT ||
           kind == YAI_RECORD_FILESYSTEM_RECEIPT;
}

yai_redaction_posture_t yai_projection_default_redaction(yai_projection_consumer_t consumer_kind) {
    switch (consumer_kind) {
    case YAI_PROJECTION_MODEL:
    case YAI_PROJECTION_AGENT:
        return YAI_REDACTION_SUMMARY_ONLY;
    case YAI_PROJECTION_DEBUG:
        return YAI_REDACTION_REFS_ONLY;
    default:
        return YAI_REDACTION_NONE;
    }
}

yai_status_t yai_projection_result_from_request(const yai_projection_request_t *request,
                                                const yai_journal_t *journal,
                                                yai_projection_result_t *result) {
    size_t index = 0;
    int written = 0;

    if (request == 0 || journal == 0 || result == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&result->projection_id, "projection:result");
    result->request_id = request->request_id;
    result->case_ref = request->case_ref;
    result->consumer_kind = request->consumer_kind;
    result->projection_kind = request->projection_kind;
    result->source_record_count = yai_journal_count(journal);
    result->source_receipt_count = 0;
    result->source_memory_count = 0;
    result->source_divergence_count = 0;
    result->redaction_posture = yai_projection_default_redaction(request->consumer_kind);
    result->freshness = YAI_PROJECTION_FRESH;

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        if (record == 0) {
            continue;
        }
        if (is_receipt_kind(record->record_kind)) {
            result->source_receipt_count += 1;
        } else if (record->record_kind == YAI_RECORD_MEMORY_CANDIDATE) {
            result->source_memory_count += 1;
        } else if (record->record_kind == YAI_RECORD_DIVERGENCE) {
            result->source_divergence_count += 1;
        }
    }

    written = snprintf(result->summary,
                       sizeof(result->summary),
                       "projection_result:%s consumer:%s kind:%s redaction:%s freshness:%s source_records:%zu source_receipts:%zu source_memory:%zu source_divergences:%zu",
                       result->projection_id.value,
                       yai_projection_consumer_string(result->consumer_kind),
                       yai_projection_kind_string(result->projection_kind),
                       yai_redaction_posture_string(result->redaction_posture),
                       yai_projection_freshness_string(result->freshness),
                       result->source_record_count,
                       result->source_receipt_count,
                       result->source_memory_count,
                       result->source_divergence_count);
    if (written < 0 || (size_t)written >= sizeof(result->summary)) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    return YAI_OK;
}
