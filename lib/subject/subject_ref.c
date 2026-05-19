#include "yai/subject/subject_ref.h"

#include <stddef.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_subject_ref_init(yai_subject_ref_t *subject_ref,
                                  const char *subject_id,
                                  const char *subject_kind,
                                  const char *locator) {
    if (subject_ref == 0 || subject_id == 0 || subject_id[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_id_set(&subject_ref->subject_id, subject_id);
    yai_copy_string(subject_ref->subject_kind, sizeof(subject_ref->subject_kind), subject_kind);
    yai_copy_string(subject_ref->locator, sizeof(subject_ref->locator), locator);
    return YAI_OK;
}

int yai_subject_ref_is_empty(const yai_subject_ref_t *subject_ref) {
    return subject_ref == 0 || yai_id_is_empty(&subject_ref->subject_id);
}
