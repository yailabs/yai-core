#ifndef YAI_OP_ATTEMPT_H
#define YAI_OP_ATTEMPT_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/subject/subject_ref.h"

typedef struct yai_op_attempt {
    yai_id_t attempt_id;
    yai_case_ref_t case_ref;
    yai_subject_ref_t actor_subject_ref;
    yai_subject_ref_t target_subject_ref;
    char effect_class[32];
    char sensitivity[32];
    char locality[32];
    char mutability[32];
    char expected_effect[128];
} yai_op_attempt_t;

yai_status_t yai_op_attempt_init(yai_op_attempt_t *attempt,
                                 const char *attempt_id,
                                 const yai_case_ref_t *case_ref,
                                 const yai_subject_ref_t *actor_subject_ref,
                                 const yai_subject_ref_t *target_subject_ref,
                                 const char *effect_class,
                                 const char *sensitivity,
                                 const char *locality,
                                 const char *mutability,
                                 const char *expected_effect);

#endif
