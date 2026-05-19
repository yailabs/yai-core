# YAI Core

YAI Core is a local AI operational control core.

It binds operational subjects to cases, captures operation attempts,
materializes policy into machine gates, emits control decisions, executes or
observes effects through carriers, records receipts, derives operational memory
and serves controlled projections.

This repository is not an agent framework, workflow engine, runtime monitor,
TUI, cloud platform or model provider.

Start with:

```text
docs/architecture/00-spine.md
docs/architecture/01-terminology.md
docs/architecture/04-subject-model.md
docs/architecture/06-control-policy-model.md
```

Status: NEW.1 minimum-loop package.

NEW.1 implements the first in-process minimum loop:

```text
open case
bind subject
submit op attempt
emit control decision
emit effect receipt
append store record
build projection
```

Ownership:

```text
C    = public ABI, daemon bootstrap, carrier/control boundary v0
Rust = yaictl and operational data engine skeleton
```

`yaictl` is Rust. The operational data engine skeleton is Rust. There is no
persistent store, no real carrier-owned filesystem/process/model effect, no
daemon IPC and no full policy engine yet.

Build and validate:

```text
make info
make check
```
