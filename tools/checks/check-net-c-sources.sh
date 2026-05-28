#!/usr/bin/env bash
set -euo pipefail

# YAI - NET C source guard
#
# Purpose:
#   Ensure NET C translation units exist only as pure vocabulary/helper
#   implementation in integrated module folders.
#
# Non-goals:
#   Does not validate future runtime transport behavior.

required_sources=(
  net/core/net.c
  net/stream/stream.c
  net/node/node.c
  net/capability/capability.c
  net/endpoint/endpoint.c
  net/health/health.c
  net/lifecycle/lifecycle.c
  net/transport/transport.c
)

if [[ -f include/yai/net/discovery.h ]]; then
  required_sources+=(net/discovery/discovery.c)
fi

for source in "${required_sources[@]}"; do
  if [[ ! -f "$source" ]]; then
    printf 'missing NET C source: %s\n' "$source" >&2
    exit 1
  fi
done

for forbidden_path in \
  net/src \
  net/docs \
  net/tests \
  net/fixtures \
  net/benches \
  net/examples \
  net/Makefile; do
  if [[ -e "$forbidden_path" ]]; then
    printf 'forbidden NET repository-shaped path: %s\n' "$forbidden_path" >&2
    exit 1
  fi
done

if find net -path 'net/*/*.c' -print -quit | grep -q .; then
  :
else
  printf 'NET must contain integrated C translation units under net/<area>/\n' >&2
  exit 1
fi

if grep -R -n -E '#[[:space:]]*include[[:space:]]+[<"]((system|engine)/|yai/(system|engine)/)' net --include='*.c' >/tmp/yai-net-c-internal-includes.$$ 2>/dev/null; then
  cat /tmp/yai-net-c-internal-includes.$$ >&2
  rm -f /tmp/yai-net-c-internal-includes.$$
  printf 'NET C sources must not include system/ or engine/ internals\n' >&2
  exit 1
fi
rm -f /tmp/yai-net-c-internal-includes.$$

if grep -R -n -E '#[[:space:]]*include[[:space:]]+[<"]((sys/socket\.h)|(netinet/)|(arpa/inet\.h)|(winsock)|(curl/))' net --include='*.c' >/tmp/yai-net-c-runtime-includes.$$ 2>/dev/null; then
  cat /tmp/yai-net-c-runtime-includes.$$ >&2
  rm -f /tmp/yai-net-c-runtime-includes.$$
  printf 'NET C sources must not include runtime network/socket headers\n' >&2
  exit 1
fi
rm -f /tmp/yai-net-c-runtime-includes.$$

if grep -R -n -E '\b(socket|connect|listen|accept|bind|recv|send|read|write|close|open|getaddrinfo|fork|exec|system|popen|kill|waitpid)[[:space:]]*\(' net --include='*.c' >/tmp/yai-net-c-runtime-symbols.$$ 2>/dev/null; then
  cat /tmp/yai-net-c-runtime-symbols.$$ >&2
  rm -f /tmp/yai-net-c-runtime-symbols.$$
  printf 'NET C sources must not contain runtime/network/process implementation calls\n' >&2
  exit 1
fi
rm -f /tmp/yai-net-c-runtime-symbols.$$

if grep -R -n -i -E '\b(curl|http request|mdns|dns_sd|broadcast)\b' net --include='*.c' >/tmp/yai-net-c-runtime-terms.$$ 2>/dev/null; then
  cat /tmp/yai-net-c-runtime-terms.$$ >&2
  rm -f /tmp/yai-net-c-runtime-terms.$$
  printf 'NET C sources must not contain runtime/discovery implementation terms\n' >&2
  exit 1
fi
rm -f /tmp/yai-net-c-runtime-terms.$$

printf 'check-net-c-sources: ok\n'
