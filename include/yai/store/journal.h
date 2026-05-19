#ifndef YAI_STORE_JOURNAL_H
#define YAI_STORE_JOURNAL_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/store/record.h"

typedef struct yai_journal {
    yai_store_record_t *records;
    size_t count;
    size_t capacity;
} yai_journal_t;

yai_status_t yai_journal_init(yai_journal_t *journal);
yai_status_t yai_journal_append(yai_journal_t *journal,
                                const yai_store_record_t *record);
size_t yai_journal_count(const yai_journal_t *journal);
const yai_store_record_t *yai_journal_get(const yai_journal_t *journal,
                                          size_t index);
void yai_journal_free(yai_journal_t *journal);

#endif
