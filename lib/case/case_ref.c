#include "yai/case/case_ref.h"

#include <stddef.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_case_ref_init(yai_case_ref_t *case_ref,
                               const char *case_id,
                               const char *case_kind,
                               const char *status) {
    if (case_ref == 0 || case_id == 0 || case_id[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_id_set(&case_ref->case_id, case_id);
    yai_copy_string(case_ref->case_kind, sizeof(case_ref->case_kind), case_kind);
    yai_copy_string(case_ref->status, sizeof(case_ref->status), status);
    return YAI_OK;
}

int yai_case_ref_is_empty(const yai_case_ref_t *case_ref) {
    return case_ref == 0 || yai_id_is_empty(&case_ref->case_id);
}
