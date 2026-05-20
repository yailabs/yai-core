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
        }
    }

    (void)snprintf(projection->summary,
                   sizeof(projection->summary),
                   "projection:%s records:%zu decisions:%zu rules:%zu gates:%zu obligations:%zu receipt_requirements:%zu filesystem_receipts:%zu subject_states:%zu effects:%zu graph_edges:%zu reconstructions:%zu memory_candidates:%zu divergences:%zu reconciliations:%zu",
                   yai_projection_consumer_string(consumer_kind),
                   projection->source_record_count,
                   projection->decision_count,
                   projection->policy_rule_count,
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
                   projection->reconciliation_count);
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
