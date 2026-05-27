#!/usr/bin/env bash
set -euo pipefail

# YAI - NET public header discipline guard
#
# Purpose:
#   Keep NET public headers vocabulary-only until runtime transport waves.
#
# Scope:
#   Checks required headers, include guards, public symbol prefixes and
#   forbidden internal or network implementation surfaces.
#
# Non-goals:
#   Does not compile NET or validate future transport behavior.

required_headers=(
  include/yai/net/net.h
  include/yai/net/version.h
  include/yai/net/error.h
  include/yai/net/ids.h
  include/yai/net/stream.h
  include/yai/net/transport.h
  include/yai/net/node.h
  include/yai/net/capability.h
  include/yai/net/endpoint.h
  include/yai/net/health.h
  include/yai/net/invocation.h
  include/yai/net/lifecycle.h
  include/yai/net/trace.h
  include/yai/net/receipt.h
  include/yai/net/metrics.h
)

strip_comments() {
  sed -E '
    /\/\*/,/\*\//d
    s://.*$::
  ' "$1"
}

for header in "${required_headers[@]}"; do
  if [[ ! -f "$header" ]]; then
    printf 'missing NET public header: %s\n' "$header" >&2
    exit 1
  fi

  base="$(basename "$header" .h)"
  guard="YAI_NET_$(printf '%s' "$base" | tr '[:lower:]' '[:upper:]')_H"

  if ! grep -Eq "^[[:space:]]*#ifndef[[:space:]]+$guard([[:space:]]|$)" "$header"; then
    printf 'missing NET include guard #ifndef %s in %s\n' "$guard" "$header" >&2
    exit 1
  fi

  if ! grep -Eq "^[[:space:]]*#define[[:space:]]+$guard([[:space:]]|$)" "$header"; then
    printf 'missing NET include guard #define %s in %s\n' "$guard" "$header" >&2
    exit 1
  fi

  stripped="$(strip_comments "$header")"

  if printf '%s\n' "$stripped" | grep -n -E '#[[:space:]]*include[[:space:]]+[<"]((system|engine)/|yai/(system|engine)/)' >&2; then
    printf 'NET header includes system/ or engine/ internal surface: %s\n' "$header" >&2
    exit 1
  fi

  if printf '%s\n' "$stripped" | grep -n -Ei '#[[:space:]]*include[[:space:]]+[<"]((sys/socket\.h)|(netinet/)|(arpa/inet\.h)|(winsock)|(curl/)|.*(http|server).*)' >&2; then
    printf 'NET header includes network/socket/server implementation surface: %s\n' "$header" >&2
    exit 1
  fi

  if printf '%s\n' "$stripped" | grep -n -E '\b(listen|accept|connect|bind|socket|discover|scan_lan|start_server)[[:space:]]*\(' >&2; then
    printf 'NET header declares forbidden runtime function: %s\n' "$header" >&2
    exit 1
  fi

  if printf '%s\n' "$stripped" | grep -n -E '^[[:space:]]*(typedef[[:space:]]+(enum|struct)[[:space:]]+|} )[A-Za-z_][A-Za-z0-9_]*' \
      | grep -v -E '\byai_net_[A-Za-z0-9_]*|YAI_NET_[A-Z0-9_]*' >&2; then
    printf 'NET header exposes public type without yai_net_ prefix: %s\n' "$header" >&2
    exit 1
  fi

  if printf '%s\n' "$stripped" | grep -n -E '^[[:space:]]*#[[:space:]]*define[[:space:]]+[A-Za-z_][A-Za-z0-9_]*' \
      | grep -v -E '\bYAI_NET_[A-Z0-9_]*\b' >&2; then
    printf 'NET header exposes macro without YAI_NET_ prefix: %s\n' "$header" >&2
    exit 1
  fi
done

stream_header="include/yai/net/stream.h"

