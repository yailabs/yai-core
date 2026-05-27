/*
 * YAI - NET transport adapter vocabulary
 *
 * Purpose:
 *   Names transport adapter descriptor vocabulary for future stream movement.
 *
 * Ownership:
 *   Public NET transport kind, scope, state, security and adapter descriptor
 *   v1 vocabulary.
 *
 * Boundary:
 *   Transport vocabulary describes declared adapter surfaces. It does not
 *   connect, send, receive, probe, route, authorize or perform HTTP.
 *
 * Status:
 *   scaffold
 */

#ifndef YAI_NET_TRANSPORT_H
#define YAI_NET_TRANSPORT_H

#include <stdint.h>

typedef enum yai_net_transport_kind {
    YAI_NET_TRANSPORT_KIND_LOCALHOST_HTTP = 0,
    YAI_NET_TRANSPORT_KIND_LOCAL_IPC = 1,
    YAI_NET_TRANSPORT_KIND_LAN_HTTP = 2,
    YAI_NET_TRANSPORT_KIND_REMOTE_HTTP = 3,
    YAI_NET_TRANSPORT_KIND_FUTURE = 4
} yai_net_transport_kind_t;

typedef enum yai_net_transport_scope {
    YAI_NET_TRANSPORT_SCOPE_PROCESS = 0,
    YAI_NET_TRANSPORT_SCOPE_MACHINE = 1,
    YAI_NET_TRANSPORT_SCOPE_LAN = 2,
    YAI_NET_TRANSPORT_SCOPE_REMOTE = 3,
    YAI_NET_TRANSPORT_SCOPE_EXTERNAL = 4
} yai_net_transport_scope_t;

typedef enum yai_net_transport_state {
    YAI_NET_TRANSPORT_STATE_UNKNOWN = 0,
    YAI_NET_TRANSPORT_STATE_DECLARED = 1,
    YAI_NET_TRANSPORT_STATE_AVAILABLE = 2,
    YAI_NET_TRANSPORT_STATE_DEGRADED = 3,
    YAI_NET_TRANSPORT_STATE_UNAVAILABLE = 4,
    YAI_NET_TRANSPORT_STATE_RETIRED = 5
} yai_net_transport_state_t;

typedef enum yai_net_transport_security {
    YAI_NET_TRANSPORT_SECURITY_UNKNOWN = 0,
    YAI_NET_TRANSPORT_SECURITY_LOCAL_ONLY = 1,
    YAI_NET_TRANSPORT_SECURITY_TRUSTED_LOCAL = 2,
    YAI_NET_TRANSPORT_SECURITY_DECLARED_REMOTE = 3,
    YAI_NET_TRANSPORT_SECURITY_EXTERNAL_UNTRUSTED = 4
} yai_net_transport_security_t;

typedef struct yai_net_transport_adapter_descriptor_v1 {
    const char *transport_adapter_id;
    const char *endpoint_id;
    const char *transport_label;
    yai_net_transport_kind_t transport_kind;
    yai_net_transport_scope_t transport_scope;
    yai_net_transport_state_t transport_state;
    yai_net_transport_security_t transport_security;
    const char *stream_profile;
    const char *metadata_json;
    uint64_t declared_at_unix_ms;
} yai_net_transport_adapter_descriptor_v1_t;

typedef enum yai_net_ipc_channel_kind {
    YAI_NET_IPC_CHANNEL_KIND_ABSTRACT_LOCAL = 0,
    YAI_NET_IPC_CHANNEL_KIND_UNIX_SOCKET_FUTURE = 1,
    YAI_NET_IPC_CHANNEL_KIND_NAMED_PIPE_FUTURE = 2,
    YAI_NET_IPC_CHANNEL_KIND_PLATFORM_IPC_FUTURE = 3,
    YAI_NET_IPC_CHANNEL_KIND_MEMORY_CHANNEL_FUTURE = 4
} yai_net_ipc_channel_kind_t;

typedef enum yai_net_ipc_channel_scope {
    YAI_NET_IPC_CHANNEL_SCOPE_PROCESS = 0,
    YAI_NET_IPC_CHANNEL_SCOPE_MACHINE = 1,
    YAI_NET_IPC_CHANNEL_SCOPE_SESSION = 2,
    YAI_NET_IPC_CHANNEL_SCOPE_FUTURE = 3
} yai_net_ipc_channel_scope_t;

typedef enum yai_net_ipc_channel_state {
    YAI_NET_IPC_CHANNEL_STATE_UNKNOWN = 0,
    YAI_NET_IPC_CHANNEL_STATE_DECLARED = 1,
    YAI_NET_IPC_CHANNEL_STATE_AVAILABLE = 2,
    YAI_NET_IPC_CHANNEL_STATE_DEGRADED = 3,
    YAI_NET_IPC_CHANNEL_STATE_UNAVAILABLE = 4,
    YAI_NET_IPC_CHANNEL_STATE_RETIRED = 5
} yai_net_ipc_channel_state_t;

typedef enum yai_net_ipc_channel_security {
    YAI_NET_IPC_CHANNEL_SECURITY_UNKNOWN = 0,
    YAI_NET_IPC_CHANNEL_SECURITY_LOCAL_ONLY = 1,
    YAI_NET_IPC_CHANNEL_SECURITY_SAME_USER_FUTURE = 2,
    YAI_NET_IPC_CHANNEL_SECURITY_DECLARED_LOCAL = 3,
    YAI_NET_IPC_CHANNEL_SECURITY_UNTRUSTED = 4
} yai_net_ipc_channel_security_t;

typedef struct yai_net_ipc_channel_descriptor_v1 {
    const char *ipc_channel_id;
    const char *transport_adapter_id;
    const char *endpoint_id;
    const char *channel_label;
    yai_net_ipc_channel_kind_t channel_kind;
    yai_net_ipc_channel_scope_t channel_scope;
    yai_net_ipc_channel_state_t channel_state;
    yai_net_ipc_channel_security_t channel_security;
    const char *channel_ref;
    const char *metadata_json;
    uint64_t declared_at_unix_ms;
} yai_net_ipc_channel_descriptor_v1_t;

#endif
