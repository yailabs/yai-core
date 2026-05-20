#ifndef YAI_INDEX_QUERY_FILTER_H
#define YAI_INDEX_QUERY_FILTER_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/case/case_ref.h"
#include "yai/store/record.h"
#include "yai/subject/subject_ref.h"

typedef struct yai_query_filter {
    yai_case_ref_t case_ref;
    int has_record_kind;
    yai_record_kind_t record_kind;
    yai_subject_ref_t subject_ref;
    yai_id_t decision_id;
    yai_id_t receipt_id;
    yai_id_t memory_id;
    yai_id_t projection_id;
    size_t limit;
    int include_summary;
} yai_query_filter_t;

yai_status_t yai_query_filter_init(yai_query_filter_t *filter);
int yai_query_filter_matches(const yai_query_filter_t *filter,
                             const yai_store_record_t *record);

#endif
