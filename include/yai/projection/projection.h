#ifndef YAI_PROJECTION_PROJECTION_H
#define YAI_PROJECTION_PROJECTION_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/store/journal.h"

typedef enum yai_projection_consumer {
    YAI_PROJECTION_OPERATOR = 0,
    YAI_PROJECTION_MODEL = 1,
    YAI_PROJECTION_AGENT = 2,
    YAI_PROJECTION_API = 3,
    YAI_PROJECTION_AUDIT = 4,
    YAI_PROJECTION_DEBUG = 5
} yai_projection_consumer_t;

typedef struct yai_projection {
    yai_id_t projection_id;
    yai_case_ref_t case_ref;
    yai_projection_consumer_t consumer_kind;
    char summary[256];
    size_t source_record_count;
    size_t decision_count;
    size_t policy_rule_count;
    size_t gate_count;
    size_t obligation_count;
    size_t receipt_requirement_count;
    size_t filesystem_receipt_count;
    size_t subject_state_count;
    size_t effect_count;
} yai_projection_t;

yai_status_t yai_projection_build(const char *projection_id,
                                  const yai_case_ref_t *case_ref,
                                  yai_projection_consumer_t consumer_kind,
                                  const yai_journal_t *journal,
                                  yai_projection_t *projection);
const char *yai_projection_consumer_string(yai_projection_consumer_t consumer);

#endif
