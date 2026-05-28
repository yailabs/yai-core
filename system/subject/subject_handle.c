#include "yai/subject/subject_handle.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_subject_handle_init(yai_subject_handle_t *handle,
                                     const yai_case_ref_t *case_ref,
                                     const yai_subject_ref_t *subject_ref,
                                     const char *subject_role) {
    if (handle == 0 || yai_case_ref_is_empty(case_ref) || yai_subject_ref_is_empty(subject_ref)) {
        return YAI_ERR_INVALID;
    }
    memset(handle, 0, sizeof(*handle));
    handle->case_ref = *case_ref;
    handle->subject_ref = *subject_ref;
    (void)snprintf(handle->subject_handle_id,
                   sizeof(handle->subject_handle_id),
                   "subject-handle:%.48s:%.48s",
                   case_ref->case_id.value,
                   subject_ref->subject_id.value);
    yai_copy_string(handle->subject_role, sizeof(handle->subject_role), subject_role);
    yai_copy_string(handle->binding_status, sizeof(handle->binding_status), "resolved");
    yai_copy_string(handle->binding_source_record, sizeof(handle->binding_source_record), "derived_v0");
    yai_copy_string(handle->control_mode, sizeof(handle->control_mode), "scoped");
    yai_copy_string(handle->projection_mode, sizeof(handle->projection_mode), "scoped_summary");
    yai_copy_string(handle->authority_scope_ref, sizeof(handle->authority_scope_ref), "authority-scope:derived-v0");
    yai_copy_string(handle->visibility_scope_ref, sizeof(handle->visibility_scope_ref), "visibility-scope:derived-v0");
    return YAI_OK;
}
