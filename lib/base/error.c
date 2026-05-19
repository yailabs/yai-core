#include "yai/base/error.h"

const char *yai_status_string(yai_status_t status) {
    switch (status) {
    case YAI_OK:
        return "ok";
    case YAI_ERR_INVALID:
        return "invalid";
    case YAI_ERR_NOT_FOUND:
        return "not_found";
    case YAI_ERR_BUFFER_TOO_SMALL:
        return "buffer_too_small";
    case YAI_ERR_FORBIDDEN:
        return "forbidden";
    case YAI_ERR_UNSUPPORTED:
        return "unsupported";
    default:
        return "unknown";
    }
}
