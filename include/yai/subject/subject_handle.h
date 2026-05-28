#ifndef YAI_SUBJECT_SUBJECT_HANDLE_H
#define YAI_SUBJECT_SUBJECT_HANDLE_H

#include "yai/base/error.h"
#include "yai/case/case_ref.h"
#include "yai/subject/subject_ref.h"

typedef struct yai_subject_handle {
    char subject_handle_id[128];
    yai_case_ref_t case_ref;
    yai_subject_ref_t subject_ref;
    char subject_role[64];
    char binding_status[32];
    char binding_source_record[128];
    char control_mode[64];
    char projection_mode[64];
    char authority_scope_ref[128];
    char visibility_scope_ref[128];
} yai_subject_handle_t;

yai_status_t yai_subject_handle_init(yai_subject_handle_t *handle,
                                     const yai_case_ref_t *case_ref,
                                     const yai_subject_ref_t *subject_ref,
                                     const char *subject_role);

#endif
