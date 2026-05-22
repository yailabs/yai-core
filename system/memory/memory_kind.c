#include "yai/memory/memory_kind.h"

#include <string.h>

const char *yai_memory_kind_string(yai_memory_kind_t kind) {
    switch (kind) {
    case YAI_MEMORY_OPERATIONAL:
        return "operational";
    case YAI_MEMORY_DECISION:
        return "decision";
    case YAI_MEMORY_SUBJECT:
        return "subject";
    case YAI_MEMORY_ERROR:
        return "error";
    case YAI_MEMORY_RECOVERY:
        return "recovery";
    default:
        return "unknown";
    }
}

yai_status_t yai_memory_kind_from_string(const char *value,
                                         yai_memory_kind_t *kind) {
    if (value == 0 || kind == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "operational") == 0) {
        *kind = YAI_MEMORY_OPERATIONAL;
    } else if (strcmp(value, "decision") == 0) {
        *kind = YAI_MEMORY_DECISION;
    } else if (strcmp(value, "subject") == 0) {
        *kind = YAI_MEMORY_SUBJECT;
    } else if (strcmp(value, "error") == 0) {
        *kind = YAI_MEMORY_ERROR;
    } else if (strcmp(value, "recovery") == 0) {
        *kind = YAI_MEMORY_RECOVERY;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}

const char *yai_memory_freshness_string(yai_memory_freshness_t freshness) {
    switch (freshness) {
    case YAI_MEMORY_FRESH:
        return "fresh";
    case YAI_MEMORY_STALE:
        return "stale";
    case YAI_MEMORY_FRESHNESS_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_status_t yai_memory_freshness_from_string(const char *value,
                                              yai_memory_freshness_t *freshness) {
    if (value == 0 || freshness == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "fresh") == 0) {
        *freshness = YAI_MEMORY_FRESH;
    } else if (strcmp(value, "stale") == 0) {
        *freshness = YAI_MEMORY_STALE;
    } else if (strcmp(value, "unknown") == 0) {
        *freshness = YAI_MEMORY_FRESHNESS_UNKNOWN;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}

const char *yai_memory_confidence_string(yai_memory_confidence_t confidence) {
    switch (confidence) {
    case YAI_MEMORY_CONFIDENCE_HIGH:
        return "high";
    case YAI_MEMORY_CONFIDENCE_MEDIUM:
        return "medium";
    case YAI_MEMORY_CONFIDENCE_LOW:
        return "low";
    case YAI_MEMORY_CONFIDENCE_UNKNOWN:
        return "unknown";
    default:
        return "unknown";
    }
}

yai_status_t yai_memory_confidence_from_string(const char *value,
                                               yai_memory_confidence_t *confidence) {
    if (value == 0 || confidence == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "high") == 0) {
        *confidence = YAI_MEMORY_CONFIDENCE_HIGH;
    } else if (strcmp(value, "medium") == 0) {
        *confidence = YAI_MEMORY_CONFIDENCE_MEDIUM;
    } else if (strcmp(value, "low") == 0) {
        *confidence = YAI_MEMORY_CONFIDENCE_LOW;
    } else if (strcmp(value, "unknown") == 0) {
        *confidence = YAI_MEMORY_CONFIDENCE_UNKNOWN;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