for required_stream_symbol in \
  yai_net_stream_event_kind_t \
  yai_net_stream_state_t \
  yai_net_stream_payload_kind_t \
  YAI_NET_STREAM_EVENT_REQUEST \
  YAI_NET_STREAM_EVENT_RESPONSE \
  YAI_NET_STREAM_EVENT_CHUNK \
  YAI_NET_STREAM_EVENT_METRIC \
  YAI_NET_STREAM_EVENT_RECEIPT \
  YAI_NET_STREAM_EVENT_ERROR \
  YAI_NET_STREAM_EVENT_COMPLETE; do
  if ! grep -q "$required_stream_symbol" "$stream_header"; then
    printf 'NET stream header missing required symbol: %s\n' "$required_stream_symbol" >&2
    exit 1
  fi
done

if strip_comments "$stream_header" | grep -n -E '\b(send|receive|connect|listen|accept|bind|socket)[[:space:]]*\(' >&2; then
  printf 'NET stream header declares forbidden runtime function\n' >&2
  exit 1
fi

node_header="include/yai/net/node.h"

for required_node_symbol in \
  yai_net_node_kind_t \
  yai_net_node_scope_t \
  yai_net_node_state_t \
  YAI_NET_NODE_KIND_LOCAL \
  YAI_NET_NODE_KIND_LOCALHOST_SERVICE \
  YAI_NET_NODE_KIND_LAN \
  YAI_NET_NODE_KIND_REMOTE \
  YAI_NET_NODE_KIND_EXTERNAL \
  YAI_NET_NODE_SCOPE_PROCESS \
  YAI_NET_NODE_SCOPE_MACHINE \
  YAI_NET_NODE_SCOPE_LAN \
  YAI_NET_NODE_SCOPE_REMOTE \
  YAI_NET_NODE_SCOPE_EXTERNAL \
  YAI_NET_NODE_STATE_UNKNOWN \
  YAI_NET_NODE_STATE_DECLARED \
  YAI_NET_NODE_STATE_OBSERVED \
  YAI_NET_NODE_STATE_HEALTHY \
  YAI_NET_NODE_STATE_DEGRADED \
  YAI_NET_NODE_STATE_UNAVAILABLE \
  YAI_NET_NODE_STATE_RETIRED; do
  if ! grep -q "$required_node_symbol" "$node_header"; then
    printf 'NET node header missing required symbol: %s\n' "$required_node_symbol" >&2
    exit 1
  fi
done

if strip_comments "$node_header" | grep -n -E '\b(discover|scan|probe|connect|listen|accept|bind|socket)[[:space:]]*\(' >&2; then
  printf 'NET node header declares forbidden runtime function\n' >&2
  exit 1
fi

capability_header="include/yai/net/capability.h"

for required_capability_symbol in \
  yai_net_capability_kind_t \
  yai_net_capability_family_t \
  yai_net_capability_scope_t \
  yai_net_capability_state_t \
  YAI_NET_CAPABILITY_KIND_NEURAL_LLM_DECODE \
  YAI_NET_CAPABILITY_KIND_NEURAL_EMBEDDING_ENCODE \
  YAI_NET_CAPABILITY_KIND_METRICS_STREAM \
  YAI_NET_CAPABILITY_KIND_RECEIPT_EMIT \
  YAI_NET_CAPABILITY_KIND_GENERIC_EXTERNAL \
  YAI_NET_CAPABILITY_FAMILY_NEURAL \
  YAI_NET_CAPABILITY_FAMILY_METRICS \
  YAI_NET_CAPABILITY_FAMILY_RECEIPT \
  YAI_NET_CAPABILITY_FAMILY_GENERIC \
  YAI_NET_CAPABILITY_SCOPE_PROCESS \
  YAI_NET_CAPABILITY_SCOPE_NODE \
  YAI_NET_CAPABILITY_SCOPE_SERVICE \
  YAI_NET_CAPABILITY_SCOPE_EXTERNAL \
  YAI_NET_CAPABILITY_STATE_UNKNOWN \
  YAI_NET_CAPABILITY_STATE_DECLARED \
  YAI_NET_CAPABILITY_STATE_AVAILABLE \
  YAI_NET_CAPABILITY_STATE_DEGRADED \
  YAI_NET_CAPABILITY_STATE_UNAVAILABLE \
  YAI_NET_CAPABILITY_STATE_RETIRED; do
  if ! grep -q "$required_capability_symbol" "$capability_header"; then
    printf 'NET capability header missing required symbol: %s\n' "$required_capability_symbol" >&2
    exit 1
  fi
