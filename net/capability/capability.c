#include "yai/net/capability.h"

const char *yai_net_capability_kind_name(yai_net_capability_kind_t value) {
    switch (value) {
    case YAI_NET_CAPABILITY_KIND_NEURAL_LLM_DECODE:
        return "neural_llm_decode";
    case YAI_NET_CAPABILITY_KIND_NEURAL_EMBEDDING_ENCODE:
        return "neural_embedding_encode";
    case YAI_NET_CAPABILITY_KIND_METRICS_STREAM:
        return "metrics_stream";
    case YAI_NET_CAPABILITY_KIND_RECEIPT_EMIT:
        return "receipt_emit";
    case YAI_NET_CAPABILITY_KIND_GENERIC_EXTERNAL:
        return "generic_external";
    default:
        return "unknown";
    }
}

int yai_net_capability_kind_is_valid(yai_net_capability_kind_t value) {
    return value >= YAI_NET_CAPABILITY_KIND_NEURAL_LLM_DECODE && value <= YAI_NET_CAPABILITY_KIND_GENERIC_EXTERNAL;
}
