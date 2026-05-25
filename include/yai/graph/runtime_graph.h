#ifndef YAI_GRAPH_RUNTIME_GRAPH_H
#define YAI_GRAPH_RUNTIME_GRAPH_H

#include "yai/base/error.h"

typedef struct yai_runtime_graph_boundary {
    const char *status;
    const char *role;
    const char *durable_truth;
    const char *hnsw;
    const char *context_compiler;
    const char *graph_store_claim;
} yai_runtime_graph_boundary_t;

yai_status_t yai_runtime_graph_boundary_current(yai_runtime_graph_boundary_t *boundary);

#endif
