#ifndef YAI_STORE_JOURNAL_FILE_H
#define YAI_STORE_JOURNAL_FILE_H

#include "yai/base/error.h"
#include "yai/store/journal.h"
#include "yai/store/record.h"

typedef struct yai_journal_file {
    char path[256];
} yai_journal_file_t;

yai_status_t yai_journal_file_init(yai_journal_file_t *journal_file,
                                   const char *path);
yai_status_t yai_journal_file_append(const yai_journal_file_t *journal_file,
                                     const yai_store_record_t *record);
yai_status_t yai_journal_file_load(const yai_journal_file_t *journal_file,
                                   yai_journal_t *journal);

#endif
