#include "yai/store/journal.h"

#include <stdlib.h>
#include <string.h>

yai_status_t yai_journal_init(yai_journal_t *journal) {
    if (journal == 0) {
        return YAI_ERR_INVALID;
    }
    journal->records = 0;
    journal->count = 0;
    journal->capacity = 0;
    return YAI_OK;
}

yai_status_t yai_journal_append(yai_journal_t *journal,
                                const yai_store_record_t *record) {
    yai_store_record_t *next_records = 0;
    size_t next_capacity = 0;

    if (journal == 0 || record == 0) {
        return YAI_ERR_INVALID;
    }

    if (journal->count == journal->capacity) {
        next_capacity = journal->capacity == 0 ? 8 : journal->capacity * 2;
        next_records = (yai_store_record_t *)realloc(journal->records, next_capacity * sizeof(*journal->records));
        if (next_records == 0) {
            return YAI_ERR_INVALID;
        }
        journal->records = next_records;
        journal->capacity = next_capacity;
    }

    memcpy(&journal->records[journal->count], record, sizeof(*record));
    journal->count += 1;
    return YAI_OK;
}

size_t yai_journal_count(const yai_journal_t *journal) {
    if (journal == 0) {
        return 0;
    }
    return journal->count;
}

const yai_store_record_t *yai_journal_get(const yai_journal_t *journal,
                                          size_t index) {
    if (journal == 0 || index >= journal->count) {
        return 0;
    }
    return &journal->records[index];
}

void yai_journal_free(yai_journal_t *journal) {
    if (journal == 0) {
        return;
    }
    free(journal->records);
    journal->records = 0;
    journal->count = 0;
    journal->capacity = 0;
}
