# Target Layout

This document separates architectural destination from the first creation waves.
It is not a command to create every directory or file immediately.

Rule:

```text
Target layout is architectural destination.
NEW.0 creates only doctrine, guards and tracked skeleton roots.
NEW.1 creates only files needed for the first executable loop.
```

## 1. Target Layout V2

SPINE.1 rebases the filesystem doctrine. The future `yai-core` repository
should eventually have this root shape:

```text
yai-core/
├── include/
├── system/
├── engine/
├── cmd/
├── proto/
├── tests/
├── packaging/
├── docs/
├── tools/
├── examples/
├── vendor/
├── Cargo.toml
├── Makefile
├── README.md
├── LICENSE
└── VERSION
```

Meaning:

| Root | Meaning |
|---|---|
| `include/` | public C ABI contracts |
| `system/` | C system plane: daemon, host, carriers, control boundary, FFI bridges |
| `engine/` | Rust operational data engine |
| `cmd/` | binaries: `yai` and `yaid` |
| `proto/` | schemas and fixtures |
| `tests/` | smoke, unit, integration and adversarial tests |

These roots are stable machine responsibilities, not feature areas.

Target system shape:

```text
system/
├── base/
├── case/
├── subject/
├── op/
├── control/
├── effect/
│   └── carriers/
├── daemon/
├── host/
├── engine_bridge/
├── internal/
└── README.md
```

Target engine shape:

```text
engine/
├── Cargo.toml
├── yai-engine/
│   ├── Cargo.toml
│   └── src/
│       ├── lib.rs
│       ├── residue/
│       ├── store/
│       ├── journal/
│       ├── record/
│       ├── graph/
│       ├── index/
│       ├── query/
│       ├── memory/
│       ├── projection/
│       ├── reconcile/
│       ├── retention/
│       ├── integrity/
│       └── ffi/
├── yai-engine-ffi/
│   ├── Cargo.toml
│   └── src/lib.rs
└── README.md
```

Target command shape:

```text
cmd/
├── yai/
│   ├── Cargo.toml
│   └── src/main.rs
└── yaid/
    └── main.c
```

Current bootstrap roots are transitional:

| Current root | Target destination | Status |
|---|---|---|
| `lib/` | `system/` plus Rust-owned data logic in `engine/` | transitional |
| `crates/` | `engine/` and `cmd/yai/` | transitional |
| `ctl/` | `cmd/yai/` or removed after drain | transitional |
| top-level `daemon/` | `cmd/yaid/` and `system/daemon/` | transitional |

The target does not keep `lib/` as the long-term implementation root. NEW.13
is target filesystem doctrine/refactor planning. Local install layout is
delayed to NEW.20.

NEW.13 adds the executable move plan in:

```text
new13-filesystem-refactor-plan.md
filesystem-move-matrix.md
engine-ownership-map.md
system-ownership-map.md
```

## 2. NEW.0 Minimal Skeleton

NEW.0 should create the smallest real repository skeleton that prevents drift.

Allowed NEW.0 content:

```text
docs/
include/yai/
lib/
daemon/
ctl/
proto/
tests/
tools/checks/
Makefile
README.md
LICENSE
VERSION
```

Allowed NEW.0 files:

```text
docs/architecture/00-spine.md
docs/architecture/04-subject-model.md
docs/architecture/06-control-policy-model.md
docs/engineering/target-layout.md
docs/engineering/extraction-plan.md
docs/engineering/c-abi.md
docs/engineering/rust-engine.md
docs/adr/0000-decision-compendium.md
tools/checks/check-no-old-roots.sh
```

Allowed NEW.0 tracked skeleton roots:

```text
include/yai/base
include/yai/ingest
include/yai/subject
include/yai/case
include/yai/op
include/yai/control
include/yai/effect
include/yai/store
include/yai/index
include/yai/graph
include/yai/memory
include/yai/projection
include/yai/reconcile
lib/base
lib/ingest
lib/subject
lib/case
lib/op
lib/control
lib/effect
lib/store
lib/index
lib/graph
lib/memory
lib/projection
lib/reconcile
daemon
ctl
```

