#ifndef YAI_EFFECT_EFFECT_HASH_H
#define YAI_EFFECT_EFFECT_HASH_H

#include <stddef.h>

#include "yai/base/error.h"

yai_status_t yai_effect_hash_bytes(const unsigned char *bytes,
                                   size_t byte_count,
                                   char *hash,
                                   size_t hash_size);
yai_status_t yai_effect_hash_file(const char *path,
                                  char *hash,
                                  size_t hash_size,
                                  size_t *byte_count);

#endif
