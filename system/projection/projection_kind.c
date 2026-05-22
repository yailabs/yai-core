#include "yai/projection/projection_kind.h"

#include <string.h>

const char *yai_projection_kind_string(yai_projection_kind_t kind) {
    switch (kind) {
    case YAI_PROJECTION_KIND_OPERATOR_SUMMARY:
        return "operator_summary";
    case YAI_PROJECTION_KIND_MODEL_CONTEXT:
        return "model_context";
    case YAI_PROJECTION_KIND_AGENT_CONTEXT:
        return "agent_context";
    case YAI_PROJECTION_KIND_AUDIT_PACKET:
        return "audit_packet";
    case YAI_PROJECTION_KIND_DEBUG_DUMP:
        return "debug_dump";
    case YAI_PROJECTION_KIND_CONTROL_SUMMARY:
        return "control_summary";
    case YAI_PROJECTION_KIND_MEMORY_SUMMARY:
        return "memory_summary";
    default:
        return "unknown";
    }
}

yai_status_t yai_projection_kind_from_string(const char *value,
                                             yai_projection_kind_t *kind) {
    if (value == 0 || kind == 0) {
        return YAI_ERR_INVALID;
    }
    if (strcmp(value, "operator_summary") == 0) {
        *kind = YAI_PROJECTION_KIND_OPERATOR_SUMMARY;
    } else if (strcmp(value, "model_context") == 0) {
        *kind = YAI_PROJECTION_KIND_MODEL_CONTEXT;
    } else if (strcmp(value, "agent_context") == 0) {
        *kind = YAI_PROJECTION_KIND_AGENT_CONTEXT;
    } else if (strcmp(value, "audit_packet") == 0) {
        *kind = YAI_PROJECTION_KIND_AUDIT_PACKET;
    } else if (strcmp(value, "debug_dump") == 0) {
        *kind = YAI_PROJECTION_KIND_DEBUG_DUMP;
    } else if (strcmp(value, "control_summary") == 0) {
        *kind = YAI_PROJECTION_KIND_CONTROL_SUMMARY;
    } else if (strcmp(value, "memory_summary") == 0) {
        *kind = YAI_PROJECTION_KIND_MEMORY_SUMMARY;
    } else {
        return YAI_ERR_INVALID;
    }
    return YAI_OK;
}
