/*
 * YAI - NET stream vocabulary
 *
 * Purpose:
 *   Names stream event kinds moved through future NET runtime surfaces.
 *
 * Ownership:
 *   Public NET stream event kind enum.
 *
 * Boundary:
 *   Stream events move data; they do not approve invocations or implement live
 *   streaming.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_STREAM_H
#define YAI_NET_STREAM_H

typedef enum yai_net_stream_event_kind {
    YAI_NET_STREAM_EVENT_REQUEST = 0,
    YAI_NET_STREAM_EVENT_RESPONSE = 1,
    YAI_NET_STREAM_EVENT_CHUNK = 2,
    YAI_NET_STREAM_EVENT_METRIC = 3,
    YAI_NET_STREAM_EVENT_RECEIPT = 4,
    YAI_NET_STREAM_EVENT_ERROR = 5,
    YAI_NET_STREAM_EVENT_COMPLETE = 6
} yai_net_stream_event_kind_t;

#endif
