#include "yai/net/endpoint.h"

const char *yai_net_endpoint_kind_name(yai_net_endpoint_kind_t value) {
    switch (value) {
    case YAI_NET_ENDPOINT_KIND_LOCAL_PROCESS:
        return "local_process";
    case YAI_NET_ENDPOINT_KIND_LOCALHOST_HTTP:
        return "localhost_http";
    case YAI_NET_ENDPOINT_KIND_LOCAL_IPC:
        return "local_ipc";
    case YAI_NET_ENDPOINT_KIND_LAN_HTTP:
        return "lan_http";
    case YAI_NET_ENDPOINT_KIND_REMOTE_HTTP:
        return "remote_http";
    case YAI_NET_ENDPOINT_KIND_FUTURE_TRANSPORT:
        return "future_transport";
    default:
        return "unknown";
    }
}

int yai_net_endpoint_kind_is_valid(yai_net_endpoint_kind_t value) {
    return value >= YAI_NET_ENDPOINT_KIND_LOCAL_PROCESS && value <= YAI_NET_ENDPOINT_KIND_FUTURE_TRANSPORT;
}
