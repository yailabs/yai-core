#include "yai/store/rust_engine_backend.h"

#include <stdlib.h>

extern void *yai_engine_new(void);
extern void yai_engine_free(void *engine);
extern int yai_engine_append_record_json(void *engine, const char *json_line);
extern size_t yai_engine_record_count(const void *engine);
extern size_t yai_engine_count_kind(const void *engine, const char *record_kind);
extern size_t yai_engine_query_kind(const void *engine, const char *record_kind);
extern int yai_engine_projection_summary_json(const void *engine,
                                              char *buffer,
                                              size_t buffer_size);

struct yai_rust_engine {
    void *handle;
};

static yai_status_t engine_status(int status) {
    return status == 0 ? YAI_OK : YAI_ERR_INVALID;
}

yai_rust_engine_t *yai_rust_engine_create(void) {
    yai_rust_engine_t *engine = (yai_rust_engine_t *)malloc(sizeof(*engine));

    if (engine == 0) {
        return 0;
    }
    engine->handle = yai_engine_new();
    if (engine->handle == 0) {
        free(engine);
        return 0;
    }
    return engine;
}

void yai_rust_engine_destroy(yai_rust_engine_t *engine) {
    if (engine == 0 || engine->handle == 0) {
        return;
    }
    yai_engine_free(engine->handle);
    engine->handle = 0;
    free(engine);
}

yai_status_t yai_rust_engine_append_record_json(yai_rust_engine_t *engine,
                                                const char *json_line) {
    if (engine == 0 || engine->handle == 0 || json_line == 0) {
        return YAI_ERR_INVALID;
    }
    return engine_status(yai_engine_append_record_json(engine->handle, json_line));
}

size_t yai_rust_engine_record_count(const yai_rust_engine_t *engine) {
    if (engine == 0 || engine->handle == 0) {
        return 0;
    }
    return yai_engine_record_count(engine->handle);
}

size_t yai_rust_engine_count_kind(const yai_rust_engine_t *engine,
                                  const char *record_kind) {
    if (engine == 0 || engine->handle == 0 || record_kind == 0) {
        return 0;
    }
    return yai_engine_count_kind(engine->handle, record_kind);
}

size_t yai_rust_engine_query_kind(const yai_rust_engine_t *engine,
                                  const char *record_kind) {
    if (engine == 0 || engine->handle == 0 || record_kind == 0) {
        return 0;
    }
    return yai_engine_query_kind(engine->handle, record_kind);
}

yai_status_t yai_rust_engine_projection_summary_json(const yai_rust_engine_t *engine,
                                                     char *buffer,
                                                     size_t buffer_size) {
    if (engine == 0 || engine->handle == 0 || buffer == 0 || buffer_size == 0) {
        return YAI_ERR_INVALID;
    }
    return engine_status(yai_engine_projection_summary_json(engine->handle,
                                                            buffer,
                                                            buffer_size));
}
