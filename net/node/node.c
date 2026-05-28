#include "yai/net/node.h"

const char *yai_net_node_kind_name(yai_net_node_kind_t value) {
    switch (value) {
    case YAI_NET_NODE_KIND_LOCAL:
        return "local";
    case YAI_NET_NODE_KIND_LOCALHOST_SERVICE:
        return "localhost_service";
    case YAI_NET_NODE_KIND_LAN:
        return "lan";
    case YAI_NET_NODE_KIND_REMOTE:
        return "remote";
    case YAI_NET_NODE_KIND_EXTERNAL:
        return "external";
    default:
        return "unknown";
    }
}

int yai_net_node_kind_is_valid(yai_net_node_kind_t value) {
    return value >= YAI_NET_NODE_KIND_LOCAL && value <= YAI_NET_NODE_KIND_EXTERNAL;
}
