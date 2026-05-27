/*
 * YAI - NET endpoint vocabulary
 *
 * Purpose:
 *   Names endpoint descriptor vocabulary for future NET communication paths.
 *
 * Ownership:
 *   Public NET endpoint kind, scope, state, security and descriptor v1
 *   vocabulary.
 *
 * Boundary:
 *   Endpoint vocabulary describes declared address surfaces. It does not
 *   connect, listen, bind, discover, probe, route or approve operations.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_ENDPOINT_H
#define YAI_NET_ENDPOINT_H

#include <stdint.h>

typedef enum yai_net_endpoint_kind {
    YAI_NET_ENDPOINT_KIND_LOCAL_PROCESS = 0,
    YAI_NET_ENDPOINT_KIND_LOCALHOST_HTTP = 1,
    YAI_NET_ENDPOINT_KIND_LOCAL_IPC = 2,
    YAI_NET_ENDPOINT_KIND_LAN_HTTP = 3,
    YAI_NET_ENDPOINT_KIND_REMOTE_HTTP = 4,
    YAI_NET_ENDPOINT_KIND_FUTURE_TRANSPORT = 5
} yai_net_endpoint_kind_t;

typedef enum yai_net_endpoint_scope {
    YAI_NET_ENDPOINT_SCOPE_PROCESS = 0,
    YAI_NET_ENDPOINT_SCOPE_MACHINE = 1,
    YAI_NET_ENDPOINT_SCOPE_LAN = 2,
    YAI_NET_ENDPOINT_SCOPE_REMOTE = 3,
    YAI_NET_ENDPOINT_SCOPE_EXTERNAL = 4
} yai_net_endpoint_scope_t;

typedef enum yai_net_endpoint_state {
    YAI_NET_ENDPOINT_STATE_UNKNOWN = 0,
    YAI_NET_ENDPOINT_STATE_DECLARED = 1,
    YAI_NET_ENDPOINT_STATE_AVAILABLE = 2,
    YAI_NET_ENDPOINT_STATE_DEGRADED = 3,
    YAI_NET_ENDPOINT_STATE_UNAVAILABLE = 4,
    YAI_NET_ENDPOINT_STATE_RETIRED = 5
} yai_net_endpoint_state_t;

typedef enum yai_net_endpoint_security {
    YAI_NET_ENDPOINT_SECURITY_UNKNOWN = 0,
    YAI_NET_ENDPOINT_SECURITY_LOCAL_ONLY = 1,
    YAI_NET_ENDPOINT_SECURITY_TRUSTED_LOCAL = 2,
    YAI_NET_ENDPOINT_SECURITY_DECLARED_REMOTE = 3,
    YAI_NET_ENDPOINT_SECURITY_EXTERNAL_UNTRUSTED = 4
} yai_net_endpoint_security_t;

typedef struct yai_net_endpoint_descriptor_v1 {
    const char *endpoint_id;
    const char *node_id;
    const char *endpoint_label;
    yai_net_endpoint_kind_t endpoint_kind;
    yai_net_endpoint_scope_t endpoint_scope;
    yai_net_endpoint_state_t endpoint_state;
    yai_net_endpoint_security_t endpoint_security;
    const char *address_ref;
    const char *metadata_json;
    uint64_t declared_at_unix_ms;
} yai_net_endpoint_descriptor_v1_t;

#endif
