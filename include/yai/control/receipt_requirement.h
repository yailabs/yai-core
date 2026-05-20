#ifndef YAI_CONTROL_RECEIPT_REQUIREMENT_H
#define YAI_CONTROL_RECEIPT_REQUIREMENT_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/control/decision.h"

typedef enum yai_receipt_requirement_kind {
    YAI_RECEIPT_REQUIREMENT_CARRIER_RECEIPT = 0,
    YAI_RECEIPT_REQUIREMENT_EXTERNAL_RECEIPT = 1,
    YAI_RECEIPT_REQUIREMENT_BLOCKED_RECEIPT = 2,
    YAI_RECEIPT_REQUIREMENT_POST_STATE_OBSERVATION = 3,
    YAI_RECEIPT_REQUIREMENT_HASH_DELTA = 4,
    YAI_RECEIPT_REQUIREMENT_EXIT_STATUS = 5,
    YAI_RECEIPT_REQUIREMENT_MODEL_INVOCATION_METADATA = 6,
    YAI_RECEIPT_REQUIREMENT_OPERATOR_ACK = 7
} yai_receipt_requirement_kind_t;

typedef struct yai_receipt_requirement {
    yai_id_t requirement_id;
    yai_case_ref_t case_ref;
    yai_id_t decision_id;
    yai_receipt_requirement_kind_t requirement_kind;
    int required;
    char summary[128];
} yai_receipt_requirement_t;

yai_status_t yai_receipt_requirement_from_decision(const char *requirement_id,
                                                   const yai_control_decision_t *decision,
                                                   yai_receipt_requirement_t *requirement);
const char *yai_receipt_requirement_kind_string(yai_receipt_requirement_kind_t kind);

#endif
