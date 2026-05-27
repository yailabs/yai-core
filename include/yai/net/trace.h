/*
 * YAI - NET trace vocabulary
 *
 * Purpose:
 *   Names correlation boundaries for future NET trace propagation.
 *
 * Ownership:
 *   Public NET trace boundary enum.
 *
 * Boundary:
 *   Trace vocabulary does not implement a tracing backend or authority log.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_TRACE_H
#define YAI_NET_TRACE_H

typedef enum yai_net_trace_boundary {
    YAI_NET_TRACE_BOUNDARY_INVOCATION = 0,
    YAI_NET_TRACE_BOUNDARY_ROUTE = 1,
    YAI_NET_TRACE_BOUNDARY_TRANSPORT = 2,
    YAI_NET_TRACE_BOUNDARY_STREAM = 3,
    YAI_NET_TRACE_BOUNDARY_RECEIPT = 4
} yai_net_trace_boundary_t;

#endif
