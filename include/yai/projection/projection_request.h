#ifndef YAI_PROJECTION_PROJECTION_REQUEST_H
#define YAI_PROJECTION_PROJECTION_REQUEST_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"
#include "yai/projection/projection.h"
#include "yai/projection/projection_kind.h"

typedef struct yai_projection_request {
    yai_id_t request_id;
    yai_case_ref_t case_ref;
    yai_projection_consumer_t consumer_kind;
    yai_projection_kind_t projection_kind;
    char scope[64];
    char summary[128];
} yai_projection_request_t;

yai_status_t yai_projection_request_init(yai_projection_request_t *request,
                                         const char *request_id,
                                         const yai_case_ref_t *case_ref,
                                         yai_projection_consumer_t consumer_kind,
                                         yai_projection_kind_t projection_kind,
                                         const char *scope,
                                         const char *summary);

#endif
