#!/bin/sh
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

for file in \
  docs/architecture/00-spine.md \
  docs/architecture/15-pack-materialization.md \
  docs/architecture/16-hot-state-plane.md \
  docs/architecture/01-terminology.md \
  docs/architecture/04-subject-model.md \
  docs/architecture/06-control-policy-model.md \
  docs/engineering/README.md \
  docs/engineering/current-status.md \
  docs/engineering/command-surface.md \
  docs/engineering/data-plane-roadmap.md \
  docs/engineering/extraction-plan.md \
  docs/engineering/filesystem-target.md \
  docs/engineering/four-repo-roadmap.md \
  docs/engineering/hot-state-plane.md \
  docs/engineering/operational-extraction-contract.md \
  docs/engineering/pack-format.md \
  docs/engineering/pack-roadmap.md \
  docs/engineering/testing.md \
  docs/engineering/wave-template.md \
  docs/manuals/manual-command-surface-validation.md \
  docs/manuals/manual-command-surface-validation.ipynb \
  docs/manuals/README.md \
  docs/adr/0017-packs-as-case-materialization-units.md \
  docs/adr/0018-hot-state-is-not-truth.md \
  docs/product/positioning.md \
  docs/adr/0000-decision-compendium.md
do
  if [ ! -f "$ROOT/$file" ]; then
    printf 'required doc missing: %s\n' "$file" >&2
    exit 1
  fi
done

printf 'check-doc-required-files: ok\n'
