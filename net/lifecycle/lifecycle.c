#include "yai/net/lifecycle.h"

const char *yai_net_lifecycle_state_name(yai_net_lifecycle_state_t value) {
    switch (value) {
    case YAI_NET_LIFECYCLE_STATE_UNKNOWN:
        return "unknown";
    case YAI_NET_LIFECYCLE_STATE_DECLARED:
        return "declared";
    case YAI_NET_LIFECYCLE_STATE_PREPARING:
        return "preparing";
    case YAI_NET_LIFECYCLE_STATE_STARTING:
        return "starting";
    case YAI_NET_LIFECYCLE_STATE_RUNNING:
        return "running";
    case YAI_NET_LIFECYCLE_STATE_STOPPING:
        return "stopping";
    case YAI_NET_LIFECYCLE_STATE_STOPPED:
        return "stopped";
    case YAI_NET_LIFECYCLE_STATE_FAILED:
        return "failed";
    case YAI_NET_LIFECYCLE_STATE_RETIRED:
        return "retired";
    default:
        return "unknown";
    }
}

int yai_net_lifecycle_state_is_valid(yai_net_lifecycle_state_t value) {
    return value >= YAI_NET_LIFECYCLE_STATE_UNKNOWN && value <= YAI_NET_LIFECYCLE_STATE_RETIRED;
}
