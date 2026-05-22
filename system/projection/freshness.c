#include "yai/projection/freshness.h"

#include <string.h>

const char *yai_projection_freshness_string(yai_projection_freshness_t freshness) {
    switch (freshness) {
    case YAI_PROJECTION_FRESH:
        return "fresh";
    case YAI_PROJECTION_STALE:
        return "stale";
    case YAI_PROJECTION_FRESHNESS_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_status_t yai_projection_freshness_from_string(const char *value,
                                                  yai_projection_freshness_t *freshness) {
    if (value == 0 || freshness == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "fresh") == 0) {
        *freshness = YAI_PROJECTION_FRESH;
    } else if (strcmp(value, "stale") == 0) {
        *freshness = YAI_PROJECTION_STALE;
    } else if (strcmp(value, "unknown") == 0) {
        *freshness = YAI_PROJECTION_FRESHNESS_UNKNOWN;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
