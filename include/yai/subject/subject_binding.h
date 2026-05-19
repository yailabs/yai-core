#ifndef YAI_SUBJECT_SUBJECT_BINDING_H
#define YAI_SUBJECT_SUBJECT_BINDING_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/subject/subject_ref.h"

typedef enum yai_binding_mode {
    YAI_BINDING_ATTACHED = 0,
    YAI_BINDING_OBSERVED = 1,
    YAI_BINDING_INTERPOSED = 2,
    YAI_BINDING_CARRIER_OWNED = 3,
    YAI_BINDING_EMBEDDED = 4,
    YAI_BINDING_EXTERNAL = 5
} yai_binding_mode_t;

typedef enum yai_control_mode {
    YAI_CONTROL_OBSERVED = 0,
    YAI_CONTROL_ADVISORY = 1,
    YAI_CONTROL_INTERPOSED = 2,
    YAI_CONTROL_ENFORCED = 3,
    YAI_CONTROL_CARRIER_OWNED = 4,
    YAI_CONTROL_EMBEDDED = 5
} yai_control_mode_t;

typedef struct yai_subject_binding {
    yai_id_t binding_id;
    yai_case_ref_t case_ref;
    yai_subject_ref_t subject_ref;
    yai_binding_mode_t binding_mode;
    yai_control_mode_t control_mode;
    char trust_class[32];
} yai_subject_binding_t;

yai_status_t yai_subject_binding_init(yai_subject_binding_t *binding,
                                      const char *binding_id,
                                      const yai_case_ref_t *case_ref,
                                      const yai_subject_ref_t *subject_ref,
                                      yai_binding_mode_t binding_mode,
                                      yai_control_mode_t control_mode,
                                      const char *trust_class);

const char *yai_binding_mode_string(yai_binding_mode_t mode);
const char *yai_control_mode_string(yai_control_mode_t mode);

#endif
