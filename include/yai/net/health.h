#ifndef YAI_NET_HEALTH_H
#define YAI_NET_HEALTH_H

typedef enum yai_net_health_state {
    YAI_NET_HEALTH_UNKNOWN = 0,
    YAI_NET_HEALTH_ALIVE = 1,
    YAI_NET_HEALTH_READY = 2,
    YAI_NET_HEALTH_DEGRADED = 3,
    YAI_NET_HEALTH_UNAVAILABLE = 4
} yai_net_health_state_t;

#endif
