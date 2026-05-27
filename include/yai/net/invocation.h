/*
 * YAI - NET invocation vocabulary
 *
 * Purpose:
 *   Names future invocation lifecycle states through NET surfaces.
 *
 * Ownership:
 *   Public NET invocation state enum.
 *
 * Boundary:
 *   Invocation state does not implement routing and is not a YAI authority
 *   decision.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_INVOCATION_H
#define YAI_NET_INVOCATION_H

typedef enum yai_net_invocation_state {
    YAI_NET_INVOCATION_CREATED = 0,
    YAI_NET_INVOCATION_ROUTED = 1,
    YAI_NET_INVOCATION_STREAMING = 2,
    YAI_NET_INVOCATION_COMPLETED = 3,
    YAI_NET_INVOCATION_FAILED = 4,
    YAI_NET_INVOCATION_CANCELLED = 5
} yai_net_invocation_state_t;

#endif