done

if strip_comments "$capability_header" | grep -n -E '\b(registry|register|discover|route|policy|authorize|connect|listen|accept|bind|socket)[[:space:]]*\(' >&2; then
  printf 'NET capability header declares forbidden runtime function\n' >&2
  exit 1
fi

endpoint_header="include/yai/net/endpoint.h"

for required_endpoint_symbol in \
  yai_net_endpoint_kind_t \
  yai_net_endpoint_scope_t \
  yai_net_endpoint_state_t \
  yai_net_endpoint_security_t \
  YAI_NET_ENDPOINT_KIND_LOCAL_PROCESS \
  YAI_NET_ENDPOINT_KIND_LOCALHOST_HTTP \
  YAI_NET_ENDPOINT_KIND_LOCAL_IPC \
  YAI_NET_ENDPOINT_KIND_LAN_HTTP \
  YAI_NET_ENDPOINT_KIND_REMOTE_HTTP \
  YAI_NET_ENDPOINT_KIND_FUTURE_TRANSPORT \
  YAI_NET_ENDPOINT_SCOPE_PROCESS \
  YAI_NET_ENDPOINT_SCOPE_MACHINE \
  YAI_NET_ENDPOINT_SCOPE_LAN \
  YAI_NET_ENDPOINT_SCOPE_REMOTE \
  YAI_NET_ENDPOINT_SCOPE_EXTERNAL \
  YAI_NET_ENDPOINT_STATE_UNKNOWN \
  YAI_NET_ENDPOINT_STATE_DECLARED \
  YAI_NET_ENDPOINT_STATE_AVAILABLE \
  YAI_NET_ENDPOINT_STATE_DEGRADED \
  YAI_NET_ENDPOINT_STATE_UNAVAILABLE \
  YAI_NET_ENDPOINT_STATE_RETIRED \
  YAI_NET_ENDPOINT_SECURITY_UNKNOWN \
  YAI_NET_ENDPOINT_SECURITY_LOCAL_ONLY \
  YAI_NET_ENDPOINT_SECURITY_TRUSTED_LOCAL \
  YAI_NET_ENDPOINT_SECURITY_DECLARED_REMOTE \
  YAI_NET_ENDPOINT_SECURITY_EXTERNAL_UNTRUSTED; do
  if ! grep -q "$required_endpoint_symbol" "$endpoint_header"; then
    printf 'NET endpoint header missing required symbol: %s\n' "$required_endpoint_symbol" >&2
    exit 1
  fi
done

if strip_comments "$endpoint_header" | grep -n -E '\b(registry|register|mutation|mutate|discovery|discover|route|authorize|connect|listen|accept|bind|socket|probe)[[:space:]]*\(' >&2; then
  printf 'NET endpoint header declares forbidden runtime function\n' >&2
  exit 1
fi

health_header="include/yai/net/health.h"

for required_health_symbol in \
  yai_net_health_state_t \
  yai_net_health_check_kind_t \
  yai_net_health_subject_kind_t \
  YAI_NET_HEALTH_STATE_UNKNOWN \
  YAI_NET_HEALTH_STATE_ALIVE \
  YAI_NET_HEALTH_STATE_READY \
  YAI_NET_HEALTH_STATE_DEGRADED \
  YAI_NET_HEALTH_STATE_UNAVAILABLE \
  YAI_NET_HEALTH_STATE_FAILED \
  YAI_NET_HEALTH_CHECK_KIND_HEALTH \
  YAI_NET_HEALTH_CHECK_KIND_LIVENESS \
  YAI_NET_HEALTH_CHECK_KIND_READINESS \
  YAI_NET_HEALTH_CHECK_KIND_CAPABILITY_READINESS \
  YAI_NET_HEALTH_CHECK_KIND_ENDPOINT_REACHABILITY \
  YAI_NET_HEALTH_SUBJECT_KIND_NODE \
  YAI_NET_HEALTH_SUBJECT_KIND_ENDPOINT \
  YAI_NET_HEALTH_SUBJECT_KIND_CAPABILITY \
  YAI_NET_HEALTH_SUBJECT_KIND_TRANSPORT \
  YAI_NET_HEALTH_SUBJECT_KIND_EXTERNAL; do
  if ! grep -q "$required_health_symbol" "$health_header"; then
    printf 'NET health header missing required symbol: %s\n' "$required_health_symbol" >&2
    exit 1
  fi
