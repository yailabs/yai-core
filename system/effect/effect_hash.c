#include "yai/effect/effect_hash.h"

#include <stdio.h>

yai_status_t yai_effect_hash_bytes(const unsigned char *bytes,
                                   size_t byte_count,
                                   char *hash,
                                   size_t hash_size) {
    size_t index = 0;
    unsigned long long value = 1469598103934665603ull;

    if ((bytes == 0 && byte_count > 0) || hash == 0 || hash_size < 17) {
        return YAI_ERR_INVALID;
    }

    for (index = 0; index < byte_count; index += 1) {
        value ^= (unsigned long long)bytes[index];
        value *= 1099511628211ull;
    }

    (void)snprintf(hash, hash_size, "%016llx", value);
    return YAI_OK;
}

yai_status_t yai_effect_hash_file(const char *path,
                                  char *hash,
                                  size_t hash_size,
                                  size_t *byte_count) {
    FILE *file = 0;
    unsigned char buffer[512];
    size_t total = 0;
    size_t read_count = 0;
    unsigned long long value = 1469598103934665603ull;

    if (path == 0 || path[0] == '\0' || hash == 0 || hash_size < 17) {
        return YAI_ERR_INVALID;
    }

    file = fopen(path, "rb");
    if (file == 0) {
        return YAI_ERR_NOT_FOUND;
    }

    while ((read_count = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        size_t index = 0;
        for (index = 0; index < read_count; index += 1) {
            value ^= (unsigned long long)buffer[index];
            value *= 1099511628211ull;
        }
        total += read_count;
    }

    if (ferror(file) != 0) {
        (void)fclose(file);
        return YAI_ERR_INVALID;
    }
    if (fclose(file) != 0) {
        return YAI_ERR_INVALID;
    }

    if (byte_count != 0) {
        *byte_count = total;
    }
    (void)snprintf(hash, hash_size, "%016llx", value);
    return YAI_OK;
}
