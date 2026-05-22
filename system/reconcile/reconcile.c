#include "yai/reconcile/reconcile.h"

#include <stdio.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_reconciliation_from_divergence(const char *reconciliation_id,
                                                const yai_divergence_t *divergence,
                                                yai_reconcile_posture_t posture,
                                                yai_reconciliation_t *reconciliation) {
    int written = 0;

    if (reconciliation == 0 || reconciliation_id == 0 ||
        reconciliation_id[0] == '\0' || divergence == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&reconciliation->reconciliation_id, reconciliation_id);
    reconciliation->case_ref = divergence->case_ref;
    reconciliation->divergence_ref = divergence->divergence_id;
    reconciliation->posture = posture;
    reconciliation->subject_ref = divergence->subject_ref;
    written = snprintf(reconciliation->summary,
                       sizeof(reconciliation->summary),
                       "reconcile:%s divergence:%.48s kind:%s severity:%s",
                       yai_reconcile_posture_string(posture),
                       divergence->divergence_id.value,
                       yai_divergence_kind_string(divergence->divergence_kind),
                       yai_divergence_severity_string(divergence->severity));
    if (written < 0 || (size_t)written >= sizeof(reconciliation->summary)) {
        return YAI_ERR_BUFFER_TOO_SMALL;
    }
    return YAI_OK;
}
