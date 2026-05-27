/*
 * YAI - NET capability vocabulary
 *
 * Purpose:
 *   Names advertised external-node capability kinds.
 *
 * Ownership:
 *   Public NET capability kind enum.
 *
 * Boundary:
 *   Capability names describe what a node can do; they do not approve use,
 *   register providers or vendor external projects.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_CAPABILITY_H
#define YAI_NET_CAPABILITY_H

typedef enum yai_net_capability_kind {
    YAI_NET_CAPABILITY_NEURAL_LLM_DECODE = 0,
    YAI_NET_CAPABILITY_NEURAL_EMBEDDING_ENCODE = 1,
    YAI_NET_CAPABILITY_METRICS_STREAM = 2,
    YAI_NET_CAPABILITY_RECEIPT_EMIT = 3,
    YAI_NET_CAPABILITY_GENERIC_EXTERNAL = 4
} yai_net_capability_kind_t;

#endif
