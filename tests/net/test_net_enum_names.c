#include "yai/net/capability.h"
#include "yai/net/endpoint.h"
#include "yai/net/health.h"
#include "yai/net/lifecycle.h"
#include "yai/net/net.h"
#include "yai/net/node.h"
#include "yai/net/stream.h"
#include "yai/net/transport.h"

#include <stdio.h>
#include <string.h>

static int expect_str(const char *label, const char *actual, const char *expected) {
    if (strcmp(actual, expected) != 0) {
        fprintf(stderr, "%s: expected %s got %s\n", label, expected, actual);
        return 1;
    }
    return 0;
}

static int expect_int(const char *label, int actual, int expected) {
    if (actual != expected) {
        fprintf(stderr, "%s: expected %d got %d\n", label, expected, actual);
        return 1;
    }
    return 0;
}

int main(void) {
    int failed = 0;

    failed |= expect_str("component_name", yai_net_component_name(), "net");
    failed |= expect_str("component_status", yai_net_component_status(), "c-translation-unit-scaffold");
    failed |= expect_str("component_version", yai_net_component_version(), "NET.SPINE.9C.0");

    failed |= expect_str("stream_request", yai_net_stream_event_kind_name(YAI_NET_STREAM_EVENT_REQUEST), "request");
    failed |= expect_str("stream_unknown", yai_net_stream_event_kind_name((yai_net_stream_event_kind_t)99), "unknown");
    failed |= expect_int("stream_valid", yai_net_stream_event_kind_is_valid(YAI_NET_STREAM_EVENT_COMPLETE), 1);
    failed |= expect_int("stream_invalid", yai_net_stream_event_kind_is_valid((yai_net_stream_event_kind_t)99), 0);

    failed |= expect_str("node_local", yai_net_node_kind_name(YAI_NET_NODE_KIND_LOCAL), "local");
    failed |= expect_str("node_unknown", yai_net_node_kind_name((yai_net_node_kind_t)99), "unknown");
    failed |= expect_int("node_valid", yai_net_node_kind_is_valid(YAI_NET_NODE_KIND_EXTERNAL), 1);
    failed |= expect_int("node_invalid", yai_net_node_kind_is_valid((yai_net_node_kind_t)99), 0);

    failed |= expect_str("capability_decode", yai_net_capability_kind_name(YAI_NET_CAPABILITY_KIND_NEURAL_LLM_DECODE), "neural_llm_decode");
    failed |= expect_str("capability_unknown", yai_net_capability_kind_name((yai_net_capability_kind_t)99), "unknown");
    failed |= expect_int("capability_valid", yai_net_capability_kind_is_valid(YAI_NET_CAPABILITY_KIND_GENERIC_EXTERNAL), 1);
    failed |= expect_int("capability_invalid", yai_net_capability_kind_is_valid((yai_net_capability_kind_t)99), 0);

    failed |= expect_str("endpoint_local_process", yai_net_endpoint_kind_name(YAI_NET_ENDPOINT_KIND_LOCAL_PROCESS), "local_process");
    failed |= expect_str("endpoint_unknown", yai_net_endpoint_kind_name((yai_net_endpoint_kind_t)99), "unknown");
    failed |= expect_int("endpoint_valid", yai_net_endpoint_kind_is_valid(YAI_NET_ENDPOINT_KIND_FUTURE_TRANSPORT), 1);
    failed |= expect_int("endpoint_invalid", yai_net_endpoint_kind_is_valid((yai_net_endpoint_kind_t)99), 0);

    failed |= expect_str("health_ready", yai_net_health_state_name(YAI_NET_HEALTH_STATE_READY), "ready");
    failed |= expect_str("health_unknown", yai_net_health_state_name((yai_net_health_state_t)99), "unknown");
    failed |= expect_int("health_valid", yai_net_health_state_is_valid(YAI_NET_HEALTH_STATE_FAILED), 1);
    failed |= expect_int("health_invalid", yai_net_health_state_is_valid((yai_net_health_state_t)99), 0);

    failed |= expect_str("lifecycle_running", yai_net_lifecycle_state_name(YAI_NET_LIFECYCLE_STATE_RUNNING), "running");
    failed |= expect_str("lifecycle_unknown", yai_net_lifecycle_state_name((yai_net_lifecycle_state_t)99), "unknown");
    failed |= expect_int("lifecycle_valid", yai_net_lifecycle_state_is_valid(YAI_NET_LIFECYCLE_STATE_RETIRED), 1);
    failed |= expect_int("lifecycle_invalid", yai_net_lifecycle_state_is_valid((yai_net_lifecycle_state_t)99), 0);

    failed |= expect_str("transport_localhost_http", yai_net_transport_kind_name(YAI_NET_TRANSPORT_KIND_LOCALHOST_HTTP), "localhost_http");
    failed |= expect_str("transport_unknown", yai_net_transport_kind_name((yai_net_transport_kind_t)99), "unknown");
    failed |= expect_int("transport_valid", yai_net_transport_kind_is_valid(YAI_NET_TRANSPORT_KIND_FUTURE), 1);
    failed |= expect_int("transport_invalid", yai_net_transport_kind_is_valid((yai_net_transport_kind_t)99), 0);

    if (failed != 0) {
        return 1;
    }

    printf("net_enum_names: ok\n");
    return 0;
}
