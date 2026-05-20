#ifndef YAI_MEMORY_MEMORY_CANDIDATE_H
#define YAI_MEMORY_MEMORY_CANDIDATE_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/graph/reconstruct.h"
#include "yai/memory/memory_kind.h"
#include "yai/memory/memory_scope.h"
#include "yai/store/journal.h"
#include "yai/subject/subject_ref.h"

typedef struct yai_memory_candidate {
    yai_id_t memory_id;
    yai_case_ref_t case_ref;
    yai_memory_kind_t memory_kind;
    yai_memory_scope_kind_t scope_kind;
    size_t basis_record_count;
    size_t basis_receipt_count;
    size_t basis_edge_count;
    yai_subject_ref_t subject_ref;
    yai_id_t decision_ref;
    yai_id_t receipt_ref;
    yai_memory_freshness_t freshness;
    yai_memory_confidence_t confidence;
    char summary[256];
} yai_memory_candidate_t;

yai_status_t yai_memory_candidate_init(yai_memory_candidate_t *candidate,
                                       const char *memory_id,
                                       const yai_case_ref_t *case_ref,
                                       yai_memory_kind_t memory_kind,
                                       yai_memory_scope_kind_t scope_kind,
                                       size_t basis_record_count,
                                       size_t basis_receipt_count,
                                       size_t basis_edge_count,
                                       const yai_subject_ref_t *subject_ref,
                                       const yai_id_t *decision_ref,
                                       const yai_id_t *receipt_ref,
                                       yai_memory_freshness_t freshness,
                                       yai_memory_confidence_t confidence,
                                       const char *summary);

yai_status_t yai_memory_candidate_from_reconstruction(const yai_journal_t *journal,
                                                      const yai_reconstruction_t *reconstruction,
                                                      yai_memory_kind_t memory_kind,
                                                      yai_memory_candidate_t *candidate);

#endif
