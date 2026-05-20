#ifndef YAI_STORE_RUST_ENGINE_BACKEND_H
#define YAI_STORE_RUST_ENGINE_BACKEND_H

#include <stddef.h>

#include "yai/base/error.h"

typedef struct yai_rust_engine yai_rust_engine_t;

yai_rust_engine_t *yai_rust_engine_create(void);
void yai_rust_engine_destroy(yai_rust_engine_t *engine);
yai_status_t yai_rust_engine_append_record_json(yai_rust_engine_t *engine,
                                                const char *json_line);
size_t yai_rust_engine_record_count(const yai_rust_engine_t *engine);
size_t yai_rust_engine_count_kind(const yai_rust_engine_t *engine,
                                  const char *record_kind);
size_t yai_rust_engine_query_kind(const yai_rust_engine_t *engine,
                                  const char *record_kind);
yai_status_t yai_rust_engine_projection_summary_json(const yai_rust_engine_t *engine,
                                                     char *buffer,
                                                     size_t buffer_size);

#endif
