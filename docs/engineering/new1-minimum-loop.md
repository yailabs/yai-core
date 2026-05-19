# NEW.1 Minimum Loop

NEW.1 implements the first in-process YAI Core spine.

## Loop

```text
open case
bind subject
submit op attempt
control decision
null carrier / observed receipt
append store record
query projection
```

## C Ownership

C owns:

```text
public ABI
base primitive headers
case/subject/op/control/effect/store/projection structs
control v0 deterministic logic
receipt v0
daemon bootstrap stub
carrier boundary placeholder
C smoke loop
```

Implemented C headers:

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
include/yai/store/journal.h
include/yai/projection/projection.h
```

Implemented C files:

```text
lib/base/id.c
lib/base/error.c
lib/case/case_ref.c
lib/subject/subject_ref.c
lib/subject/subject_binding.c
lib/op/attempt.c
lib/control/decision.c
lib/effect/receipt.c
lib/store/record.c
lib/store/journal.c
lib/projection/projection.c
daemon/main.c
tests/smoke/minimum-loop/test_minimum_loop.c
```

## Rust Ownership

Rust owns:

```text
yaictl implementation
operational data engine skeleton
store/journal/record modules
memory module
projection module
graph/index/query placeholders
future FFI surface
```

Implemented Rust crates:

```text
crates/yai-ctl
crates/yai-core-engine
crates/yai-core-engine-sys
```

`yaictl` commands:

```text
yaictl --version
yaictl info
yaictl doctor
```

`yaictl` is a Rust client over core primitives. It does not own core semantics.

## Deterministic Control V0

```text
deny if case is empty
deny if target subject is empty
allow observed/read-like attempts
require_review for mutative attempts
```

NEW.1 does not implement the full policy engine.

## Smoke Test

The C smoke test proves:

```text
case:newcore-test
subject:repo-test bound
op:file.write attempted
decision:require_review
receipt:blocked
store:records appended
projection:operator summary built
```

## Validation

```text
make info
make check-layout
make build-c
make build-rust
make build
make smoke
make check
```

## Non-Goals

```text
no old yai/src copy
no src/
no real filesystem/process/model carriers
no persistent store
no full memory consolidation
no daemon IPC
no full policy engine
no Rust types across public C ABI
```
