#include "yai/projection/projection.h"

#include <stdio.h>

yai_status_t yai_projection_build(const char *projection_id,
                                  const yai_case_ref_t *case_ref,
                                  yai_projection_consumer_t consumer_kind,
                                  const yai_journal_t *journal,
                                  yai_projection_t *projection) {
    if (projection == 0 || projection_id == 0 || projection_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref) || journal == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&projection->projection_id, projection_id);
    projection->case_ref = *case_ref;
    projection->consumer_kind = consumer_kind;
    projection->source_record_count = yai_journal_count(journal);
    (void)snprintf(projection->summary,
                   sizeof(projection->summary),
                   "projection:%s records:%zu",
                   yai_projection_consumer_string(consumer_kind),
                   projection->source_record_count);
    return YAI_OK;
}

const char *yai_projection_consumer_string(yai_projection_consumer_t consumer) {
    switch (consumer) {
    case YAI_PROJECTION_OPERATOR:
        return "operator";
    case YAI_PROJECTION_MODEL:
        return "model";
    case YAI_PROJECTION_AGENT:
        return "agent";
    case YAI_PROJECTION_API:
        return "api";
    case YAI_PROJECTION_AUDIT:
        return "audit";
    case YAI_PROJECTION_DEBUG:
        return "debug";
    default:
        return "unknown";
    }
}
