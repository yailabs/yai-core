#!/bin/sh
# YAI - required docs guard
#
# Purpose:
#   Ensure the active documentation set needed by current waves exists.
#
# Scope:
#   Checks required active docs, lab docs and ADR files.
#
# Non-goals:
#   Does not validate every doc phrase.
set -eu

ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)

for file in \
  work/archive/architecture-snapshots/00-spine.md \
  work/archive/architecture-snapshots/15-pack-materialization.md \
  work/archive/architecture-snapshots/16-hot-state-plane.md \
  work/archive/architecture-snapshots/18-control-carrier-substrate.md \
  work/archive/architecture-snapshots/19-operation-dispatch-multiplex.md \
  work/archive/architecture-snapshots/20-carrier-contract-v1.md \
  work/archive/architecture-snapshots/21-process-carrier-signal-control.md \
  work/archive/architecture-snapshots/22-host-observation-probe.md \
  work/archive/architecture-snapshots/23-carrier-coverage-matrix.md \
  work/archive/architecture-snapshots/24-non-process-carrier-skeletons.md \
  work/archive/architecture-snapshots/25-carrier-outcome-harness.md \
  work/archive/architecture-snapshots/26-context-compiler-retrieval-boundary.md \
  work/archive/architecture-snapshots/27-model-runtime-mtp-boundary.md \
  work/archive/architecture-snapshots/28-provider-runtime-lan-target-surface.md \
  work/archive/architecture-snapshots/30-carrier-receipt-divergence.md \
  work/archive/architecture-snapshots/31-data-context-runtime-runtimegraph.md \
  work/archive/architecture-snapshots/32-lmdb-record-plane-freeze.md \
  work/archive/architecture-snapshots/01-terminology.md \
  work/archive/architecture-snapshots/04-subject-model.md \
  work/archive/architecture-snapshots/06-control-policy-model.md \
  work/archive/engineering-snapshots/README.md \
  work/agents/agent-operating-appendix.md \
  work/spines/current-status.md \
  work/spines/command-surface.md \
  work/archive/engineering-snapshots/data-context-runtime-runtimegraph.md \
  work/archive/engineering-snapshots/carrier-contract-v1.md \
  work/archive/engineering-snapshots/control-carrier-rebase.md \
  work/archive/engineering-snapshots/operation-dispatch-multiplex.md \
  work/archive/engineering-snapshots/process-carrier-signal-control.md \
  work/archive/engineering-snapshots/host-observation-probe.md \
  work/archive/engineering-snapshots/carrier-coverage-matrix.md \
  work/spines/data-plane.md \
  work/archive/engineering-snapshots/extraction-plan.md \
  work/archive/engineering-snapshots/file-header-standard.md \
  work/archive/engineering-snapshots/filesystem-target.md \
  work/spines/yai-spine.md \
  work/archive/engineering-snapshots/hot-state-plane.md \
  work/archive/engineering-snapshots/lmdb-record-plane-freeze.md \
  work/archive/engineering-snapshots/carrier-outcome-harness.md \
  work/archive/engineering-snapshots/carrier-receipt-divergence.md \
  work/archive/engineering-snapshots/non-process-carrier-skeletons.md \
  work/archive/engineering-snapshots/provider-runtime-lan-target-surface.md \
  work/waves/operational-extraction-contract.md \
  work/archive/engineering-snapshots/pack-format.md \
  work/archive/engineering-snapshots/pack-roadmap.md \
  work/spines/source-surface.md \
  work/spines/testing.md \
  work/waves/wave-template.md \
  labs/README.md \
  labs/registry.md \
  labs/shared/bin/probe-local-provider.sh \
  labs/shared/bin/read-lab-prompt.py \
  labs/filesystem-loop/README.md \
  labs/filesystem-loop/runbook.md \
  labs/filesystem-loop/notebook.ipynb \
  labs/filesystem-loop/run.sh \
  labs/filesystem-loop/prompts.json \
  labs/context-residency/README.md \
  labs/context-residency/runbook.md \
  labs/context-residency/tests.md \
  labs/context-residency/outputs.md \
  labs/context-residency/run.sh \
  labs/context-residency/prompts.json \
  labs/external-runtime/README.md \
  labs/external-runtime/runbook.md \
  labs/external-runtime/notebook.ipynb \
  labs/external-runtime/run.sh \
  labs/external-runtime/prompts.json \
  work/archive/adr/0017-packs-as-case-materialization-units.md \
  work/archive/adr/0018-hot-state-is-not-truth.md \
  work/archive/adr/0020-control-carrier-substrate.md \
  work/archive/adr/0021-operation-dispatch-multiplex.md \
  work/archive/adr/0022-carrier-contract-v1.md \
  work/archive/adr/0023-process-carrier-signal-control.md \
  work/archive/adr/0024-host-observation-probe.md \
  work/archive/adr/0025-carrier-coverage-matrix.md \
  work/archive/adr/0026-non-process-carrier-skeletons.md \
  work/archive/adr/0027-carrier-outcome-harness.md \
  work/archive/adr/0028-provider-runtime-lan-target-surface.md \
  work/archive/adr/0030-carrier-receipt-divergence.md \
  work/archive/adr/0031-data-context-runtime-runtimegraph.md \
  work/archive/adr/0032-lmdb-record-plane-freeze.md \
  work/archive/product/positioning.md \
  work/archive/adr/0000-decision-compendium.md
do
  if [ ! -f "$ROOT/$file" ]; then
    printf 'required doc missing: %s\n' "$file" >&2
    exit 1
  fi
done

printf 'check-doc-required-files: ok\n'
