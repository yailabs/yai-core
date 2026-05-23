#ifndef YAI_CASE_CASE_WORLD_H
#define YAI_CASE_CASE_WORLD_H

#include "yai/base/error.h"
#include "yai/case/case_ref.h"
#include "yai/store/record.h"
#include "yai/subject/subject_ref.h"

#define YAI_CASE_SCOPE_MAX 64
#define YAI_CASE_REF_MAX 128
#define YAI_CASE_SEMANTIC_MAX 96

typedef enum yai_case_domain_kind {
    YAI_CASE_DOMAIN_ROOT = 0,
    YAI_CASE_DOMAIN_USER = 1,
    YAI_CASE_DOMAIN_NESTED = 2,
    YAI_CASE_DOMAIN_OPERATIONAL = 3
} yai_case_domain_kind_t;

typedef enum yai_case_attachment_kind {
    YAI_CASE_ATTACHMENT_FILE = 0,
    YAI_CASE_ATTACHMENT_DIRECTORY = 1,
    YAI_CASE_ATTACHMENT_ARTIFACT = 2,
    YAI_CASE_ATTACHMENT_REPOSITORY = 3,
    YAI_CASE_ATTACHMENT_PROCESS = 4,
    YAI_CASE_ATTACHMENT_ENDPOINT = 5,
    YAI_CASE_ATTACHMENT_DATABASE = 6,
    YAI_CASE_ATTACHMENT_SERVICE = 7,
    YAI_CASE_ATTACHMENT_SOCKET = 8,
    YAI_CASE_ATTACHMENT_DATASET = 9,
    YAI_CASE_ATTACHMENT_MODEL = 10,
    YAI_CASE_ATTACHMENT_POLICY = 11,
    YAI_CASE_ATTACHMENT_EVIDENCE = 12,
    YAI_CASE_ATTACHMENT_RUNTIME_SUBJECT = 13,
    YAI_CASE_ATTACHMENT_SOURCE = 14,
    YAI_CASE_ATTACHMENT_UNKNOWN = 15
} yai_case_attachment_kind_t;

typedef enum yai_case_binding_kind {
    YAI_CASE_BINDING_ACTOR = 0,
    YAI_CASE_BINDING_WORKFLOW = 1,
    YAI_CASE_BINDING_CAPABILITY = 2,
    YAI_CASE_BINDING_DECISION = 3,
    YAI_CASE_BINDING_RUNTIME = 4,
    YAI_CASE_BINDING_MODEL = 5,
    YAI_CASE_BINDING_SUBSTRATE_RECORD = 6,
    YAI_CASE_BINDING_SUBSTRATE_SOURCE = 7,
    YAI_CASE_BINDING_SUBSTRATE_GRAPH = 8,
    YAI_CASE_BINDING_SUBSTRATE_MEMORY = 9,
    YAI_CASE_BINDING_POLICY = 10,
    YAI_CASE_BINDING_EVIDENCE = 11,
    YAI_CASE_BINDING_SUBJECT = 12,
    YAI_CASE_BINDING_ATTACHMENT = 13,
    YAI_CASE_BINDING_RECEIPT = 14,
    YAI_CASE_BINDING_PRINCIPAL = 15,
    YAI_CASE_BINDING_EVENT = 16,
    YAI_CASE_BINDING_ATTEMPT = 17,
    YAI_CASE_BINDING_EFFECT = 18
} yai_case_binding_kind_t;

typedef enum yai_case_posture {
    YAI_CASE_POSTURE_CANDIDATE = 0,
    YAI_CASE_POSTURE_BOUND = 1,
    YAI_CASE_POSTURE_STALE = 2,
    YAI_CASE_POSTURE_CHALLENGED = 3,
    YAI_CASE_POSTURE_RETIRED = 4
} yai_case_posture_t;

typedef struct yai_case_domain {
    yai_case_ref_t case_ref;
    yai_case_domain_kind_t domain_kind;
    char scope_id[YAI_CASE_SCOPE_MAX];
    char parent_case_id[YAI_CASE_REF_MAX];
    char declared_family[YAI_CASE_SEMANTIC_MAX];
    char declared_specialization[YAI_CASE_SEMANTIC_MAX];
    char effective_family[YAI_CASE_SEMANTIC_MAX];
    char effective_specialization[YAI_CASE_SEMANTIC_MAX];
    char policy_profile_ref[YAI_CASE_REF_MAX];
    char substrate_ref[YAI_CASE_REF_MAX];
    int normative_materialization_ready;
    int active;
} yai_case_domain_t;

typedef struct yai_case_attachment {
    yai_case_ref_t case_ref;
    yai_subject_ref_t subject_ref;
    yai_case_attachment_kind_t attachment_kind;
    yai_case_posture_t posture;
    char attachment_ref[YAI_CASE_REF_MAX];
    char asset_ref[YAI_CASE_REF_MAX];
    char trace_ref[YAI_CASE_REF_MAX];
} yai_case_attachment_t;

typedef struct yai_case_binding {
    yai_case_ref_t case_ref;
    yai_case_binding_kind_t binding_kind;
    yai_case_posture_t posture;
    char binding_ref[YAI_CASE_REF_MAX];
    char target_ref[YAI_CASE_REF_MAX];
    char trace_ref[YAI_CASE_REF_MAX];
} yai_case_binding_t;

const char *yai_case_domain_kind_string(yai_case_domain_kind_t kind);
const char *yai_case_attachment_kind_string(yai_case_attachment_kind_t kind);
const char *yai_case_binding_kind_string(yai_case_binding_kind_t kind);
const char *yai_case_posture_string(yai_case_posture_t posture);

yai_status_t yai_case_domain_init(yai_case_domain_t *domain,
                                  const yai_case_ref_t *case_ref,
                                  yai_case_domain_kind_t domain_kind,
                                  const char *scope_id,
                                  const char *parent_case_id,
                                  const char *declared_family,
                                  const char *declared_specialization);

yai_status_t yai_case_attachment_init(yai_case_attachment_t *attachment,
                                      const yai_case_ref_t *case_ref,
                                      const yai_subject_ref_t *subject_ref,
                                      yai_case_attachment_kind_t attachment_kind,
                                      yai_case_posture_t posture,
                                      const char *attachment_ref,
                                      const char *asset_ref,
                                      const char *trace_ref);

yai_status_t yai_case_binding_init(yai_case_binding_t *binding,
                                   const yai_case_ref_t *case_ref,
                                   yai_case_binding_kind_t binding_kind,
                                   yai_case_posture_t posture,
                                   const char *binding_ref,
                                   const char *target_ref,
                                   const char *trace_ref);

yai_status_t yai_case_domain_record_init(yai_store_record_t *record,
                                         const char *record_id,
                                         const yai_case_domain_t *domain);

yai_status_t yai_case_attachment_record_init(yai_store_record_t *record,
                                             const char *record_id,
                                             const yai_case_attachment_t *attachment);

yai_status_t yai_case_binding_record_init(yai_store_record_t *record,
                                          const char *record_id,
                                          const yai_case_binding_t *binding);

#endif
