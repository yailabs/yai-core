#ifndef YAI_RECONCILE_DIVERGENCE_H
#define YAI_RECONCILE_DIVERGENCE_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/store/journal.h"
#include "yai/subject/subject_ref.h"

typedef enum yai_divergence_kind {
    YAI_DIVERGENCE_DENIED_BUT_EXECUTED = 0,
    YAI_DIVERGENCE_RECEIPT_WITHOUT_DECISION = 1,
    YAI_DIVERGENCE_CLAIM_WITHOUT_RECEIPT = 2,
    YAI_DIVERGENCE_MISSING_TARGET_SUBJECT = 3,
    YAI_DIVERGENCE_STALE_SUBJECT_STATE = 4,
    YAI_DIVERGENCE_MEMORY_WITHOUT_BASIS = 5,
    YAI_DIVERGENCE_GRAPH_CHAIN_MISSING = 6,
    YAI_DIVERGENCE_DENIED_BUT_ATTEMPTED = 7,
    YAI_DIVERGENCE_BLOCKED_BUT_EFFECT_OBSERVED = 8,
    YAI_DIVERGENCE_EXECUTED_WITHOUT_RECEIPT = 9,
    YAI_DIVERGENCE_RECEIPT_CLAIMED_EXECUTED_BUT_NOT_OBSERVED = 10,
    YAI_DIVERGENCE_FAILED_WITH_PARTIAL_EFFECT = 11,
    YAI_DIVERGENCE_MISSING_RECEIPT = 12,
    YAI_DIVERGENCE_DECISION_WITHOUT_RECEIPT = 13,
    YAI_DIVERGENCE_CARRIER_TIMEOUT = 14,
    YAI_DIVERGENCE_UNSAFE_TARGET_ATTEMPTED = 15,
    YAI_DIVERGENCE_SKELETON_EXECUTED_UNEXPECTEDLY = 16,
    YAI_DIVERGENCE_NONE = 17,
    YAI_DIVERGENCE_UNKNOWN = 18
} yai_divergence_kind_t;

typedef enum yai_divergence_severity {
    YAI_DIVERGENCE_INFO = 0,
    YAI_DIVERGENCE_WARNING = 1,
    YAI_DIVERGENCE_ERROR = 2,
    YAI_DIVERGENCE_CRITICAL = 3,
    YAI_DIVERGENCE_SEVERITY_UNKNOWN = 4
} yai_divergence_severity_t;

typedef struct yai_divergence {
    yai_id_t divergence_id;
    yai_case_ref_t case_ref;
    yai_divergence_kind_t divergence_kind;
    yai_divergence_severity_t severity;
    yai_subject_ref_t subject_ref;
    yai_id_t attempt_ref;
    yai_id_t decision_ref;
    yai_id_t receipt_ref;
    char summary[256];
} yai_divergence_t;

const char *yai_divergence_kind_string(yai_divergence_kind_t kind);
yai_status_t yai_divergence_kind_from_string(const char *value,
                                             yai_divergence_kind_t *kind);
const char *yai_divergence_severity_string(yai_divergence_severity_t severity);
yai_status_t yai_divergence_severity_from_string(const char *value,
                                                 yai_divergence_severity_t *severity);
yai_status_t yai_divergence_init(yai_divergence_t *divergence,
                                 const char *divergence_id,
                                 const yai_case_ref_t *case_ref,
                                 yai_divergence_kind_t divergence_kind,
                                 yai_divergence_severity_t severity,
                                 const yai_subject_ref_t *subject_ref,
                                 const yai_id_t *attempt_ref,
                                 const yai_id_t *decision_ref,
                                 const yai_id_t *receipt_ref,
                                 const char *summary);
yai_status_t yai_divergence_detect_first(const yai_journal_t *journal,
                                         yai_divergence_t *divergence);

#endif
