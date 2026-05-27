/*
 * YAI - NET metric vocabulary
 *
 * Purpose:
 *   Names seed metrics for future NET observation.
 *
 * Ownership:
 *   Public NET metric name enum.
 *
 * Boundary:
 *   Metric names do not implement collection and do not decide control
 *   outcomes.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_METRICS_H
#define YAI_NET_METRICS_H

typedef enum yai_net_metric_name {
    YAI_NET_METRIC_DISCOVERY_MS = 0,
    YAI_NET_METRIC_CONNECT_MS = 1,
    YAI_NET_METRIC_HANDSHAKE_MS = 2,
    YAI_NET_METRIC_FIRST_CHUNK_MS = 3,
    YAI_NET_METRIC_STREAM_BYTES = 4,
    YAI_NET_METRIC_QUEUE_WAIT_MS = 5,
    YAI_NET_METRIC_TRANSPORT_ERROR_COUNT = 6
} yai_net_metric_name_t;

#endif
