#include "yai/subject/subject_binding.h"

#include <stddef.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_subject_binding_init(yai_subject_binding_t *binding,
                                      const char *binding_id,
                                      const yai_case_ref_t *case_ref,
                                      const yai_subject_ref_t *subject_ref,
                                      yai_binding_mode_t binding_mode,
                                      yai_control_mode_t control_mode,
                                      const char *trust_class) {
    if (binding == 0 || binding_id == 0 || binding_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref) || yai_subject_ref_is_empty(subject_ref)) {
        return YAI_ERR_INVALID;
    }
    yai_id_set(&binding->binding_id, binding_id);
    binding->case_ref = *case_ref;
    binding->subject_ref = *subject_ref;
    binding->binding_mode = binding_mode;
    binding->control_mode = control_mode;
    yai_copy_string(binding->trust_class, sizeof(binding->trust_class), trust_class);
    return YAI_OK;
}

const char *yai_binding_mode_string(yai_binding_mode_t mode) {
    switch (mode) {
    case YAI_BINDING_ATTACHED:
        return "attached";
    case YAI_BINDING_OBSERVED:
        return "observed";
    case YAI_BINDING_INTERPOSED:
        return "interposed";
    case YAI_BINDING_CARRIER_OWNED:
        return "carrier_owned";
    case YAI_BINDING_EMBEDDED:
        return "embedded";
    case YAI_BINDING_EXTERNAL:
        return "external";
    default:
        return "unknown";
    }
}

const char *yai_control_mode_string(yai_control_mode_t mode) {
    switch (mode) {
    case YAI_CONTROL_OBSERVED:
        return "observed";
    case YAI_CONTROL_ADVISORY:
        return "advisory";
    case YAI_CONTROL_INTERPOSED:
        return "interposed";
    case YAI_CONTROL_ENFORCED:
        return "enforced";
    case YAI_CONTROL_CARRIER_OWNED:
        return "carrier_owned";
    case YAI_CONTROL_EMBEDDED:
        return "embedded";
    default:
        return "unknown";
    }
}
