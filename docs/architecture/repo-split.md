# Repository Split

The YAI workspace is split by ownership, not by UI or product packaging.

## Current Transition Workspace

```text
YAI/
├── yai/        # old/current concept mine, future ai-environment material
├── yai-core/   # canonical local AI operational control core
├── interfaces/ # projection/API/SDK/conformance
└── console/    # operator terminal/client UX
```

`yai-core` is a sibling to `yai`, not nested inside `yai`.

## Canonical Future Workspace

```text
YAI/
├── yai-core/
├── ai-environment/
├── interfaces/
└── console/
```

`yai` is not renamed during SPINE.0. It is declared transition concept mine and
future `ai-environment` source. A physical rename is a later dedicated wave.

## Ownership

```text
yai-core   = canonical new core and local AI operational control core
yai        = old/current repo; concept mine and future ai-environment material
interfaces = projection/API/SDK/conformance over yai-core primitive truth
console    = operator client / TUI / human UX over projections and interfaces
```

`yai` is not the future core and must not be migrated folder-by-folder into
`yai-core`. `interfaces` must not define core semantics independently.
`console` must not own daemon, store, control, memory or carrier semantics.

## Order

```text
core first
interfaces second
console later
platform last
```

`interfaces` projects protocol and SDK truth from core operations. `console`
consumes operations and projections. Neither owns core semantics.

## Canonical Wave Families

```text
CORE.NEW.*       yai-core implementation
ENV.CANON.*      yai to ai-environment canonicalization
INTF.CANON.*     interfaces canonicalization against yai-core
CONSOLE.CANON.*  console canonicalization against yai-core/interfaces
PLATFORM.LATER.* cloud/team/platform deferred line
```

The canonical roadmap lives in:

```text
../engineering/four-repo-roadmap.md
```

## Non-Goal

Do not recreate `yai-core` as a subfolder of `yai`. Do not keep two active copies
of New Core docs. Do not create `ai-environment`, rename `yai`, or make
`yai-core` depend on old `yai` in the roadmap rebase wave.
