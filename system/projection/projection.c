#include "yai/projection/projection.h"

#include <stdio.h>
#include <string.h>

static int summary_has_memory_kind(const yai_store_record_t *record,
                                   const char *kind) {
    char pattern[64];

    if (record == 0 || kind == 0) {
        return 0;
    }
    (void)snprintf(pattern, sizeof(pattern), "memory:%s", kind);
    return strstr(record->summary, pattern) != 0;
}

yai_status_t yai_projection_build(const char *projection_id,
                                  const yai_case_ref_t *case_ref,
                                  yai_projection_consumer_t consumer_kind,
                                  const yai_journal_t *journal,
                                  yai_projection_t *projection) {
    size_t index = 0;

    if (projection == 0 || projection_id == 0 || projection_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref) || journal == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&projection->projection_id, projection_id);
    projection->case_ref = *case_ref;
    projection->consumer_kind = consumer_kind;
    projection->source_record_count = yai_journal_count(journal);
    projection->decision_count = 0;
    projection->policy_rule_count = 0;
    projection->gate_count = 0;
    projection->obligation_count = 0;
    projection->receipt_requirement_count = 0;
    projection->filesystem_receipt_count = 0;
    projection->subject_state_count = 0;
    projection->effect_count = 0;
    projection->graph_edge_count = 0;
    projection->reconstruction_count = 0;
    projection->memory_candidate_count = 0;
    projection->operational_memory_candidate_count = 0;
    projection->decision_memory_candidate_count = 0;
    projection->subject_memory_candidate_count = 0;
    projection->error_memory_candidate_count = 0;
    projection->recovery_memory_candidate_count = 0;
    projection->divergence_count = 0;
    projection->reconciliation_count = 0;
    projection->critical_divergence_count = 0;
    projection->warning_divergence_count = 0;
    projection->projection_request_count = 0;
    projection->projection_result_count = 0;
    projection->operator_projection_count = 0;
    projection->model_projection_count = 0;
    projection->audit_projection_count = 0;
    projection->limited_projection_count = 0;
    projection->query_result_count = 0;
    projection->projection_rule_count = 0;
    projection->authority_scope_count = 0;
    projection->model_interpretation_count = 0;
    projection->case_domain_count = 0;
    projection->case_attachment_count = 0;
    projection->case_binding_count = 0;
    projection->interaction_thread_count = 0;
    projection->interaction_turn_count = 0;
    projection->participant_view_frame_count = 0;

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        if (record == 0) {
            continue;
        }
        if (record->record_kind == YAI_RECORD_DECISION) {
            projection->decision_count += 1;
        } else if (record->record_kind == YAI_RECORD_POLICY_RULE) {
            projection->policy_rule_count += 1;
        } else if (record->record_kind == YAI_RECORD_GATE_RESULT) {
            projection->gate_count += 1;
        } else if (record->record_kind == YAI_RECORD_OBLIGATION) {
            projection->obligation_count += 1;
        } else if (record->record_kind == YAI_RECORD_RECEIPT_REQUIREMENT) {
            projection->receipt_requirement_count += 1;
        } else if (record->record_kind == YAI_RECORD_FILESYSTEM_RECEIPT) {
            projection->filesystem_receipt_count += 1;
            projection->effect_count += 1;
        } else if (record->record_kind == YAI_RECORD_EFFECT_RECEIPT) {
            projection->effect_count += 1;
        } else if (record->record_kind == YAI_RECORD_SUBJECT_STATE) {
            projection->subject_state_count += 1;
        } else if (record->record_kind == YAI_RECORD_RECEIPT) {
            projection->effect_count += 1;
        } else if (record->record_kind == YAI_RECORD_GRAPH_EDGE) {
            projection->graph_edge_count += 1;
        } else if (record->record_kind == YAI_RECORD_RECONSTRUCTION) {
            projection->reconstruction_count += 1;
        } else if (record->record_kind == YAI_RECORD_MEMORY_CANDIDATE) {
            projection->memory_candidate_count += 1;
            if (summary_has_memory_kind(record, "operational")) {
                projection->operational_memory_candidate_count += 1;
            } else if (summary_has_memory_kind(record, "decision")) {
                projection->decision_memory_candidate_count += 1;
            } else if (summary_has_memory_kind(record, "subject")) {
                projection->subject_memory_candidate_count += 1;
            } else if (summary_has_memory_kind(record, "error")) {
                projection->error_memory_candidate_count += 1;
            } else if (summary_has_memory_kind(record, "recovery")) {
                projection->recovery_memory_candidate_count += 1;
            }
        } else if (record->record_kind == YAI_RECORD_DIVERGENCE) {
            projection->divergence_count += 1;
            if (strstr(record->summary, "severity:critical") != 0) {
                projection->critical_divergence_count += 1;
            } else if (strstr(record->summary, "severity:warning") != 0) {
                projection->warning_divergence_count += 1;
            }
        } else if (record->record_kind == YAI_RECORD_RECONCILIATION) {
            projection->reconciliation_count += 1;
        } else if (record->record_kind == YAI_RECORD_PROJECTION_REQUEST) {
            projection->projection_request_count += 1;
            if (strstr(record->summary, "consumer:operator") != 0) {
                projection->operator_projection_count += 1;
            } else if (strstr(record->summary, "consumer:model") != 0) {
                projection->model_projection_count += 1;
            } else if (strstr(record->summary, "consumer:audit") != 0) {
                projection->audit_projection_count += 1;
            }
        } else if (record->record_kind == YAI_RECORD_PROJECTION_RESULT) {
            projection->projection_result_count += 1;
            if (strstr(record->summary, "consumer:operator") != 0) {
                projection->operator_projection_count += 1;
            } else if (strstr(record->summary, "consumer:model") != 0) {
                projection->model_projection_count += 1;
            } else if (strstr(record->summary, "consumer:audit") != 0) {
                projection->audit_projection_count += 1;
            }
            if (strstr(record->summary, "redaction:summary_only") != 0 ||
                strstr(record->summary, "redaction:refs_only") != 0 ||
                strstr(record->summary, "redaction:redacted") != 0 ||
                strstr(record->summary, "redaction:blocked") != 0) {
                projection->limited_projection_count += 1;
            }
        } else if (record->record_kind == YAI_RECORD_QUERY_RESULT) {
            projection->query_result_count += 1;
        } else if (record->record_kind == YAI_RECORD_PROJECTION_RULE) {
            projection->projection_rule_count += 1;
        } else if (record->record_kind == YAI_RECORD_AUTHORITY_SCOPE) {
            projection->authority_scope_count += 1;
        } else if (record->record_kind == YAI_RECORD_MODEL_INTERPRETATION) {
            projection->model_interpretation_count += 1;
        } else if (record->record_kind == YAI_RECORD_CASE_DOMAIN) {
            projection->case_domain_count += 1;
        } else if (record->record_kind == YAI_RECORD_CASE_ATTACHMENT) {
            projection->case_attachment_count += 1;
        } else if (record->record_kind == YAI_RECORD_CASE_BINDING) {
            projection->case_binding_count += 1;
        } else if (record->record_kind == YAI_RECORD_INTERACTION_THREAD) {
            projection->interaction_thread_count += 1;
        } else if (record->record_kind == YAI_RECORD_INTERACTION_TURN) {
            projection->interaction_turn_count += 1;
        } else if (record->record_kind == YAI_RECORD_PARTICIPANT_VIEW_FRAME) {
            projection->participant_view_frame_count += 1;
        }
    }

    (void)snprintf(projection->summary,
                   sizeof(projection->summary),
                   "projection:%s records:%zu case_domains:%zu case_attachments:%zu case_bindings:%zu interaction_threads:%zu interaction_turns:%zu participant_view_frames:%zu decisions:%zu rules:%zu projection_rules:%zu authority_scopes:%zu model_interpretations:%zu gates:%zu obligations:%zu receipt_requirements:%zu filesystem_receipts:%zu subject_states:%zu effects:%zu graph_edges:%zu reconstructions:%zu memory_candidates:%zu divergences:%zu reconciliations:%zu projection_requests:%zu projection_results:%zu query_results:%zu",
                   yai_projection_consumer_string(consumer_kind),
                   projection->source_record_count,
                   projection->case_domain_count,
                   projection->case_attachment_count,
                   projection->case_binding_count,
                   projection->interaction_thread_count,
                   projection->interaction_turn_count,
                   projection->participant_view_frame_count,
                   projection->decision_count,
                   projection->policy_rule_count,
                   projection->projection_rule_count,
                   projection->authority_scope_count,
                   projection->model_interpretation_count,
                   projection->gate_count,
                   projection->obligation_count,
                   projection->receipt_requirement_count,
                   projection->filesystem_receipt_count,
                   projection->subject_state_count,
                   projection->effect_count,
                   projection->graph_edge_count,
                   projection->reconstruction_count,
                   projection->memory_candidate_count,
                   projection->divergence_count,
                   projection->reconciliation_count,
                   projection->projection_request_count,
                   projection->projection_result_count,
                   projection->query_result_count);
    return YAI_OK;
}

const char *yai_projection_consumer_string(yai_projection_consumer_t consumer) {
    switch (consumer) {
    case YAI_PROJECTION_OPERATOR:
        return "operator";
    case YAI_PROJECTION_MODEL:
        return "model";
    case YAI_PROJECTION_AGENT:
        return "agent";
    case YAI_PROJECTION_API:
        return "api";
    case YAI_PROJECTION_AUDIT:
        return "audit";
    case YAI_PROJECTION_DEBUG:
        return "debug";
    default:
        return "unknown";
    }
}
