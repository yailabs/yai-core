# Repository Split

The YAI workspace is split by ownership, not by UI or product packaging.

## Current Workspace

```text
YAI/
├── yai/        # canonical local AI operational control system
├── yai-dev/    # development lab, concept mine, harness and scenario workspace
├── interfaces/ # contracts, APIs, SDK and conformance
├── studio/     # product client workspace, not touched by this repo
└── console/    # operator terminal/client UX
```

`yai` is the product/core system. `yai-dev` is the workshop and lab: old
concepts, experiments, provider labs, agentic QA, scenarios, workflows and
release rehearsal material live there until a SPINE wave explicitly extracts
or normalizes them.

## Ownership

```text
yai       = canonical local AI operational control system
yai-dev   = development lab, concept mine, harness and workshop
interfaces= contracts/API/SDK/conformance over yai truth
studio    = official product client surface
console   = operator client / TUI / human UX over projections and interfaces
```

`yai-dev` is not migrated folder-by-folder into `yai`. Useful material is
mined by explicit SPINE waves, rewritten in the current grammar and classified
in the extraction inventory.

`interfaces`, `studio` and `console` do not own daemon, store, control, memory,
carrier, case-world or projection truth.

## Order

```text
core first
interfaces contracts second
studio/client surfaces downstream
console/operator UX downstream
platform later
```

## Wave Families

```text
SPINE.*          yai implementation
DEV.*            yai-dev lab, harness and residue normalization
INTF.SPINE.*     interfaces canonicalization against yai
STUDIO.SPINE.*   studio product client over interfaces/yai
CONSOLE.CANON.*  console canonicalization against projections/interfaces
PLATFORM.LATER.* cloud/team/platform deferred line
```

The active `yai` roadmap lives in:

```text
../engineering/four-repo-roadmap.md
```

The interface and studio roadmap lives in:

```text
../../interfaces/docs/intf-studio-spine.md
```

## Non-Goal

Do not recreate `yai-dev` roots inside `yai`. Do not keep two active copies of
core docs. Do not make `yai` depend on `yai-dev`.
