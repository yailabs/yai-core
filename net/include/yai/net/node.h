#ifndef YAI_NET_NODE_H
#define YAI_NET_NODE_H

typedef enum yai_net_node_kind {
    YAI_NET_NODE_LOCAL = 0,
    YAI_NET_NODE_LOCALHOST_SERVICE = 1,
    YAI_NET_NODE_LAN = 2,
    YAI_NET_NODE_REMOTE = 3,
    YAI_NET_NODE_EXTERNAL = 4
} yai_net_node_kind_t;

#endif
