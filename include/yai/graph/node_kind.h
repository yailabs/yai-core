#ifndef YAI_GRAPH_NODE_KIND_H
#define YAI_GRAPH_NODE_KIND_H

#include "yai/base/error.h"

typedef enum yai_graph_node_kind {
    YAI_GRAPH_NODE_CASE = 0,
    YAI_GRAPH_NODE_SUBJECT = 1,
    YAI_GRAPH_NODE_OPERATION = 2,
    YAI_GRAPH_NODE_ATTEMPT = 3,
    YAI_GRAPH_NODE_DECISION = 4,
    YAI_GRAPH_NODE_DISPATCH = 5,
    YAI_GRAPH_NODE_CARRIER = 6,
    YAI_GRAPH_NODE_RECEIPT = 7,
    YAI_GRAPH_NODE_EFFECT = 8,
    YAI_GRAPH_NODE_OBSERVATION = 9,
    YAI_GRAPH_NODE_DIVERGENCE = 10,
    YAI_GRAPH_NODE_POLICY = 11,
    YAI_GRAPH_NODE_PROJECTION = 12,
    YAI_GRAPH_NODE_MEMORY_CANDIDATE = 13,
    YAI_GRAPH_NODE_MODEL_OUTPUT = 14,
    YAI_GRAPH_NODE_MODEL_INTERPRETATION = 15,
    YAI_GRAPH_NODE_RECORD = 16,
    YAI_GRAPH_NODE_UNKNOWN = 17
} yai_graph_node_kind_t;

const char *yai_graph_node_kind_string(yai_graph_node_kind_t kind);
yai_status_t yai_graph_node_kind_from_string(const char *value,
                                             yai_graph_node_kind_t *kind);

#endif
