#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

for file in \
  docs/architecture/00-spine.md \
  docs/architecture/01-terminology.md \
  docs/architecture/04-subject-model.md \
  docs/architecture/06-control-policy-model.md \
  docs/engineering/target-layout.md \
  docs/engineering/new1-minimum-loop.md \
  docs/engineering/new2-persistent-journal.md \
  docs/engineering/new3-control-gate.md \
  docs/engineering/new4-filesystem-carrier.md \
  docs/engineering/new5-graph-reconstruction.md \
  docs/engineering/new6-operational-memory.md \
  docs/engineering/new7-reconcile-divergence.md \
  docs/engineering/new8-projection-hardening.md \
  docs/engineering/new9-query-boundary.md \
  docs/engineering/new10-rust-engine-r1.md \
  docs/engineering/new11-daemon-ipc.md \
  docs/product/positioning.md \
  docs/adr/0000-decision-compendium.md
do
  if [ ! -f "$ROOT/$file" ]; then
    printf 'required doc missing: %s\n' "$file" >&2
    exit 1
  fi
done

printf 'check-doc-required-files: ok\n'
