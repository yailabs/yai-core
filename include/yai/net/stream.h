/*
 * YAI - NET stream vocabulary
 *
 * Purpose:
 *   Names stream envelope vocabulary moved through future NET runtime surfaces.
 *
 * Ownership:
 *   Public NET stream event, state, payload and envelope v1 vocabulary.
 *
 * Boundary:
 *   Stream envelopes move event material; they do not approve invocations,
 *   allocate payloads, serialize data or implement live streaming.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_STREAM_H
#define YAI_NET_STREAM_H

#include <stdint.h>

typedef enum yai_net_stream_event_kind {
    YAI_NET_STREAM_EVENT_REQUEST = 0,
    YAI_NET_STREAM_EVENT_RESPONSE = 1,
    YAI_NET_STREAM_EVENT_CHUNK = 2,
    YAI_NET_STREAM_EVENT_METRIC = 3,
    YAI_NET_STREAM_EVENT_RECEIPT = 4,
    YAI_NET_STREAM_EVENT_ERROR = 5,
    YAI_NET_STREAM_EVENT_COMPLETE = 6
} yai_net_stream_event_kind_t;

typedef enum yai_net_stream_state {
    YAI_NET_STREAM_STATE_CREATED = 0,
    YAI_NET_STREAM_STATE_OPEN = 1,
    YAI_NET_STREAM_STATE_HALF_CLOSED = 2,
    YAI_NET_STREAM_STATE_COMPLETED = 3,
    YAI_NET_STREAM_STATE_FAILED = 4,
    YAI_NET_STREAM_STATE_CANCELLED = 5
} yai_net_stream_state_t;

typedef enum yai_net_stream_payload_kind {
    YAI_NET_STREAM_PAYLOAD_NONE = 0,
    YAI_NET_STREAM_PAYLOAD_BYTES = 1,
    YAI_NET_STREAM_PAYLOAD_TEXT = 2,
    YAI_NET_STREAM_PAYLOAD_JSON = 3,
    YAI_NET_STREAM_PAYLOAD_METRIC = 4,
    YAI_NET_STREAM_PAYLOAD_RECEIPT = 5,
    YAI_NET_STREAM_PAYLOAD_ERROR = 6
} yai_net_stream_payload_kind_t;

typedef struct yai_net_stream_envelope_v1 {
    const char *stream_id;
    const char *invocation_id;
    const char *trace_id;
    const char *origin_node_id;
    const char *target_node_id;
    uint64_t sequence;
    yai_net_stream_event_kind_t event_kind;
    yai_net_stream_state_t stream_state;
    yai_net_stream_payload_kind_t payload_kind;
    const void *payload;
    uint64_t payload_size;
    uint64_t timestamp_unix_ms;
} yai_net_stream_envelope_v1_t;

const char *yai_net_stream_event_kind_name(yai_net_stream_event_kind_t value);
int yai_net_stream_event_kind_is_valid(yai_net_stream_event_kind_t value);

#endif
