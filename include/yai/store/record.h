#ifndef YAI_STORE_RECORD_H
#define YAI_STORE_RECORD_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/subject/subject_ref.h"

typedef enum yai_record_kind {
    YAI_RECORD_CASE = 0,
    YAI_RECORD_SUBJECT_BINDING = 1,
    YAI_RECORD_ATTEMPT = 2,
    YAI_RECORD_DECISION = 3,
    YAI_RECORD_RECEIPT = 4,
    YAI_RECORD_PROJECTION = 5
} yai_record_kind_t;

typedef struct yai_store_record {
    yai_id_t record_id;
    yai_case_ref_t case_ref;
    yai_record_kind_t record_kind;
    yai_subject_ref_t subject_ref;
    yai_id_t attempt_id;
    yai_id_t decision_id;
    yai_id_t receipt_id;
    char summary[128];
} yai_store_record_t;

yai_status_t yai_store_record_init(yai_store_record_t *record,
                                   const char *record_id,
                                   const yai_case_ref_t *case_ref,
                                   yai_record_kind_t record_kind,
                                   const yai_subject_ref_t *subject_ref,
                                   const yai_id_t *attempt_id,
                                   const yai_id_t *decision_id,
                                   const yai_id_t *receipt_id,
                                   const char *summary);
const char *yai_record_kind_string(yai_record_kind_t kind);

#endif
