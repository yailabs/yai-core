#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
BANNER='Historical/superseded engineering record. Not an active source of truth.'
HITS=/tmp/yai-archive-banner-missing
rm -f "$HITS"

find "$ROOT/docs/archive" -type f -name '*.md' | sort | while IFS= read -r file; do
  first_line=$(sed -n '1p' "$file")
  if [ "$first_line" != "$BANNER" ]; then
    printf '%s\n' "$file" >>"$HITS"
  fi
done

if [ -s "$HITS" ]; then
  cat "$HITS" >&2
  printf 'archive markdown file missing historical banner\n' >&2
  exit 1
fi

if [ ! -f "$ROOT/docs/archive/README.md" ]; then
  printf 'archive README missing\n' >&2
  exit 1
fi

printf 'check-archive-historical-records: ok\n'
