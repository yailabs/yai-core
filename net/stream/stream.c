#include "yai/net/stream.h"

const char *yai_net_stream_event_kind_name(yai_net_stream_event_kind_t value) {
    switch (value) {
    case YAI_NET_STREAM_EVENT_REQUEST:
        return "request";
    case YAI_NET_STREAM_EVENT_RESPONSE:
        return "response";
    case YAI_NET_STREAM_EVENT_CHUNK:
        return "chunk";
    case YAI_NET_STREAM_EVENT_METRIC:
        return "metric";
    case YAI_NET_STREAM_EVENT_RECEIPT:
        return "receipt";
    case YAI_NET_STREAM_EVENT_ERROR:
        return "error";
    case YAI_NET_STREAM_EVENT_COMPLETE:
        return "complete";
    default:
        return "unknown";
    }
}

int yai_net_stream_event_kind_is_valid(yai_net_stream_event_kind_t value) {
    return value >= YAI_NET_STREAM_EVENT_REQUEST && value <= YAI_NET_STREAM_EVENT_COMPLETE;
}
