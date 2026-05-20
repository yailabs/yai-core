#include "yai/effect/carrier.h"

const char *yai_carrier_kind_string(yai_carrier_kind_t kind) {
    switch (kind) {
    case YAI_CARRIER_FILESYSTEM:
        return "filesystem";
    case YAI_CARRIER_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

const char *yai_effect_kind_string(yai_effect_kind_t kind) {
    switch (kind) {
    case YAI_EFFECT_FS_READ:
        return "fs.read";
    case YAI_EFFECT_FS_WRITE:
        return "fs.write";
    case YAI_EFFECT_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}
