/*
 * YAI - NET node vocabulary
 *
 * Purpose:
 *   Names node identity and projection vocabulary for NET participants.
 *
 * Ownership:
 *   Public NET node kind, scope, state and projection v1 vocabulary.
 *
 * Boundary:
 *   Node vocabulary does not discover LAN nodes, probe machines, mutate
 *   registries, grant trust or assign YAI authority.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_NODE_H
#define YAI_NET_NODE_H

#include <stdint.h>

typedef enum yai_net_node_kind {
    YAI_NET_NODE_KIND_LOCAL = 0,
    YAI_NET_NODE_KIND_LOCALHOST_SERVICE = 1,
    YAI_NET_NODE_KIND_LAN = 2,
    YAI_NET_NODE_KIND_REMOTE = 3,
    YAI_NET_NODE_KIND_EXTERNAL = 4
} yai_net_node_kind_t;

typedef enum yai_net_node_scope {
    YAI_NET_NODE_SCOPE_PROCESS = 0,
    YAI_NET_NODE_SCOPE_MACHINE = 1,
    YAI_NET_NODE_SCOPE_LAN = 2,
    YAI_NET_NODE_SCOPE_REMOTE = 3,
    YAI_NET_NODE_SCOPE_EXTERNAL = 4
} yai_net_node_scope_t;

typedef enum yai_net_node_state {
    YAI_NET_NODE_STATE_UNKNOWN = 0,
    YAI_NET_NODE_STATE_DECLARED = 1,
    YAI_NET_NODE_STATE_OBSERVED = 2,
    YAI_NET_NODE_STATE_HEALTHY = 3,
    YAI_NET_NODE_STATE_DEGRADED = 4,
    YAI_NET_NODE_STATE_UNAVAILABLE = 5,
    YAI_NET_NODE_STATE_RETIRED = 6
} yai_net_node_state_t;

typedef struct yai_net_node_projection_v1 {
    const char *node_id;
    const char *node_label;
    yai_net_node_kind_t node_kind;
    yai_net_node_scope_t node_scope;
    yai_net_node_state_t node_state;
    const char *identity_source;
    const char *safe_metadata_json;
    uint64_t observed_at_unix_ms;
} yai_net_node_projection_v1_t;

const char *yai_net_node_kind_name(yai_net_node_kind_t value);
int yai_net_node_kind_is_valid(yai_net_node_kind_t value);

#endif
