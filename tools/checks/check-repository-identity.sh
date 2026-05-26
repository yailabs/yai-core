#!/bin/sh
# YAI - repository identity guard
#
# Purpose:
#   Prevent active surfaces from drifting back to old repository names.
#
# Scope:
#   Scans active source and docs roots, excluding historical archives.
#
# Non-goals:
#   Does not edit or rename repositories.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
HITS=/tmp/yai-repository-identity-hit
rm -f "$HITS"
OLD_CORE='yai''-core'
OLD_ENV='ai''-environment'
OLD_YAI='old''-yai'
OLD_YAI_TEXT='old'' yai'
OLD_TITLE='YAI ''Core'
OLD_TITLE_HYPHEN='YAI''-core'
OLD_TMP='/tmp/yai''-core'
PATTERN="$OLD_CORE|$OLD_ENV|$OLD_YAI|$OLD_YAI_TEXT|$OLD_TITLE|$OLD_TITLE_HYPHEN|$OLD_TMP"

for path in \
  README.md \
  Makefile \
  cmd \
  tools \
  docs/README.md \
  docs/adr \
  docs/architecture \
  docs/engineering \
  docs/internal \
  docs/labs \ \
  docs/product \
  docs/protocols \
  docs/status \
  include \
  system \
  proto \
  engine \
  tests \
  packaging \
  examples
do
  if [ -e "$ROOT/$path" ]; then
    grep -R -n -E \
      --exclude-dir=.git \
      --exclude-dir=build \
      --exclude-dir=target \
      --exclude-dir=docs/archive \
      --exclude-dir=repo-readiness \
      "$PATTERN" "$ROOT/$path" >>"$HITS" 2>/dev/null || true
  fi
done

if [ -s "$HITS" ]; then
  cat "$HITS" >&2
  printf 'active repository identity still references old names\n' >&2
  exit 1
fi

if ! grep -Fq 'Repository Identity Cutover' "$ROOT/work/spines/yai-spine.md"; then
  printf 'repository identity cutover missing from roadmap\n' >&2
  exit 1
fi

if ! grep -Fq 'yai-dev' "$ROOT/work/spines/yai-spine.md"; then
  printf 'yai-dev role missing from roadmap\n' >&2
  exit 1
fi

rm -f "$HITS"
printf 'check-repository-identity: ok\n'
