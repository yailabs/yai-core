#include "yai/net/health.h"

const char *yai_net_health_state_name(yai_net_health_state_t value) {
    switch (value) {
    case YAI_NET_HEALTH_STATE_UNKNOWN:
        return "unknown";
    case YAI_NET_HEALTH_STATE_ALIVE:
        return "alive";
    case YAI_NET_HEALTH_STATE_READY:
        return "ready";
    case YAI_NET_HEALTH_STATE_DEGRADED:
        return "degraded";
    case YAI_NET_HEALTH_STATE_UNAVAILABLE:
        return "unavailable";
    case YAI_NET_HEALTH_STATE_FAILED:
        return "failed";
    default:
        return "unknown";
    }
}

int yai_net_health_state_is_valid(yai_net_health_state_t value) {
    return value >= YAI_NET_HEALTH_STATE_UNKNOWN && value <= YAI_NET_HEALTH_STATE_FAILED;
}
