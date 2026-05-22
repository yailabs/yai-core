#include "yai/index/query_result.h"

#include <stdio.h>
#include <string.h>

yai_status_t yai_query_result_init(yai_query_result_t *result,
                                   const char *result_id,
                                   const yai_id_t *query_id,
                                   size_t matched_count,
                                   size_t returned_count,
                                   int truncated) {
    if (result == 0 || result_id == 0 || result_id[0] == '\0' || query_id == 0) {
        return YAI_ERR_INVALID;
    }
    (void)memset(result, 0, sizeof(*result));
    yai_id_set(&result->result_id, result_id);
    result->query_id = *query_id;
    result->matched_count = matched_count;
    result->returned_count = returned_count;
    result->truncated = truncated ? 1 : 0;
    (void)snprintf(result->summary,
                   sizeof(result->summary),
                   "query_result:%s query:%s matched:%zu returned:%zu truncated:%s",
                   result->result_id.value,
                   result->query_id.value,
                   result->matched_count,
                   result->returned_count,
                   result->truncated ? "true" : "false");
    return YAI_OK;
}
