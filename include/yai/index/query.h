#ifndef YAI_INDEX_QUERY_H
#define YAI_INDEX_QUERY_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/index/query_filter.h"
#include "yai/index/query_result.h"
#include "yai/store/journal.h"

typedef struct yai_query_request {
    yai_id_t query_id;
    yai_query_filter_t filter;
} yai_query_request_t;

yai_status_t yai_query_request_init(yai_query_request_t *request,
                                    const char *query_id,
                                    const yai_query_filter_t *filter);
yai_status_t yai_query_scan(const yai_journal_t *journal,
                            const yai_query_request_t *request,
                            const char *result_id,
                            yai_query_result_t *result);

#endif
