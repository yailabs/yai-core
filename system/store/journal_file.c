#include "yai/store/journal_file.h"

#include "yai/store/record_codec.h"

#include <stdio.h>
#include <string.h>

void yai_copy_string(char *dst, size_t dst_size, const char *src);

yai_status_t yai_journal_file_init(yai_journal_file_t *journal_file,
                                   const char *path) {
    if (journal_file == 0 || path == 0 || path[0] == '\0') {
        return YAI_ERR_INVALID;
    }
    yai_copy_string(journal_file->path, sizeof(journal_file->path), path);
    return journal_file->path[0] == '\0' ? YAI_ERR_INVALID : YAI_OK;
}

yai_status_t yai_journal_file_append(const yai_journal_file_t *journal_file,
                                     const yai_store_record_t *record) {
    char line[1024];
    FILE *file = 0;
    yai_status_t status = YAI_OK;

    if (journal_file == 0 || journal_file->path[0] == '\0' || record == 0) {
        return YAI_ERR_INVALID;
    }

    status = yai_store_record_encode_jsonl(record, line, sizeof(line));
    if (status != YAI_OK) {
        return status;
    }

    file = fopen(journal_file->path, "ab");
    if (file == 0) {
        return YAI_ERR_NOT_FOUND;
    }
    if (fputs(line, file) < 0) {
        (void)fclose(file);
        return YAI_ERR_INVALID;
    }
    if (fclose(file) != 0) {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}

yai_status_t yai_journal_file_load(const yai_journal_file_t *journal_file,
                                   yai_journal_t *journal) {
    char line[2048];
    FILE *file = 0;

    if (journal_file == 0 || journal_file->path[0] == '\0' || journal == 0) {
        return YAI_ERR_INVALID;
    }

    file = fopen(journal_file->path, "rb");
    if (file == 0) {
        return YAI_ERR_NOT_FOUND;
    }

    while (fgets(line, sizeof(line), file) != 0) {
        yai_store_record_t record;
        yai_status_t status = yai_store_record_decode_jsonl(line, &record);
        if (status != YAI_OK) {
            (void)fclose(file);
            return status;
        }
        status = yai_journal_append(journal, &record);
        if (status != YAI_OK) {
            (void)fclose(file);
            return status;
        }
    }

    if (ferror(file) != 0) {
        (void)fclose(file);
        return YAI_ERR_INVALID;
    }

    if (fclose(file) != 0) {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
