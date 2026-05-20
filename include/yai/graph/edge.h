#ifndef YAI_GRAPH_EDGE_H
#define YAI_GRAPH_EDGE_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/case/case_ref.h"

typedef enum yai_graph_edge_kind {
    YAI_GRAPH_EDGE_CASE_BINDS_SUBJECT = 0,
    YAI_GRAPH_EDGE_OP_TARGETS_SUBJECT = 1,
    YAI_GRAPH_EDGE_OP_HAS_ACTOR = 2,
    YAI_GRAPH_EDGE_DECISION_CONTROLS_OP = 3,
    YAI_GRAPH_EDGE_GATE_SUPPORTS_DECISION = 4,
    YAI_GRAPH_EDGE_OBLIGATION_ATTACHED_TO_DECISION = 5,
    YAI_GRAPH_EDGE_RECEIPT_RECORDS_EFFECT = 6,
    YAI_GRAPH_EDGE_RECEIPT_UPDATES_SUBJECT = 7,
    YAI_GRAPH_EDGE_CARRIER_ACTS_ON_SUBJECT = 8,
    YAI_GRAPH_EDGE_PROJECTION_DERIVED_FROM_RECORD = 9
} yai_graph_edge_kind_t;

typedef struct yai_graph_edge {
    yai_id_t edge_id;
    yai_case_ref_t case_ref;
    yai_graph_edge_kind_t edge_kind;
    yai_id_t from_ref;
    yai_id_t to_ref;
    char summary[128];
} yai_graph_edge_t;

yai_status_t yai_graph_edge_init(yai_graph_edge_t *edge,
                                 const char *edge_id,
                                 const yai_case_ref_t *case_ref,
                                 yai_graph_edge_kind_t edge_kind,
                                 const char *from_ref,
                                 const char *to_ref,
                                 const char *summary);
const char *yai_graph_edge_kind_string(yai_graph_edge_kind_t kind);
yai_status_t yai_graph_edge_kind_from_string(const char *value,
                                             yai_graph_edge_kind_t *kind);

#endif