done

if strip_comments "$health_header" | grep -n -E '\b(probe|health_check_run|connect|listen|accept|bind|socket|http|route|authorize|discover)[[:space:]]*\(' >&2; then
  printf 'NET health header declares forbidden runtime function\n' >&2
  exit 1
fi

if ! grep -F '#include "yai/net/lifecycle.h"' include/yai/net/net.h >/dev/null; then
  printf 'NET umbrella header must include lifecycle.h\n' >&2
  exit 1
fi

lifecycle_header="include/yai/net/lifecycle.h"

for required_lifecycle_symbol in \
  yai_net_lifecycle_action_t \
  yai_net_lifecycle_state_t \
  yai_net_lifecycle_subject_kind_t \
  YAI_NET_LIFECYCLE_ACTION_DECLARE \
  YAI_NET_LIFECYCLE_ACTION_PREPARE \
  YAI_NET_LIFECYCLE_ACTION_START \
  YAI_NET_LIFECYCLE_ACTION_STOP \
  YAI_NET_LIFECYCLE_ACTION_RESTART \
  YAI_NET_LIFECYCLE_ACTION_STATUS \
  YAI_NET_LIFECYCLE_ACTION_RETIRE \
  YAI_NET_LIFECYCLE_STATE_UNKNOWN \
  YAI_NET_LIFECYCLE_STATE_DECLARED \
  YAI_NET_LIFECYCLE_STATE_PREPARING \
  YAI_NET_LIFECYCLE_STATE_STARTING \
  YAI_NET_LIFECYCLE_STATE_RUNNING \
  YAI_NET_LIFECYCLE_STATE_STOPPING \
  YAI_NET_LIFECYCLE_STATE_STOPPED \
  YAI_NET_LIFECYCLE_STATE_FAILED \
  YAI_NET_LIFECYCLE_STATE_RETIRED \
  YAI_NET_LIFECYCLE_SUBJECT_KIND_NODE \
  YAI_NET_LIFECYCLE_SUBJECT_KIND_ENDPOINT \
  YAI_NET_LIFECYCLE_SUBJECT_KIND_CAPABILITY \
  YAI_NET_LIFECYCLE_SUBJECT_KIND_SERVICE \
  YAI_NET_LIFECYCLE_SUBJECT_KIND_EXTERNAL; do
  if ! grep -q "$required_lifecycle_symbol" "$lifecycle_header"; then
    printf 'NET lifecycle header missing required symbol: %s\n' "$required_lifecycle_symbol" >&2
    exit 1
  fi
done

if strip_comments "$lifecycle_header" | grep -n -E '\b(start_service|stop_service|restart_service|spawn|exec|fork|kill|system|popen|connect|listen|accept|bind|socket|probe|route)[[:space:]]*\(' >&2; then
  printf 'NET lifecycle header declares forbidden runtime function\n' >&2
  exit 1
fi

if ! grep -F '#include "yai/net/transport.h"' include/yai/net/net.h >/dev/null; then
  printf 'NET umbrella header must include transport.h\n' >&2
  exit 1
fi

transport_header="include/yai/net/transport.h"

