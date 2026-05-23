#include "yai/case/case_world.h"

#include <stdio.h>
#include <stddef.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

const char *yai_case_domain_kind_string(yai_case_domain_kind_t kind) {
    switch (kind) {
    case YAI_CASE_DOMAIN_ROOT:
        return "root_case";
    case YAI_CASE_DOMAIN_USER:
        return "user_case";
    case YAI_CASE_DOMAIN_NESTED:
        return "nested_case";
    case YAI_CASE_DOMAIN_OPERATIONAL:
        return "operational_case";
    default:
        return "unknown";
    }
}

const char *yai_case_attachment_kind_string(yai_case_attachment_kind_t kind) {
    switch (kind) {
    case YAI_CASE_ATTACHMENT_FILE:
        return "file";
    case YAI_CASE_ATTACHMENT_DIRECTORY:
        return "directory";
    case YAI_CASE_ATTACHMENT_ARTIFACT:
        return "artifact";
    case YAI_CASE_ATTACHMENT_REPOSITORY:
        return "repository";
    case YAI_CASE_ATTACHMENT_PROCESS:
        return "process";
    case YAI_CASE_ATTACHMENT_ENDPOINT:
        return "endpoint";
    case YAI_CASE_ATTACHMENT_DATABASE:
        return "database";
    case YAI_CASE_ATTACHMENT_SERVICE:
        return "service";
    case YAI_CASE_ATTACHMENT_SOCKET:
        return "socket";
    case YAI_CASE_ATTACHMENT_DATASET:
        return "dataset";
    case YAI_CASE_ATTACHMENT_MODEL:
        return "model";
    case YAI_CASE_ATTACHMENT_POLICY:
        return "policy";
    case YAI_CASE_ATTACHMENT_EVIDENCE:
        return "evidence";
    case YAI_CASE_ATTACHMENT_RUNTIME_SUBJECT:
        return "runtime_subject";
    case YAI_CASE_ATTACHMENT_SOURCE:
        return "source";
    case YAI_CASE_ATTACHMENT_UNKNOWN:
    default:
        return "unknown";
    }
}

const char *yai_case_binding_kind_string(yai_case_binding_kind_t kind) {
    switch (kind) {
    case YAI_CASE_BINDING_ACTOR:
        return "actor";
    case YAI_CASE_BINDING_WORKFLOW:
        return "workflow";
    case YAI_CASE_BINDING_CAPABILITY:
        return "capability";
    case YAI_CASE_BINDING_DECISION:
        return "decision";
    case YAI_CASE_BINDING_RUNTIME:
        return "runtime";
    case YAI_CASE_BINDING_MODEL:
        return "model";
    case YAI_CASE_BINDING_SUBSTRATE_RECORD:
        return "substrate_record";
    case YAI_CASE_BINDING_SUBSTRATE_SOURCE:
        return "substrate_source";
    case YAI_CASE_BINDING_SUBSTRATE_GRAPH:
        return "substrate_graph";
    case YAI_CASE_BINDING_SUBSTRATE_MEMORY:
        return "substrate_memory";
    case YAI_CASE_BINDING_POLICY:
        return "policy";
    case YAI_CASE_BINDING_EVIDENCE:
        return "evidence";
    case YAI_CASE_BINDING_SUBJECT:
        return "subject";
    case YAI_CASE_BINDING_ATTACHMENT:
        return "attachment";
    case YAI_CASE_BINDING_RECEIPT:
        return "receipt";
    case YAI_CASE_BINDING_PRINCIPAL:
        return "principal";
    case YAI_CASE_BINDING_EVENT:
        return "event";
    case YAI_CASE_BINDING_ATTEMPT:
        return "attempt";
    case YAI_CASE_BINDING_EFFECT:
        return "effect";
    default:
        return "unknown";
    }
}

const char *yai_case_posture_string(yai_case_posture_t posture) {
    switch (posture) {
    case YAI_CASE_POSTURE_CANDIDATE:
        return "candidate";
    case YAI_CASE_POSTURE_BOUND:
        return "bound";
    case YAI_CASE_POSTURE_STALE:
        return "stale";
    case YAI_CASE_POSTURE_CHALLENGED:
        return "challenged";
    case YAI_CASE_POSTURE_RETIRED:
        return "retired";
    default:
        return "unknown";
    }
}

yai_status_t yai_case_domain_init(yai_case_domain_t *domain,
                                  const yai_case_ref_t *case_ref,
                                  yai_case_domain_kind_t domain_kind,
                                  const char *scope_id,
                                  const char *parent_case_id,
                                  const char *declared_family,
                                  const char *declared_specialization) {
    if (domain == 0 || yai_case_ref_is_empty(case_ref)) {
        return YAI_ERR_INVALID;
    }

    domain->case_ref = *case_ref;
    domain->domain_kind = domain_kind;
    yai_copy_string(domain->scope_id, sizeof(domain->scope_id), scope_id);
    yai_copy_string(domain->parent_case_id, sizeof(domain->parent_case_id), parent_case_id);
    yai_copy_string(domain->declared_family, sizeof(domain->declared_family), declared_family);
    yai_copy_string(domain->declared_specialization,
                    sizeof(domain->declared_specialization),
                    declared_specialization);
    yai_copy_string(domain->effective_family,
                    sizeof(domain->effective_family),
                    declared_family);
    yai_copy_string(domain->effective_specialization,
                    sizeof(domain->effective_specialization),
                    declared_specialization);
    yai_copy_string(domain->policy_profile_ref,
                    sizeof(domain->policy_profile_ref),
                    "policy:case-default");
    yai_copy_string(domain->substrate_ref,
                    sizeof(domain->substrate_ref),
                    "substrate:journal-graph-memory");
    domain->normative_materialization_ready = 0;
    domain->active = 1;
    return YAI_OK;
}

