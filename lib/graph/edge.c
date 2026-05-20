#include "yai/graph/edge.h"

#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_graph_edge_init(yai_graph_edge_t *edge,
                                 const char *edge_id,
                                 const yai_case_ref_t *case_ref,
                                 yai_graph_edge_kind_t edge_kind,
                                 const char *from_ref,
                                 const char *to_ref,
                                 const char *summary) {
    if (edge == 0 || edge_id == 0 || edge_id[0] == '\0' ||
        yai_case_ref_is_empty(case_ref) ||
        from_ref == 0 || from_ref[0] == '\0' ||
        to_ref == 0 || to_ref[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_id_set(&edge->edge_id, edge_id);
    edge->case_ref = *case_ref;
    edge->edge_kind = edge_kind;
    yai_id_set(&edge->from_ref, from_ref);
    yai_id_set(&edge->to_ref, to_ref);
    yai_copy_string(edge->summary, sizeof(edge->summary), summary);
    return YAI_OK;
}

const char *yai_graph_edge_kind_string(yai_graph_edge_kind_t kind) {
    switch (kind) {
    case YAI_GRAPH_EDGE_CASE_BINDS_SUBJECT:
        return "case_binds_subject";
    case YAI_GRAPH_EDGE_OP_TARGETS_SUBJECT:
        return "op_targets_subject";
    case YAI_GRAPH_EDGE_OP_HAS_ACTOR:
        return "op_has_actor";
    case YAI_GRAPH_EDGE_DECISION_CONTROLS_OP:
        return "decision_controls_op";
    case YAI_GRAPH_EDGE_GATE_SUPPORTS_DECISION:
        return "gate_supports_decision";
    case YAI_GRAPH_EDGE_OBLIGATION_ATTACHED_TO_DECISION:
        return "obligation_attached_to_decision";
    case YAI_GRAPH_EDGE_RECEIPT_RECORDS_EFFECT:
        return "receipt_records_effect";
    case YAI_GRAPH_EDGE_RECEIPT_UPDATES_SUBJECT:
        return "receipt_updates_subject";
    case YAI_GRAPH_EDGE_CARRIER_ACTS_ON_SUBJECT:
        return "carrier_acts_on_subject";
    case YAI_GRAPH_EDGE_PROJECTION_DERIVED_FROM_RECORD:
        return "projection_derived_from_record";
    default:
        return "unknown";
    }
}

yai_status_t yai_graph_edge_kind_from_string(const char *value,
                                             yai_graph_edge_kind_t *kind) {
    if (value == 0 || kind == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "case_binds_subject") == 0) {
        *kind = YAI_GRAPH_EDGE_CASE_BINDS_SUBJECT;
    } else if (strcmp(value, "op_targets_subject") == 0) {
        *kind = YAI_GRAPH_EDGE_OP_TARGETS_SUBJECT;
    } else if (strcmp(value, "op_has_actor") == 0) {
        *kind = YAI_GRAPH_EDGE_OP_HAS_ACTOR;
    } else if (strcmp(value, "decision_controls_op") == 0) {
        *kind = YAI_GRAPH_EDGE_DECISION_CONTROLS_OP;
    } else if (strcmp(value, "gate_supports_decision") == 0) {
        *kind = YAI_GRAPH_EDGE_GATE_SUPPORTS_DECISION;
    } else if (strcmp(value, "obligation_attached_to_decision") == 0) {
        *kind = YAI_GRAPH_EDGE_OBLIGATION_ATTACHED_TO_DECISION;
    } else if (strcmp(value, "receipt_records_effect") == 0) {
        *kind = YAI_GRAPH_EDGE_RECEIPT_RECORDS_EFFECT;
    } else if (strcmp(value, "receipt_updates_subject") == 0) {
        *kind = YAI_GRAPH_EDGE_RECEIPT_UPDATES_SUBJECT;
    } else if (strcmp(value, "carrier_acts_on_subject") == 0) {
        *kind = YAI_GRAPH_EDGE_CARRIER_ACTS_ON_SUBJECT;
    } else if (strcmp(value, "projection_derived_from_record") == 0) {
        *kind = YAI_GRAPH_EDGE_PROJECTION_DERIVED_FROM_RECORD;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
