#include "yai/graph/graph.h"

#include <stdlib.h>
#include <string.h>

yai_status_t yai_graph_init(yai_graph_t *graph) {
    if (graph == 0) {
        return YAI_ERR_INVALID;
    }
    graph->edges = 0;
    graph->count = 0;
    graph->capacity = 0;
    return YAI_OK;
}

yai_status_t yai_graph_append(yai_graph_t *graph,
                              const yai_graph_edge_t *edge) {
    yai_graph_edge_t *next_edges = 0;
    size_t next_capacity = 0;

    if (graph == 0 || edge == 0) {
        return YAI_ERR_INVALID;
    }

    if (graph->count == graph->capacity) {
        next_capacity = graph->capacity == 0 ? 8 : graph->capacity * 2;
        next_edges = (yai_graph_edge_t *)realloc(graph->edges, next_capacity * sizeof(*graph->edges));
        if (next_edges == 0) {
            return YAI_ERR_INVALID;
        }
        graph->edges = next_edges;
        graph->capacity = next_capacity;
    }

    memcpy(&graph->edges[graph->count], edge, sizeof(*edge));
    graph->count += 1;
    return YAI_OK;
}

size_t yai_graph_count(const yai_graph_t *graph) {
    return graph == 0 ? 0 : graph->count;
}

size_t yai_graph_count_kind(const yai_graph_t *graph,
                            yai_graph_edge_kind_t kind) {
    size_t index = 0;
    size_t count = 0;
    if (graph == 0) {
        return 0;
    }
    for (index = 0; index < graph->count; index += 1) {
        if (graph->edges[index].edge_kind == kind) {
            count += 1;
        }
    }
    return count;
}

const yai_graph_edge_t *yai_graph_get(const yai_graph_t *graph,
                                      size_t index) {
    if (graph == 0 || index >= graph->count) {
        return 0;
    }
    return &graph->edges[index];
}

void yai_graph_free(yai_graph_t *graph) {
    if (graph == 0) {
        return;
    }
    free(graph->edges);
    graph->edges = 0;
    graph->count = 0;
    graph->capacity = 0;
}
