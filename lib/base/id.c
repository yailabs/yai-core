#include "yai/base/id.h"

#include <stdio.h>
#include <string.h>

void yai_id_set(yai_id_t *id, const char *value) {
    if (id == 0) {
        return;
    }
    if (value == 0) {
        id->value[0] = '\0';
        return;
    }
    (void)snprintf(id->value, sizeof(id->value), "%s", value);
}

int yai_id_is_empty(const yai_id_t *id) {
    return id == 0 || id->value[0] == '\0';
}

int yai_id_equal(const yai_id_t *left, const yai_id_t *right) {
    if (left == 0 || right == 0) {
        return 0;
    }
    return strncmp(left->value, right->value, sizeof(left->value)) == 0;
}