NEW.0 did not create large empty file forests. Bootstrap directories were
tracked with transitional README markers and guard scripts only; NEW.13 maps
those markers to target roles and owning refactor waves.

## 3. NEW.1 Minimum Loop Files

NEW.1 creates only files needed for the first executable loop:

```text
open case
bind subject
submit op attempt
control decision
null carrier / observed receipt
append store record
query projection
```

Minimum public headers:

```text
include/yai/base/id.h
include/yai/base/error.h
include/yai/case/case_ref.h
include/yai/subject/subject_ref.h
include/yai/subject/subject_binding.h
include/yai/op/attempt.h
include/yai/control/decision.h
include/yai/effect/receipt.h
include/yai/store/record.h
include/yai/projection/projection.h
```

Minimum C implementation files:

```text
lib/base/id.c
lib/base/error.c
lib/case/case_ref.c
lib/subject/subject_binding.c
lib/op/attempt.c
lib/control/decision.c
lib/effect/receipt.c
lib/store/record.c
lib/store/journal.c
lib/projection/projection.c
daemon/main.c
```

Minimum Rust files:

```text
engine/Cargo.toml
engine/yai-engine/Cargo.toml
engine/yai-engine/src/lib.rs
engine/yai-engine/src/store.rs
engine/yai-engine/src/journal.rs
engine/yai-engine/src/record.rs
engine/yai-engine/src/graph.rs
engine/yai-engine/src/index.rs
engine/yai-engine/src/memory.rs
engine/yai-engine/src/projection.rs
engine/yai-engine/src/query.rs
engine/yai-engine/src/ffi.rs
engine/yai-engine-ffi/Cargo.toml
engine/yai-engine-ffi/src/lib.rs
crates/Cargo.toml
crates/yai-ctl/Cargo.toml
crates/yai-ctl/src/main.rs
```

The `engine/` paths are the NEW.14 engine target. The `crates/yai-ctl` paths
remain transitional until NEW.15 moves the command to `cmd/yai`.

The planned NEW.14 through NEW.21 sequence moves these bootstrap paths without
changing smoke behavior.

Minimum tests:

```text
tests/smoke/open-case
tests/smoke/bind-subject
tests/smoke/submit-op
tests/smoke/deny-op
tests/smoke/receipt-record
tests/smoke/query-projection
tests/guards/no-old-roots
```

## 4. Forbidden Future Core Roots

```text
agents
analytics
capabilities
decision
lineage
models
orchestrator
runtime
substrate
governance
knowledge
state
providers
workflow
```

The guard must check both root directories and accidental new public include
roots. Compatibility headers in the current repo can exist until drained, but
the future `yai-core` must not recreate these names as core roots.

## 5. Repo Split

Current transition workspace:

```text
YAI/
├── yai-core/
├── yai/
├── interfaces/
└── console/
```

Canonical future workspace:

```text
YAI/
├── yai-core/
├── ai-environment/
├── interfaces/
└── console/
```

Ownership:

```text
yai-core       = canonical new core and local AI operational control core
yai            = transition concept mine and future ai-environment source
ai-environment = external AI systems, adapters, tools and workflow lab
interfaces     = projection/API/SDK/conformance over yai-core primitive truth
console        = operator client / TUI / human control surface
```

`yai` is not renamed in SPINE.0. It remains a sibling repo and must not be
migrated folder-by-folder into `yai-core`.

Core first. Interfaces second. Console later. Platform last.

`yai` belongs in `yai-core` as technical control. Console belongs outside the
core as a human operator surface consuming projections and operations.

The updated milestone roadmap is maintained in:

```text
docs/engineering/four-repo-roadmap.md
```
