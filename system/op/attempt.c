#include "yai/op/attempt.h"

#include <stddef.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_op_attempt_init(yai_op_attempt_t *attempt,
                                 const char *attempt_id,
                                 const yai_case_ref_t *case_ref,
                                 const yai_subject_ref_t *actor_subject_ref,
                                 const yai_subject_ref_t *target_subject_ref,
                                 const char *effect_class,
                                 const char *sensitivity,
                                 const char *locality,
                                 const char *mutability,
                                 const char *expected_effect) {
    if (attempt == 0 || attempt_id == 0 || attempt_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref) || yai_subject_ref_is_empty(target_subject_ref)) {
        return YAI_ERR_INVALID;
    }
    yai_id_set(&attempt->attempt_id, attempt_id);
    attempt->case_ref = *case_ref;
    if (actor_subject_ref != 0) {
        attempt->actor_subject_ref = *actor_subject_ref;
    } else {
        (void)yai_subject_ref_init(&attempt->actor_subject_ref, "subject:unknown-actor", "external_system", "unknown");
    }
    attempt->target_subject_ref = *target_subject_ref;
    yai_copy_string(attempt->effect_class, sizeof(attempt->effect_class), effect_class);
    yai_copy_string(attempt->sensitivity, sizeof(attempt->sensitivity), sensitivity);
    yai_copy_string(attempt->locality, sizeof(attempt->locality), locality);
    yai_copy_string(attempt->mutability, sizeof(attempt->mutability), mutability);
    yai_copy_string(attempt->expected_effect, sizeof(attempt->expected_effect), expected_effect);
    return YAI_OK;
}
