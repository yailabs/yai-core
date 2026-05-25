#include "yai/graph/edge_kind.h"

#include <string.h>

const char *yai_graph_schema_edge_kind_string(yai_graph_schema_edge_kind_t kind) {
    switch (kind) {
    case YAI_GRAPH_SCHEMA_EDGE_BELONGS_TO_CASE:
        return "belongs_to_case";
    case YAI_GRAPH_SCHEMA_EDGE_SUBJECT_PARTICIPATES_IN_CASE:
        return "subject_participates_in_case";
    case YAI_GRAPH_SCHEMA_EDGE_ATTEMPT_TARGETS_SUBJECT:
        return "attempt_targets_subject";
    case YAI_GRAPH_SCHEMA_EDGE_DECISION_CONTROLS_ATTEMPT:
        return "decision_controls_attempt";
    case YAI_GRAPH_SCHEMA_EDGE_DISPATCH_ROUTES_DECISION:
        return "dispatch_routes_decision";
    case YAI_GRAPH_SCHEMA_EDGE_CARRIER_REALIZES_DISPATCH:
        return "carrier_realizes_dispatch";
    case YAI_GRAPH_SCHEMA_EDGE_RECEIPT_RECORDS_EFFECT:
        return "receipt_records_effect";
    case YAI_GRAPH_SCHEMA_EDGE_OBSERVATION_CHECKS_EFFECT:
        return "observation_checks_effect";
    case YAI_GRAPH_SCHEMA_EDGE_DIVERGENCE_DESCRIBES_MISMATCH:
        return "divergence_describes_mismatch";
    case YAI_GRAPH_SCHEMA_EDGE_POLICY_CONSTRAINS_SUBJECT:
        return "policy_constrains_subject";
    case YAI_GRAPH_SCHEMA_EDGE_POLICY_CONSTRAINS_OPERATION:
        return "policy_constrains_operation";
    case YAI_GRAPH_SCHEMA_EDGE_PROJECTION_EXPOSES_RECORD:
        return "projection_exposes_record";
    case YAI_GRAPH_SCHEMA_EDGE_MODEL_OUTPUT_PRODUCES_INTERPRETATION:
        return "model_output_produces_interpretation";
    case YAI_GRAPH_SCHEMA_EDGE_MEMORY_DERIVED_FROM_RECEIPT:
        return "memory_derived_from_receipt";
    case YAI_GRAPH_SCHEMA_EDGE_RECORD_MATERIALIZES_NODE:
        return "record_materializes_node";
    case YAI_GRAPH_SCHEMA_EDGE_DERIVED_FROM:
        return "derived_from";
    case YAI_GRAPH_SCHEMA_EDGE_SUPPORTS:
        return "supports";
    case YAI_GRAPH_SCHEMA_EDGE_CONTRADICTS:
        return "contradicts";
    case YAI_GRAPH_SCHEMA_EDGE_UNKNOWN:
    default:
        return "unknown";
    }
}

yai_status_t yai_graph_schema_edge_kind_from_string(const char *value,
                                                    yai_graph_schema_edge_kind_t *kind) {
    if (value == 0 || kind == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "belongs_to_case") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_BELONGS_TO_CASE;
    } else if (strcmp(value, "subject_participates_in_case") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_SUBJECT_PARTICIPATES_IN_CASE;
    } else if (strcmp(value, "attempt_targets_subject") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_ATTEMPT_TARGETS_SUBJECT;
    } else if (strcmp(value, "decision_controls_attempt") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_DECISION_CONTROLS_ATTEMPT;
    } else if (strcmp(value, "dispatch_routes_decision") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_DISPATCH_ROUTES_DECISION;
    } else if (strcmp(value, "carrier_realizes_dispatch") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_CARRIER_REALIZES_DISPATCH;
    } else if (strcmp(value, "receipt_records_effect") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_RECEIPT_RECORDS_EFFECT;
    } else if (strcmp(value, "observation_checks_effect") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_OBSERVATION_CHECKS_EFFECT;
    } else if (strcmp(value, "divergence_describes_mismatch") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_DIVERGENCE_DESCRIBES_MISMATCH;
    } else if (strcmp(value, "policy_constrains_subject") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_POLICY_CONSTRAINS_SUBJECT;
    } else if (strcmp(value, "policy_constrains_operation") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_POLICY_CONSTRAINS_OPERATION;
    } else if (strcmp(value, "projection_exposes_record") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_PROJECTION_EXPOSES_RECORD;
    } else if (strcmp(value, "model_output_produces_interpretation") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_MODEL_OUTPUT_PRODUCES_INTERPRETATION;
    } else if (strcmp(value, "memory_derived_from_receipt") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_MEMORY_DERIVED_FROM_RECEIPT;
    } else if (strcmp(value, "record_materializes_node") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_RECORD_MATERIALIZES_NODE;
    } else if (strcmp(value, "derived_from") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_DERIVED_FROM;
    } else if (strcmp(value, "supports") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_SUPPORTS;
    } else if (strcmp(value, "contradicts") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_CONTRADICTS;
    } else if (strcmp(value, "unknown") == 0) {
        *kind = YAI_GRAPH_SCHEMA_EDGE_UNKNOWN;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
