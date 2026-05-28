#include "yai/net/transport.h"

const char *yai_net_transport_kind_name(yai_net_transport_kind_t value) {
    switch (value) {
    case YAI_NET_TRANSPORT_KIND_LOCALHOST_HTTP:
        return "localhost_http";
    case YAI_NET_TRANSPORT_KIND_LOCAL_IPC:
        return "local_ipc";
    case YAI_NET_TRANSPORT_KIND_LAN_HTTP:
        return "lan_http";
    case YAI_NET_TRANSPORT_KIND_REMOTE_HTTP:
        return "remote_http";
    case YAI_NET_TRANSPORT_KIND_FUTURE:
        return "future";
    default:
        return "unknown";
    }
}

int yai_net_transport_kind_is_valid(yai_net_transport_kind_t value) {
    return value >= YAI_NET_TRANSPORT_KIND_LOCALHOST_HTTP && value <= YAI_NET_TRANSPORT_KIND_FUTURE;
}
