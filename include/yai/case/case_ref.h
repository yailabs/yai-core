#ifndef YAI_CASE_CASE_REF_H
#define YAI_CASE_CASE_REF_H

#include "yai/base/error.h"
#include "yai/base/id.h"

typedef struct yai_case_ref {
    yai_id_t case_id;
    char case_kind[32];
    char status[32];
} yai_case_ref_t;

yai_status_t yai_case_ref_init(yai_case_ref_t *case_ref,
                               const char *case_id,
                               const char *case_kind,
                               const char *status);
int yai_case_ref_is_empty(const yai_case_ref_t *case_ref);

#endif
