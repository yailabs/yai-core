#ifndef YAI_GRAPH_GRAPH_H
#define YAI_GRAPH_GRAPH_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/graph/edge.h"

typedef struct yai_graph {
    yai_graph_edge_t *edges;
    size_t count;
    size_t capacity;
} yai_graph_t;

yai_status_t yai_graph_init(yai_graph_t *graph);
yai_status_t yai_graph_append(yai_graph_t *graph,
                              const yai_graph_edge_t *edge);
size_t yai_graph_count(const yai_graph_t *graph);
size_t yai_graph_count_kind(const yai_graph_t *graph,
                            yai_graph_edge_kind_t kind);
const yai_graph_edge_t *yai_graph_get(const yai_graph_t *graph,
                                      size_t index);
void yai_graph_free(yai_graph_t *graph);

#endif