yai_status_t yai_case_attachment_init(yai_case_attachment_t *attachment,
                                      const yai_case_ref_t *case_ref,
                                      const yai_subject_ref_t *subject_ref,
                                      yai_case_attachment_kind_t attachment_kind,
                                      yai_case_posture_t posture,
                                      const char *attachment_ref,
                                      const char *asset_ref,
                                      const char *trace_ref) {
    if (attachment == 0 || yai_case_ref_is_empty(case_ref) || subject_ref == 0) {
        return YAI_ERR_INVALID;
    }

    attachment->case_ref = *case_ref;
    attachment->subject_ref = *subject_ref;
    attachment->attachment_kind = attachment_kind;
    attachment->posture = posture;
    yai_copy_string(attachment->attachment_ref,
                    sizeof(attachment->attachment_ref),
                    attachment_ref);
    yai_copy_string(attachment->asset_ref, sizeof(attachment->asset_ref), asset_ref);
    yai_copy_string(attachment->trace_ref, sizeof(attachment->trace_ref), trace_ref);
    return YAI_OK;
}

yai_status_t yai_case_binding_init(yai_case_binding_t *binding,
                                   const yai_case_ref_t *case_ref,
                                   yai_case_binding_kind_t binding_kind,
                                   yai_case_posture_t posture,
                                   const char *binding_ref,
                                   const char *target_ref,
                                   const char *trace_ref) {
    if (binding == 0 || yai_case_ref_is_empty(case_ref) ||
        binding_ref == 0 || binding_ref[0] == '\0') {
        return YAI_ERR_INVALID;
    }

    binding->case_ref = *case_ref;
    binding->binding_kind = binding_kind;
    binding->posture = posture;
    yai_copy_string(binding->binding_ref, sizeof(binding->binding_ref), binding_ref);
    yai_copy_string(binding->target_ref, sizeof(binding->target_ref), target_ref);
    yai_copy_string(binding->trace_ref, sizeof(binding->trace_ref), trace_ref);
    return YAI_OK;
}

yai_status_t yai_case_domain_record_init(yai_store_record_t *record,
                                         const char *record_id,
                                         const yai_case_domain_t *domain) {
    char summary[128];

    if (domain == 0) {
        return YAI_ERR_INVALID;
    }
    (void)snprintf(summary,
                   sizeof(summary),
                   "case_domain domain:%.20s scope:%.20s family:%.24s active:%s",
                   yai_case_domain_kind_string(domain->domain_kind),
                   domain->scope_id,
                   domain->effective_family,
                   domain->active ? "true" : "false");
    return yai_store_record_init(record,
                                 record_id,
                                 &domain->case_ref,
                                 YAI_RECORD_CASE_DOMAIN,
                                 0,
                                 0,
                                 0,
                                 0,
                                 summary);
}

yai_status_t yai_case_attachment_record_init(yai_store_record_t *record,
                                             const char *record_id,
                                             const yai_case_attachment_t *attachment) {
    char summary[128];

    if (attachment == 0) {
        return YAI_ERR_INVALID;
    }
    (void)snprintf(summary,
                   sizeof(summary),
                   "case_attachment kind:%.20s posture:%.12s subject:%.40s",
                   yai_case_attachment_kind_string(attachment->attachment_kind),
                   yai_case_posture_string(attachment->posture),
                   attachment->subject_ref.subject_id.value);
    return yai_store_record_init(record,
                                 record_id,
                                 &attachment->case_ref,
                                 YAI_RECORD_CASE_ATTACHMENT,
                                 &attachment->subject_ref,
                                 0,
                                 0,
                                 0,
                                 summary);
}

yai_status_t yai_case_binding_record_init(yai_store_record_t *record,
                                          const char *record_id,
                                          const yai_case_binding_t *binding) {
    char summary[128];

    if (binding == 0) {
        return YAI_ERR_INVALID;
    }
    (void)snprintf(summary,
                   sizeof(summary),
                   "case_binding kind:%.20s posture:%.12s target:%.40s",
                   yai_case_binding_kind_string(binding->binding_kind),
                   yai_case_posture_string(binding->posture),
                   binding->target_ref);
    return yai_store_record_init(record,
                                 record_id,
                                 &binding->case_ref,
                                 YAI_RECORD_CASE_BINDING,
                                 0,
                                 0,
                                 0,
                                 0,
                                 summary);
}
