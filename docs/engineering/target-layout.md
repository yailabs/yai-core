# Target Layout

This document separates architectural destination from the first creation waves.
It is not a command to create every directory or file immediately.

Rule:

```text
Target layout is architectural destination.
NEW.0 creates only doctrine, guards and tracked skeleton roots.
NEW.1 creates only files needed for the first executable loop.
```

## 1. Final Target Layout

The future `yai-core` repository should eventually have this root shape:

```text
yai-core/
├── docs/
├── include/
│   └── yai/
├── lib/
├── daemon/
├── ctl/
├── crates/
├── proto/
├── tests/
├── tools/
├── packaging/
├── examples/
├── vendor/
├── Makefile
├── README.md
├── LICENSE
└── VERSION
```

Final internal roots:

```text
base
ingest
subject
case
op
control
effect
store
index
graph
memory
projection
reconcile
daemon
ctl
```

These roots are stable machine responsibilities, not feature areas.

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

NEW.0 should not create large empty file forests. Skeleton directories are
tracked with README placeholders and guard scripts only.

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
crates/Cargo.toml
crates/yai-ctl/Cargo.toml
crates/yai-ctl/src/main.rs
crates/yai-core-engine/Cargo.toml
crates/yai-core-engine/src/lib.rs
crates/yai-core-engine/src/store.rs
crates/yai-core-engine/src/journal.rs
crates/yai-core-engine/src/record.rs
crates/yai-core-engine/src/graph.rs
crates/yai-core-engine/src/index.rs
crates/yai-core-engine/src/memory.rs
crates/yai-core-engine/src/projection.rs
crates/yai-core-engine/src/query.rs
crates/yai-core-engine/src/ffi.rs
crates/yai-core-engine-sys/Cargo.toml
crates/yai-core-engine-sys/src/lib.rs
```

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

```text
yai-core        = local AI operational control core
ai-environment  = external AI systems / agents / tools / workflows lab
interfaces      = schemas / transports / SDK / conformance
console         = operator client / TUI / human control surface
```

Core first. Interfaces second. Console later. Platform last.

`yaictl` belongs in `yai-core` as technical control. Console belongs outside the
core as a human operator surface consuming projections and operations.
