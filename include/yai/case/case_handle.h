#ifndef YAI_CASE_CASE_HANDLE_H
#define YAI_CASE_CASE_HANDLE_H

#include <stdint.h>

#include "yai/base/error.h"
#include "yai/case/case_ref.h"
#include "yai/subject/subject_ref.h"

typedef struct yai_case_handle {
    char case_handle_id[128];
    yai_case_ref_t case_ref;
    yai_subject_ref_t subject_ref;
    char case_status[32];
    char case_generation[32];
    char policy_generation[32];
    char graph_generation[32];
    char fact_generation[32];
    char memory_generation[32];
    char projection_generation[32];
    char generation_status[32];
    uint64_t opened_at_unix_ms;
    char source[64];
    int refs_are_authority;
} yai_case_handle_t;

yai_status_t yai_case_handle_init(yai_case_handle_t *handle,
                                  const yai_case_ref_t *case_ref,
                                  const yai_subject_ref_t *subject_ref);

#endif
