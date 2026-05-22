#include "yai/index/query.h"

#include <string.h>

yai_status_t yai_query_request_init(yai_query_request_t *request,
                                    const char *query_id,
                                    const yai_query_filter_t *filter) {
    if (request == 0 || query_id == 0 || query_id[0] == '\0' || filter == 0) {
        return YAI_ERR_INVALID;
    }
    (void)memset(request, 0, sizeof(*request));
    yai_id_set(&request->query_id, query_id);
    request->filter = *filter;
    return YAI_OK;
}

yai_status_t yai_query_scan(const yai_journal_t *journal,
                            const yai_query_request_t *request,
                            const char *result_id,
                            yai_query_result_t *result) {
    size_t index = 0;
    size_t matched = 0;
    size_t returned = 0;
    size_t limit = 0;

    if (journal == 0 || request == 0 || result_id == 0 || result == 0) {
        return YAI_ERR_INVALID;
    }

    limit = request->filter.limit;
    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        if (!yai_query_filter_matches(&request->filter, record)) {
            continue;
        }
        matched += 1;
        if (limit == 0 || returned < limit) {
            returned += 1;
        }
    }

    return yai_query_result_init(result,
                                 result_id,
                                 &request->query_id,
                                 matched,
                                 returned,
                                 limit != 0 && matched > returned);
}
