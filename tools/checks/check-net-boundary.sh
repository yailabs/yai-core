#!/usr/bin/env bash
set -euo pipefail

# YAI - NET boundary guard
#
# Purpose:
#   Keep NET.SPINE.0 focused on a root communication substrate scaffold without
#   authority, transport or external execution overclaims.
#
# Scope:
#   Checks NET component layout, docs, public vocabulary headers and forbidden
#   dependency/implementation signals.
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

require_phrase() {
  local corpus="$1"
  local phrase="$2"
  if [[ "$corpus" != *"$phrase"* ]]; then
    printf 'missing NET boundary phrase: %s\n' "$phrase" >&2
    exit 1
  fi
}

for path in \
  net \
  net/docs \
  net/include/yai/net \
  net/src; do
  require_dir "$path"
done

for path in \
  net/README.md \
  net/docs/boundary.md \
  net/docs/architecture.md \
  net/docs/interfaces-transport-boundary.md \
  net/docs/clori-node-boundary.md \
  net/include/yai/net/net.h \
  work/spines/net-spine.md; do
  require_file "$path"
done

corpus="$(cat net/README.md net/docs/*.md work/spines/net-spine.md | tr '\n' ' ')"

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

if grep -R -n -E '#[[:space:]]*include[[:space:]]+[<"]((system|engine)/|yai/(system|engine)/)' net/include net/src >/tmp/yai-net-boundary-includes.$$ 2>/dev/null; then
  if ! grep -F 'NET_INTERNAL_INCLUDE_ALLOWED' /tmp/yai-net-boundary-includes.$$ >/dev/null; then
    cat /tmp/yai-net-boundary-includes.$$ >&2
    rm -f /tmp/yai-net-boundary-includes.$$
    printf 'NET.SPINE.0 must not include system/ or engine/ internals\n' >&2
    exit 1
  fi
fi
rm -f /tmp/yai-net-boundary-includes.$$

if grep -R -n -E '\b(socket|connect|listen|accept)\b' net/include net/src >/tmp/yai-net-boundary-network.$$ 2>/dev/null; then
  cat /tmp/yai-net-boundary-network.$$ >&2
  rm -f /tmp/yai-net-boundary-network.$$
  printf 'NET.SPINE.0 must not contain network implementation symbols in source/header files\n' >&2
  exit 1
fi
rm -f /tmp/yai-net-boundary-network.$$

printf 'check-net-boundary: ok\n'
