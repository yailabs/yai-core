#ifndef YAI_PROJECTION_PROJECTION_RESULT_H
#define YAI_PROJECTION_PROJECTION_RESULT_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/projection/freshness.h"
#include "yai/projection/projection.h"
#include "yai/projection/projection_kind.h"
#include "yai/projection/projection_request.h"
#include "yai/projection/redaction.h"
#include "yai/store/journal.h"

typedef struct yai_projection_result {
    yai_id_t projection_id;
    yai_id_t request_id;
    yai_case_ref_t case_ref;
    yai_projection_consumer_t consumer_kind;
    yai_projection_kind_t projection_kind;
    size_t source_record_count;
    size_t source_receipt_count;
    size_t source_memory_count;
    size_t source_divergence_count;
    yai_redaction_posture_t redaction_posture;
    yai_projection_freshness_t freshness;
    char summary[256];
} yai_projection_result_t;

yai_redaction_posture_t yai_projection_default_redaction(yai_projection_consumer_t consumer_kind);
yai_status_t yai_projection_result_from_request(const yai_projection_request_t *request,
                                                const yai_journal_t *journal,
                                                yai_projection_result_t *result);

#endif
