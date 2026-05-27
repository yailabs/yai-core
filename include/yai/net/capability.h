/*
 * YAI - NET capability vocabulary
 *
 * Purpose:
 *   Names advertised external-node capability vocabulary.
 *
 * Ownership:
 *   Public NET capability kind, family, scope, state and advertisement v1
 *   vocabulary.
 *
 * Boundary:
 *   Capability names describe what a node can do; they do not approve use,
 *   mutate registries, route invocations, resolve policy, authorize work,
 *   register providers or vendor external projects.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_CAPABILITY_H
#define YAI_NET_CAPABILITY_H

#include <stdint.h>

typedef enum yai_net_capability_kind {
    YAI_NET_CAPABILITY_KIND_NEURAL_LLM_DECODE = 0,
    YAI_NET_CAPABILITY_KIND_NEURAL_EMBEDDING_ENCODE = 1,
    YAI_NET_CAPABILITY_KIND_METRICS_STREAM = 2,
    YAI_NET_CAPABILITY_KIND_RECEIPT_EMIT = 3,
    YAI_NET_CAPABILITY_KIND_GENERIC_EXTERNAL = 4
} yai_net_capability_kind_t;

typedef enum yai_net_capability_family {
    YAI_NET_CAPABILITY_FAMILY_NEURAL = 0,
    YAI_NET_CAPABILITY_FAMILY_METRICS = 1,
    YAI_NET_CAPABILITY_FAMILY_RECEIPT = 2,
    YAI_NET_CAPABILITY_FAMILY_GENERIC = 3
} yai_net_capability_family_t;

typedef enum yai_net_capability_scope {
    YAI_NET_CAPABILITY_SCOPE_PROCESS = 0,
    YAI_NET_CAPABILITY_SCOPE_NODE = 1,
    YAI_NET_CAPABILITY_SCOPE_SERVICE = 2,
    YAI_NET_CAPABILITY_SCOPE_EXTERNAL = 3
} yai_net_capability_scope_t;

typedef enum yai_net_capability_state {
    YAI_NET_CAPABILITY_STATE_UNKNOWN = 0,
    YAI_NET_CAPABILITY_STATE_DECLARED = 1,
    YAI_NET_CAPABILITY_STATE_AVAILABLE = 2,
    YAI_NET_CAPABILITY_STATE_DEGRADED = 3,
    YAI_NET_CAPABILITY_STATE_UNAVAILABLE = 4,
    YAI_NET_CAPABILITY_STATE_RETIRED = 5
} yai_net_capability_state_t;

typedef struct yai_net_capability_advertisement_v1 {
    const char *capability_id;
    const char *node_id;
    const char *capability_label;
    yai_net_capability_kind_t capability_kind;
    yai_net_capability_family_t capability_family;
    yai_net_capability_scope_t capability_scope;
    yai_net_capability_state_t capability_state;
    const char *constraints_json;
    const char *metadata_json;
    uint64_t advertised_at_unix_ms;
} yai_net_capability_advertisement_v1_t;

#endif
