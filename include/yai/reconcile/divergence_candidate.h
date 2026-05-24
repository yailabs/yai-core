/*
 * YAI - Divergence candidate
 *
 * Purpose:
 *   Describes mismatch posture produced by independent host observation.
 *
 * Ownership:
 *   Minimal divergence candidate vocabulary and ABI for observation mismatch.
 *
 * Boundary:
 *   A candidate is not a hardened divergence record, repair plan or graph fact.
 *
 * Status:
 *   active
 */

#ifndef YAI_RECONCILE_DIVERGENCE_CANDIDATE_H
#define YAI_RECONCILE_DIVERGENCE_CANDIDATE_H

#include "yai/base/error.h"

typedef enum yai_divergence_candidate_kind {
    YAI_DIVERGENCE_CANDIDATE_EXPECTED_STOPPED_BUT_RUNNING = 0,
    YAI_DIVERGENCE_CANDIDATE_EXPECTED_RUNNING_BUT_NOT_FOUND = 1,
    YAI_DIVERGENCE_CANDIDATE_RECEIPT_CLAIMED_EXECUTED_BUT_NOT_OBSERVED = 2,
    YAI_DIVERGENCE_CANDIDATE_DECISION_DENIED_BUT_EFFECT_OBSERVED = 3,
    YAI_DIVERGENCE_CANDIDATE_RECEIPT_MISSING_FOR_OBSERVED_EFFECT = 4,
    YAI_DIVERGENCE_CANDIDATE_UNKNOWN = 5
} yai_divergence_candidate_kind_t;

typedef enum yai_divergence_candidate_severity {
    YAI_DIVERGENCE_CANDIDATE_INFO = 0,
    YAI_DIVERGENCE_CANDIDATE_WARNING = 1,
    YAI_DIVERGENCE_CANDIDATE_ERROR = 2,
    YAI_DIVERGENCE_CANDIDATE_CRITICAL = 3
} yai_divergence_candidate_severity_t;

typedef struct yai_divergence_candidate {
    char expected_state[32];
    char observed_state[32];
    char source_receipt_ref[64];
    char source_decision_ref[64];
    char observation_ref[64];
    yai_divergence_candidate_kind_t divergence_kind;
    yai_divergence_candidate_severity_t severity;
} yai_divergence_candidate_t;

const char *yai_divergence_candidate_kind_to_string(yai_divergence_candidate_kind_t kind);
yai_divergence_candidate_kind_t yai_divergence_candidate_kind_from_string(const char *value);
const char *yai_divergence_candidate_severity_to_string(yai_divergence_candidate_severity_t severity);
yai_status_t yai_divergence_candidate_init(yai_divergence_candidate_t *candidate,
                                           const char *expected_state,
                                           const char *observed_state,
                                           yai_divergence_candidate_kind_t kind,
                                           yai_divergence_candidate_severity_t severity,
                                           const char *observation_ref);

#endif
