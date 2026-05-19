#ifndef YAI_SUBJECT_SUBJECT_REF_H
#define YAI_SUBJECT_SUBJECT_REF_H

#include "yai/base/error.h"
#include "yai/base/id.h"

typedef struct yai_subject_ref {
    yai_id_t subject_id;
    char subject_kind[32];
    char locator[128];
} yai_subject_ref_t;

yai_status_t yai_subject_ref_init(yai_subject_ref_t *subject_ref,
                                  const char *subject_id,
                                  const char *subject_kind,
                                  const char *locator);
int yai_subject_ref_is_empty(const yai_subject_ref_t *subject_ref);

#endif
