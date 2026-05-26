#include "yai/yai.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct node_case {
    yai_graph_node_kind_t kind;
    const char *name;
} node_case_t;

typedef struct edge_case {
    yai_graph_schema_edge_kind_t kind;
    const char *name;
} edge_case_t;

int main(void) {
    static const node_case_t nodes[] = {
        {YAI_GRAPH_NODE_CASE, "case"},
        {YAI_GRAPH_NODE_SUBJECT, "subject"},
        {YAI_GRAPH_NODE_OPERATION, "operation"},
        {YAI_GRAPH_NODE_ATTEMPT, "attempt"},
        {YAI_GRAPH_NODE_DECISION, "decision"},
        {YAI_GRAPH_NODE_DISPATCH, "dispatch"},
        {YAI_GRAPH_NODE_CARRIER, "carrier"},
        {YAI_GRAPH_NODE_RECEIPT, "receipt"},
        {YAI_GRAPH_NODE_EFFECT, "effect"},
        {YAI_GRAPH_NODE_OBSERVATION, "observation"},
        {YAI_GRAPH_NODE_DIVERGENCE, "divergence"},
        {YAI_GRAPH_NODE_POLICY, "policy"},
        {YAI_GRAPH_NODE_PROJECTION, "projection"},
        {YAI_GRAPH_NODE_MEMORY_CANDIDATE, "memory_candidate"},
        {YAI_GRAPH_NODE_MODEL_OUTPUT, "model_output"},
        {YAI_GRAPH_NODE_MODEL_INTERPRETATION, "model_interpretation"},
        {YAI_GRAPH_NODE_RECORD, "record"},
        {YAI_GRAPH_NODE_UNKNOWN, "unknown"}
    };
    static const edge_case_t edges[] = {
        {YAI_GRAPH_SCHEMA_EDGE_BELONGS_TO_CASE, "belongs_to_case"},
        {YAI_GRAPH_SCHEMA_EDGE_SUBJECT_PARTICIPATES_IN_CASE, "subject_participates_in_case"},
        {YAI_GRAPH_SCHEMA_EDGE_ATTEMPT_TARGETS_SUBJECT, "attempt_targets_subject"},
        {YAI_GRAPH_SCHEMA_EDGE_DECISION_CONTROLS_ATTEMPT, "decision_controls_attempt"},
        {YAI_GRAPH_SCHEMA_EDGE_DISPATCH_ROUTES_DECISION, "dispatch_routes_decision"},
        {YAI_GRAPH_SCHEMA_EDGE_CARRIER_REALIZES_DISPATCH, "carrier_realizes_dispatch"},
        {YAI_GRAPH_SCHEMA_EDGE_RECEIPT_RECORDS_EFFECT, "receipt_records_effect"},
        {YAI_GRAPH_SCHEMA_EDGE_OBSERVATION_CHECKS_EFFECT, "observation_checks_effect"},
        {YAI_GRAPH_SCHEMA_EDGE_DIVERGENCE_DESCRIBES_MISMATCH, "divergence_describes_mismatch"},
        {YAI_GRAPH_SCHEMA_EDGE_POLICY_CONSTRAINS_SUBJECT, "policy_constrains_subject"},
        {YAI_GRAPH_SCHEMA_EDGE_POLICY_CONSTRAINS_OPERATION, "policy_constrains_operation"},
        {YAI_GRAPH_SCHEMA_EDGE_PROJECTION_EXPOSES_RECORD, "projection_exposes_record"},
        {YAI_GRAPH_SCHEMA_EDGE_MODEL_OUTPUT_PRODUCES_INTERPRETATION, "model_output_produces_interpretation"},
        {YAI_GRAPH_SCHEMA_EDGE_MEMORY_DERIVED_FROM_RECEIPT, "memory_derived_from_receipt"},
        {YAI_GRAPH_SCHEMA_EDGE_RECORD_MATERIALIZES_NODE, "record_materializes_node"},
        {YAI_GRAPH_SCHEMA_EDGE_DERIVED_FROM, "derived_from"},
        {YAI_GRAPH_SCHEMA_EDGE_SUPPORTS, "supports"},
        {YAI_GRAPH_SCHEMA_EDGE_CONTRADICTS, "contradicts"},
        {YAI_GRAPH_SCHEMA_EDGE_UNKNOWN, "unknown"}
    };
    yai_runtime_graph_boundary_t boundary;
    size_t index;

    for (index = 0; index < sizeof(nodes) / sizeof(nodes[0]); index += 1) {
        yai_graph_node_kind_t parsed;
        assert(strcmp(yai_graph_node_kind_string(nodes[index].kind), nodes[index].name) == 0);
        assert(yai_graph_node_kind_from_string(nodes[index].name, &parsed) == YAI_OK);
        assert(parsed == nodes[index].kind);
    }

    for (index = 0; index < sizeof(edges) / sizeof(edges[0]); index += 1) {
        yai_graph_schema_edge_kind_t parsed;
        assert(strcmp(yai_graph_schema_edge_kind_string(edges[index].kind), edges[index].name) == 0);
        assert(yai_graph_schema_edge_kind_from_string(edges[index].name, &parsed) == YAI_OK);
        assert(parsed == edges[index].kind);
    }

    assert(yai_runtime_graph_boundary_current(&boundary) == YAI_OK);
    assert(strcmp(boundary.status, "active_minimal") == 0);
    assert(strcmp(boundary.role, "in_memory_active_case_working_set") == 0);
    assert(strcmp(boundary.working_set, "per_command_ephemeral") == 0);
    assert(strcmp(boundary.resident_service, "planned") == 0);
    assert(strcmp(boundary.source, "graph_relations") == 0);
    assert(strcmp(boundary.durable_truth, "graph_persistence") == 0);
    assert(strcmp(boundary.hnsw, "future_candidate_index") == 0);
    assert(strcmp(boundary.context_compiler, "future_consumer") == 0);
    assert(strcmp(boundary.graph_store_claim, "lmdb_graph_relations_v0") == 0);

    printf("graph_schema:node_kinds ok\n");
    printf("graph_schema:edge_kinds ok\n");
    printf("runtime_graph:boundary ok\n");
    printf("runtime_graph:active_minimal ok\n");
    return 0;
}
