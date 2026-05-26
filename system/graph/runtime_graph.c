#include "yai/graph/runtime_graph.h"

yai_status_t yai_runtime_graph_boundary_current(yai_runtime_graph_boundary_t *boundary) {
    if (boundary == 0) {
        return YAI_ERR_INVALID;
    }
    boundary->status = "active_minimal";
    boundary->role = "in_memory_active_case_working_set";
    boundary->working_set = "per_command_ephemeral";
    boundary->resident_service = "planned";
    boundary->source = "graph_relations";
    boundary->durable_truth = "graph_persistence";
    boundary->hnsw = "future_candidate_index";
    boundary->context_compiler = "future_consumer";
    boundary->graph_store_claim = "lmdb_graph_relations_v0";
    return YAI_OK;
}
