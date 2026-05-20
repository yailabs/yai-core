#ifndef YAI_GRAPH_RECONSTRUCT_H
#define YAI_GRAPH_RECONSTRUCT_H

#include "yai/base/error.h"
#include "yai/base/id.h"
#include "yai/graph/graph.h"
#include "yai/store/journal.h"

typedef struct yai_reconstruction {
    yai_id_t reconstruction_id;
    yai_id_t receipt_id;
    yai_id_t case_id;
    yai_id_t attempt_id;
    yai_id_t decision_id;
    yai_id_t subject_id;
    size_t edge_count;
    char summary[128];
} yai_reconstruction_t;

yai_status_t yai_graph_from_journal(const yai_journal_t *journal,
                                    yai_graph_t *graph);
yai_status_t yai_reconstruct_receipt_chain(const yai_journal_t *journal,
                                           const char *receipt_id,
                                           yai_reconstruction_t *reconstruction);

#endif
