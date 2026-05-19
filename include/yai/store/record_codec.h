#ifndef YAI_STORE_RECORD_CODEC_H
#define YAI_STORE_RECORD_CODEC_H

#include <stddef.h>

#include "yai/base/error.h"
#include "yai/store/record.h"

yai_status_t yai_store_record_encode_jsonl(const yai_store_record_t *record,
                                           char *buffer,
                                           size_t buffer_size);
yai_status_t yai_store_record_decode_jsonl(const char *line,
                                           yai_store_record_t *record);

#endif
