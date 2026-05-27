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
  include/yai/net/node.h
  include/yai/net/capability.h
  include/yai/net/endpoint.h
  include/yai/net/health.h
  include/yai/net/invocation.h
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

printf 'check-net-headers: ok\n'