for required_transport_symbol in \
  yai_net_transport_kind_t \
  yai_net_transport_scope_t \
  yai_net_transport_state_t \
  yai_net_transport_security_t \
  yai_net_transport_adapter_descriptor_v1_t \
  YAI_NET_TRANSPORT_KIND_LOCALHOST_HTTP \
  YAI_NET_TRANSPORT_KIND_LOCAL_IPC \
  YAI_NET_TRANSPORT_KIND_LAN_HTTP \
  YAI_NET_TRANSPORT_KIND_REMOTE_HTTP \
  YAI_NET_TRANSPORT_KIND_FUTURE \
  YAI_NET_TRANSPORT_SCOPE_PROCESS \
  YAI_NET_TRANSPORT_SCOPE_MACHINE \
  YAI_NET_TRANSPORT_SCOPE_LAN \
  YAI_NET_TRANSPORT_SCOPE_REMOTE \
  YAI_NET_TRANSPORT_SCOPE_EXTERNAL \
  YAI_NET_TRANSPORT_STATE_UNKNOWN \
  YAI_NET_TRANSPORT_STATE_DECLARED \
  YAI_NET_TRANSPORT_STATE_AVAILABLE \
  YAI_NET_TRANSPORT_STATE_DEGRADED \
  YAI_NET_TRANSPORT_STATE_UNAVAILABLE \
  YAI_NET_TRANSPORT_STATE_RETIRED \
  YAI_NET_TRANSPORT_SECURITY_UNKNOWN \
  YAI_NET_TRANSPORT_SECURITY_LOCAL_ONLY \
  YAI_NET_TRANSPORT_SECURITY_TRUSTED_LOCAL \
  YAI_NET_TRANSPORT_SECURITY_DECLARED_REMOTE \
  YAI_NET_TRANSPORT_SECURITY_EXTERNAL_UNTRUSTED; do
  if ! grep -q "$required_transport_symbol" "$transport_header"; then
    printf 'NET transport header missing required symbol: %s\n' "$required_transport_symbol" >&2
    exit 1
  fi
done

if strip_comments "$transport_header" | grep -n -E '\b(connect|listen|accept|bind|socket|recv|send|http|request|probe|route|authorize)[[:space:]]*\(' >&2; then
  printf 'NET transport header declares forbidden runtime function\n' >&2
  exit 1
fi

for required_ipc_symbol in \
  yai_net_ipc_channel_kind_t \
  yai_net_ipc_channel_scope_t \
  yai_net_ipc_channel_state_t \
  yai_net_ipc_channel_security_t \
  yai_net_ipc_channel_descriptor_v1_t \
  YAI_NET_IPC_CHANNEL_KIND_ABSTRACT_LOCAL \
  YAI_NET_IPC_CHANNEL_KIND_UNIX_SOCKET_FUTURE \
  YAI_NET_IPC_CHANNEL_KIND_NAMED_PIPE_FUTURE \
  YAI_NET_IPC_CHANNEL_KIND_PLATFORM_IPC_FUTURE \
  YAI_NET_IPC_CHANNEL_KIND_MEMORY_CHANNEL_FUTURE \
  YAI_NET_IPC_CHANNEL_SCOPE_PROCESS \
  YAI_NET_IPC_CHANNEL_SCOPE_MACHINE \
  YAI_NET_IPC_CHANNEL_SCOPE_SESSION \
  YAI_NET_IPC_CHANNEL_SCOPE_FUTURE \
  YAI_NET_IPC_CHANNEL_STATE_UNKNOWN \
  YAI_NET_IPC_CHANNEL_STATE_DECLARED \
  YAI_NET_IPC_CHANNEL_STATE_AVAILABLE \
  YAI_NET_IPC_CHANNEL_STATE_DEGRADED \
  YAI_NET_IPC_CHANNEL_STATE_UNAVAILABLE \
  YAI_NET_IPC_CHANNEL_STATE_RETIRED \
  YAI_NET_IPC_CHANNEL_SECURITY_UNKNOWN \
  YAI_NET_IPC_CHANNEL_SECURITY_LOCAL_ONLY \
  YAI_NET_IPC_CHANNEL_SECURITY_SAME_USER_FUTURE \
  YAI_NET_IPC_CHANNEL_SECURITY_DECLARED_LOCAL \
  YAI_NET_IPC_CHANNEL_SECURITY_UNTRUSTED; do
  if ! grep -q "$required_ipc_symbol" "$transport_header"; then
    printf 'NET transport header missing required IPC symbol: %s\n' "$required_ipc_symbol" >&2
    exit 1
  fi
done

if strip_comments "$transport_header" | grep -n -E '\b(connect|listen|accept|bind|socket|recv|send|read|write|close|open|mkfifo|pipe|http|request|probe|route|authorize)[[:space:]]*\(' >&2; then
  printf 'NET transport header declares forbidden IPC/runtime function\n' >&2
  exit 1
fi

printf 'check-net-headers: ok\n'
