#ifndef YAI_RECONCILE_RECONCILE_H
#define YAI_RECONCILE_RECONCILE_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/reconcile/divergence.h"
#include "yai/reconcile/recovery.h"
#include "yai/subject/subject_ref.h"

typedef struct yai_reconciliation {
    yai_id_t reconciliation_id;
    yai_case_ref_t case_ref;
    yai_id_t divergence_ref;
    yai_reconcile_posture_t posture;
    yai_subject_ref_t subject_ref;
    char summary[256];
} yai_reconciliation_t;

yai_status_t yai_reconciliation_from_divergence(const char *reconciliation_id,
                                                const yai_divergence_t *divergence,
                                                yai_reconcile_posture_t posture,
                                                yai_reconciliation_t *reconciliation);

#endif
