#include "yai/case/case_handle.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_case_handle_init(yai_case_handle_t *handle,
                                  const yai_case_ref_t *case_ref,
                                  const yai_subject_ref_t *subject_ref) {
    if (handle == 0 || yai_case_ref_is_empty(case_ref)) {
        return YAI_ERR_INVALID;
    }
    memset(handle, 0, sizeof(*handle));
    handle->case_ref = *case_ref;
    if (subject_ref != 0) {
        handle->subject_ref = *subject_ref;
    }
    (void)snprintf(handle->case_handle_id,
                   sizeof(handle->case_handle_id),
                   "case-handle:%s",
                   case_ref->case_id.value);
    yai_copy_string(handle->case_status, sizeof(handle->case_status), "resolved");
    yai_copy_string(handle->case_generation, sizeof(handle->case_generation), "1");
    yai_copy_string(handle->policy_generation, sizeof(handle->policy_generation), "partial");
    yai_copy_string(handle->graph_generation, sizeof(handle->graph_generation), "partial");
    yai_copy_string(handle->fact_generation, sizeof(handle->fact_generation), "partial");
    yai_copy_string(handle->memory_generation, sizeof(handle->memory_generation), "not_available");
    yai_copy_string(handle->projection_generation, sizeof(handle->projection_generation), "partial");
    yai_copy_string(handle->generation_status, sizeof(handle->generation_status), "partial");
    yai_copy_string(handle->source, sizeof(handle->source), "runtime_resolved_v0");
    handle->refs_are_authority = 0;
    return YAI_OK;
}
