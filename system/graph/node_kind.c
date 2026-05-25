#include "yai/graph/node_kind.h"

#include <string.h>

const char *yai_graph_node_kind_string(yai_graph_node_kind_t kind) {
    switch (kind) {
    case YAI_GRAPH_NODE_CASE:
        return "case";
    case YAI_GRAPH_NODE_SUBJECT:
        return "subject";
    case YAI_GRAPH_NODE_OPERATION:
        return "operation";
    case YAI_GRAPH_NODE_ATTEMPT:
        return "attempt";
    case YAI_GRAPH_NODE_DECISION:
        return "decision";
    case YAI_GRAPH_NODE_DISPATCH:
        return "dispatch";
    case YAI_GRAPH_NODE_CARRIER:
        return "carrier";
    case YAI_GRAPH_NODE_RECEIPT:
        return "receipt";
    case YAI_GRAPH_NODE_EFFECT:
        return "effect";
    case YAI_GRAPH_NODE_OBSERVATION:
        return "observation";
    case YAI_GRAPH_NODE_DIVERGENCE:
        return "divergence";
    case YAI_GRAPH_NODE_POLICY:
        return "policy";
    case YAI_GRAPH_NODE_PROJECTION:
        return "projection";
    case YAI_GRAPH_NODE_MEMORY_CANDIDATE:
        return "memory_candidate";
    case YAI_GRAPH_NODE_MODEL_OUTPUT:
        return "model_output";
    case YAI_GRAPH_NODE_MODEL_INTERPRETATION:
        return "model_interpretation";
    case YAI_GRAPH_NODE_RECORD:
        return "record";
    case YAI_GRAPH_NODE_UNKNOWN:
    default:
        return "unknown";
    }
}

yai_status_t yai_graph_node_kind_from_string(const char *value,
                                             yai_graph_node_kind_t *kind) {
    if (value == 0 || kind == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "case") == 0) {
        *kind = YAI_GRAPH_NODE_CASE;
    } else if (strcmp(value, "subject") == 0) {
        *kind = YAI_GRAPH_NODE_SUBJECT;
    } else if (strcmp(value, "operation") == 0) {
        *kind = YAI_GRAPH_NODE_OPERATION;
    } else if (strcmp(value, "attempt") == 0) {
        *kind = YAI_GRAPH_NODE_ATTEMPT;
    } else if (strcmp(value, "decision") == 0) {
        *kind = YAI_GRAPH_NODE_DECISION;
    } else if (strcmp(value, "dispatch") == 0) {
        *kind = YAI_GRAPH_NODE_DISPATCH;
    } else if (strcmp(value, "carrier") == 0) {
        *kind = YAI_GRAPH_NODE_CARRIER;
    } else if (strcmp(value, "receipt") == 0) {
        *kind = YAI_GRAPH_NODE_RECEIPT;
    } else if (strcmp(value, "effect") == 0) {
        *kind = YAI_GRAPH_NODE_EFFECT;
    } else if (strcmp(value, "observation") == 0) {
        *kind = YAI_GRAPH_NODE_OBSERVATION;
    } else if (strcmp(value, "divergence") == 0) {
        *kind = YAI_GRAPH_NODE_DIVERGENCE;
    } else if (strcmp(value, "policy") == 0) {
        *kind = YAI_GRAPH_NODE_POLICY;
    } else if (strcmp(value, "projection") == 0) {
        *kind = YAI_GRAPH_NODE_PROJECTION;
    } else if (strcmp(value, "memory_candidate") == 0) {
        *kind = YAI_GRAPH_NODE_MEMORY_CANDIDATE;
    } else if (strcmp(value, "model_output") == 0) {
        *kind = YAI_GRAPH_NODE_MODEL_OUTPUT;
    } else if (strcmp(value, "model_interpretation") == 0) {
        *kind = YAI_GRAPH_NODE_MODEL_INTERPRETATION;
    } else if (strcmp(value, "record") == 0) {
        *kind = YAI_GRAPH_NODE_RECORD;
    } else if (strcmp(value, "unknown") == 0) {
        *kind = YAI_GRAPH_NODE_UNKNOWN;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
