#ifndef YAI_INDEX_QUERY_RESULT_H
#define YAI_INDEX_QUERY_RESULT_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/base/id.h"

typedef struct yai_query_result {
    yai_id_t result_id;
    yai_id_t query_id;
    size_t matched_count;
    size_t returned_count;
    int truncated;
    char summary[256];
} yai_query_result_t;

yai_status_t yai_query_result_init(yai_query_result_t *result,
                                   const char *result_id,
                                   const yai_id_t *query_id,
                                   size_t matched_count,
                                   size_t returned_count,
                                   int truncated);

#endif
