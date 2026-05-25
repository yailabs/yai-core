#!/bin/sh
# YAI - required docs guard
#
# Purpose:
#   Ensure the active documentation set needed by current waves exists.
#
# Scope:
#   Checks required active docs, manuals and ADR files.
#
# Non-goals:
#   Does not validate every doc phrase.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

for file in \
  docs/architecture/00-spine.md \
  docs/architecture/15-pack-materialization.md \
  docs/architecture/16-hot-state-plane.md \
  docs/architecture/18-control-carrier-substrate.md \
  docs/architecture/19-operation-dispatch-multiplex.md \
  docs/architecture/20-carrier-contract-v1.md \
  docs/architecture/21-process-carrier-signal-control.md \
  docs/architecture/22-host-observation-probe.md \
  docs/architecture/23-carrier-coverage-matrix.md \
  docs/architecture/24-non-process-carrier-skeletons.md \
  docs/architecture/25-carrier-outcome-harness.md \
  docs/architecture/26-context-compiler-retrieval-boundary.md \
  docs/architecture/27-model-runtime-mtp-boundary.md \
  docs/architecture/28-provider-runtime-lan-target-surface.md \
  docs/architecture/01-terminology.md \
  docs/architecture/04-subject-model.md \
  docs/architecture/06-control-policy-model.md \
  docs/engineering/README.md \
  docs/engineering/agent-operating-appendix.md \
  docs/engineering/current-status.md \
  docs/engineering/command-surface.md \
  docs/engineering/carrier-contract-v1.md \
  docs/engineering/control-carrier-rebase.md \
  docs/engineering/operation-dispatch-multiplex.md \
  docs/engineering/process-carrier-signal-control.md \
  docs/engineering/host-observation-probe.md \
  docs/engineering/carrier-coverage-matrix.md \
  docs/engineering/data-plane-roadmap.md \
  docs/engineering/extraction-plan.md \
  docs/engineering/file-header-standard.md \
  docs/engineering/filesystem-target.md \
  docs/engineering/four-repo-roadmap.md \
  docs/engineering/hot-state-plane.md \
  docs/engineering/carrier-outcome-harness.md \
  docs/engineering/non-process-carrier-skeletons.md \
  docs/engineering/provider-runtime-lan-target-surface.md \
  docs/engineering/operational-extraction-contract.md \
  docs/engineering/pack-format.md \
  docs/engineering/pack-roadmap.md \
  docs/engineering/source-surface.md \
  docs/engineering/testing.md \
  docs/engineering/wave-template.md \
  docs/manuals/README.md \
  docs/manuals/manual-filesystem-loop-validation.md \
  docs/manuals/manual-filesystem-loop-validation.ipynb \
  docs/manuals/manual-filesystem-loop-validation.it.ipynb \
  docs/adr/0017-packs-as-case-materialization-units.md \
  docs/adr/0018-hot-state-is-not-truth.md \
  docs/adr/0020-control-carrier-substrate.md \
  docs/adr/0021-operation-dispatch-multiplex.md \
  docs/adr/0022-carrier-contract-v1.md \
  docs/adr/0023-process-carrier-signal-control.md \
  docs/adr/0024-host-observation-probe.md \
  docs/adr/0025-carrier-coverage-matrix.md \
  docs/adr/0026-non-process-carrier-skeletons.md \
  docs/adr/0027-carrier-outcome-harness.md \
  docs/adr/0028-provider-runtime-lan-target-surface.md \
  docs/product/positioning.md \
  docs/adr/0000-decision-compendium.md
do
  if [ ! -f "$ROOT/$file" ]; then
    printf 'required doc missing: %s\n' "$file" >&2
    exit 1
  fi
done

printf 'check-doc-required-files: ok\n'
