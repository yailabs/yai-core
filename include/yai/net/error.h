#ifndef YAI_NET_ERROR_H
#define YAI_NET_ERROR_H

typedef enum yai_net_status {
    YAI_NET_OK = 0,
    YAI_NET_ERR_INVALID_ARGUMENT = 1,
    YAI_NET_ERR_UNAVAILABLE = 2,
    YAI_NET_ERR_UNSUPPORTED = 3,
    YAI_NET_ERR_BOUNDARY_VIOLATION = 4
} yai_net_status_t;

#endif
