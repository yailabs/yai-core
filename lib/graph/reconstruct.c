#include "yai/graph/reconstruct.h"

#include <stdio.h>
#include <string.h>

static yai_status_t extract_summary_value(const char *summary,
                                          const char *key,
                                          char *dst,
                                          size_t dst_size) {
    const char *cursor = 0;
    size_t index = 0;
    char pattern[32];

    if (summary == 0 || key == 0 || dst == 0 || dst_size == 0) {
        return YAI_ERR_INVALID;
    }

    (void)snprintf(pattern, sizeof(pattern), "%s:", key);
    cursor = strstr(summary, pattern);
    if (cursor == 0) {
        dst[0] = '\0';
        return YAI_OK;
    }
    cursor += strlen(pattern);
    while (cursor[index] != '\0' &&
           cursor[index] != ' ' &&
           index + 1 < dst_size) {
        dst[index] = cursor[index];
        index += 1;
    }
    dst[index] = '\0';
    return YAI_OK;
}

yai_status_t yai_graph_from_journal(const yai_journal_t *journal,
                                    yai_graph_t *graph) {
    size_t index = 0;
    yai_status_t status = YAI_OK;

    if (journal == 0 || graph == 0) {
        return YAI_ERR_INVALID;
    }
    status = yai_graph_init(graph);
    if (status != YAI_OK) {
        return status;
    }

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        char edge_kind_value[64];
        char from_ref[64];
        char to_ref[64];
        yai_graph_edge_kind_t edge_kind;
        yai_graph_edge_t edge;

        if (record == 0 || record->record_kind != YAI_RECORD_GRAPH_EDGE) {
            continue;
        }
        status = extract_summary_value(record->summary, "edge", edge_kind_value, sizeof(edge_kind_value));
        if (status != YAI_OK) {
            return status;
        }
        status = extract_summary_value(record->summary, "from", from_ref, sizeof(from_ref));
        if (status != YAI_OK) {
            return status;
        }
        status = extract_summary_value(record->summary, "to", to_ref, sizeof(to_ref));
        if (status != YAI_OK) {
            return status;
        }
        if (yai_graph_edge_kind_from_string(edge_kind_value, &edge_kind) != YAI_OK) {
            continue;
        }
        status = yai_graph_edge_init(&edge,
                                     record->record_id.value,
                                     &record->case_ref,
                                     edge_kind,
                                     from_ref,
                                     to_ref,
                                     record->summary);
        if (status != YAI_OK) {
            return status;
        }
        status = yai_graph_append(graph, &edge);
        if (status != YAI_OK) {
            return status;
        }
    }
    return YAI_OK;
}

yai_status_t yai_reconstruct_receipt_chain(const yai_journal_t *journal,
                                           const char *receipt_id,
                                           yai_reconstruction_t *reconstruction) {
    size_t index = 0;
    yai_graph_t graph;
    yai_status_t status = YAI_OK;

    if (journal == 0 || receipt_id == 0 || receipt_id[0] == '\0' || reconstruction == 0) {
        return YAI_ERR_INVALID;
    }

    yai_id_set(&reconstruction->reconstruction_id, "reconstruction:receipt-chain");
    yai_id_set(&reconstruction->receipt_id, receipt_id);
    yai_id_set(&reconstruction->case_id, "");
    yai_id_set(&reconstruction->attempt_id, "");
    yai_id_set(&reconstruction->decision_id, "");
    yai_id_set(&reconstruction->subject_id, "");
    reconstruction->edge_count = 0;

    for (index = 0; index < yai_journal_count(journal); index += 1) {
        const yai_store_record_t *record = yai_journal_get(journal, index);
        if (record == 0) {
            continue;
        }
        if (strcmp(record->receipt_id.value, receipt_id) == 0 ||
            (record->record_kind == YAI_RECORD_FILESYSTEM_RECEIPT &&
             strcmp(record->receipt_id.value, receipt_id) == 0)) {
            reconstruction->case_id = record->case_ref.case_id;
            reconstruction->attempt_id = record->attempt_id;
            reconstruction->decision_id = record->decision_id;
            reconstruction->subject_id = record->subject_ref.subject_id;
        }
    }

    status = yai_graph_from_journal(journal, &graph);
    if (status != YAI_OK) {
        return status;
    }
    reconstruction->edge_count = yai_graph_count(&graph);
    yai_graph_free(&graph);

    if (reconstruction->case_id.value[0] == '\0' ||
        reconstruction->attempt_id.value[0] == '\0' ||
        reconstruction->decision_id.value[0] == '\0' ||
        reconstruction->subject_id.value[0] == '\0') {
        return YAI_ERR_NOT_FOUND;
    }

    (void)snprintf(reconstruction->summary,
                   sizeof(reconstruction->summary),
                   "chain:%s->%s->%s->%s->%s",
                   reconstruction->case_id.value,
                   reconstruction->attempt_id.value,
                   reconstruction->decision_id.value,
                   reconstruction->receipt_id.value,
                   reconstruction->subject_id.value);
    return YAI_OK;
}
