#!/usr/bin/env bash
set -euo pipefail

# YAI - NET boundary guard
#
# Purpose:
#   Keep NET as an integrated YAI runtime module, not a repository-shaped
#   subtree or authority/execution owner.
#
# Scope:
#   Checks NET integrated topology, public headers, work-owned spine/snapshots
#   and forbidden implementation signals.
#
# Non-goals:
#   Does not compile NET or validate runtime transport behavior.
#
require_file() {
  if [[ ! -f "$1" ]]; then
    printf 'missing NET boundary file: %s\n' "$1" >&2
    exit 1
  fi
}

require_dir() {
  if [[ ! -d "$1" ]]; then
    printf 'missing NET boundary dir: %s\n' "$1" >&2
    exit 1
  fi
}

forbidden_path() {
  if [[ -e "$1" ]]; then
    printf 'forbidden NET repository-shaped path: %s\n' "$1" >&2
    exit 1
  fi
}

require_phrase() {
  local corpus="$1"
  local phrase="$2"
  if [[ "$corpus" != *"$phrase"* ]]; then
    printf 'missing NET boundary phrase: %s\n' "$phrase" >&2
    exit 1
  fi
}

require_dir "net"
require_file "net/README.md"
require_file "work/protocols/net.md"

for area in core registry stream node capability endpoint health transport route observe compat; do
  require_dir "net/$area"
  require_file "net/$area/README.md"
done

for path in \
  include/yai/net/net.h \
  include/yai/net/version.h \
  include/yai/net/error.h \
  include/yai/net/ids.h \
  include/yai/net/stream.h \
  include/yai/net/node.h \
  include/yai/net/capability.h \
  include/yai/net/endpoint.h \
  include/yai/net/health.h \
  include/yai/net/invocation.h \
  include/yai/net/trace.h \
  include/yai/net/receipt.h \
  include/yai/net/metrics.h \
  work/spines/net-spine.md; do
  require_file "$path"
done

if ! grep -Fx 'Reference version: NET.SPINE.1.0' work/spines/net-spine.md >/dev/null; then
  printf 'work/spines/net-spine.md must declare Reference version: NET.SPINE.1.0\n' >&2
  exit 1
fi

for path in \
  net/docs \
  net/src \
  net/tests \
  net/fixtures \
  net/benches \
  net/examples \
  net/Makefile \
  clori \
  third_party/clori \
  vendor/clori \
  external/clori \
  docs/""spines/net-spine.md \
  docs/archive/engineering/waves/net-spine-0-root-component-scaffold-boundary-guard.md \
  docs/""engineering/net-yai-boundary.md \
  docs/""engineering/external-clori-boundary.md; do
  forbidden_path "$path"
done

corpus="$(cat \
  net/README.md \
  work/protocols/net.md \
  work/spines/net-spine.md \
  work/archive/architecture-snapshots/net-root-substrate.md \
  work/archive/engineering-snapshots/net-yai-boundary.md \
  work/archive/engineering-snapshots/external-clori-boundary.md \
  | tr '\n' ' ')"

for phrase in \
  "YAI controls authority." \
  "NET moves streams." \
  "External nodes execute capabilities." \
  "CLORI executes neural computation."; do
  require_phrase "$corpus" "$phrase"
done

for forbidden in \
  "NET owns case authority" \
  "NET owns policy resolution" \
  "NET executes neural computation" \
  "NET loads models" \
  "NET mutates RuntimeGraph" \
  "NET writes YAI journal" \
  "NET writes YAI memory" \
  "CLORI is inside YAI" \
  "CLORI is vendored into YAI"; do
  if [[ "$corpus" == *"$forbidden"* ]]; then
    printf 'forbidden NET boundary claim: %s\n' "$forbidden" >&2
    exit 1
  fi
done

if grep -R -n -E '#[[:space:]]*include[[:space:]]+[<"]((system|engine)/|yai/(system|engine)/)' net include/yai/net >/tmp/yai-net-boundary-includes.$$ 2>/dev/null; then
  if ! grep -F 'NET_INTERNAL_INCLUDE_ALLOWED' /tmp/yai-net-boundary-includes.$$ >/dev/null; then
    cat /tmp/yai-net-boundary-includes.$$ >&2
    rm -f /tmp/yai-net-boundary-includes.$$
    printf 'NET.SPINE.0R must not include system/ or engine/ internals\n' >&2
    exit 1
  fi
fi
rm -f /tmp/yai-net-boundary-includes.$$

if grep -R -n -E '\b(socket|connect|listen|accept|bind|getaddrinfo)[[:space:]]*\(' net include/yai/net >/tmp/yai-net-boundary-network.$$ 2>/dev/null; then
  cat /tmp/yai-net-boundary-network.$$ >&2
  rm -f /tmp/yai-net-boundary-network.$$
  printf 'NET.SPINE.0R must not contain network implementation symbols\n' >&2
  exit 1
fi
rm -f /tmp/yai-net-boundary-network.$$

if grep -R -n -i 'HTTP server implementation' net include/yai/net >/tmp/yai-net-boundary-http.$$ 2>/dev/null; then
  cat /tmp/yai-net-boundary-http.$$ >&2
  rm -f /tmp/yai-net-boundary-http.$$
  printf 'NET.SPINE.0R must not claim HTTP server implementation\n' >&2
  exit 1
fi
rm -f /tmp/yai-net-boundary-http.$$

printf 'check-net-boundary: ok\n'
