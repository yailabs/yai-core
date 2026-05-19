#ifndef YAI_BASE_ID_H
#define YAI_BASE_ID_H

#include <stddef.h>

typedef struct yai_id {
    char value[64];
} yai_id_t;

void yai_id_set(yai_id_t *id, const char *value);
int yai_id_is_empty(const yai_id_t *id);
int yai_id_equal(const yai_id_t *left, const yai_id_t *right);

#endif
