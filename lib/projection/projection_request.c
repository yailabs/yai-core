#include "yai/projection/projection_request.h"

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_projection_request_init(yai_projection_request_t *request,
                                         const char *request_id,
                                         const yai_case_ref_t *case_ref,
                                         yai_projection_consumer_t consumer_kind,
                                         yai_projection_kind_t projection_kind,
                                         const char *scope,
                                         const char *summary) {
    if (request == 0 || request_id == 0 || request_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref)) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&request->request_id, request_id);
    request->case_ref = *case_ref;
    request->consumer_kind = consumer_kind;
    request->projection_kind = projection_kind;
    yai_copy_string(request->scope, sizeof(request->scope), scope);
    yai_copy_string(request->summary, sizeof(request->summary), summary);
    return YAI_OK;
}
