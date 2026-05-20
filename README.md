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

Status: NEW.3 control gate and policy rule skeleton.

NEW.1 implemented the first in-process minimum loop. NEW.2 makes that loop
persistent and reconstructable through a file-backed JSONL journal:

```text
open case
bind subject
submit op attempt
emit control decision
emit effect receipt
append store record
write journal file
reload journal records
reconstruct subject state
build projection from persisted records
```

NEW.3 adds the first structured control machine over that persistence:

```text
policy rule
gate evaluation
decision basis
decision outcome
obligation
receipt requirement
persisted control records
control projection
```

Ownership:

```text
C    = public ABI, daemon bootstrap, carrier/control boundary v0
Rust = yaictl and operational data engine skeleton
```

`yaictl` is Rust. The operational data engine skeleton is Rust. NEW.3 adds
file-based control and decision inspection commands, but there is no real daemon
IPC, no carrier-owned filesystem/process/model effect, no full policy engine, no
graph engine and no memory consolidation yet.

Build and validate:

```text
make info
make check
```

Inspect the NEW.2 smoke journal after `make smoke-new2`:

```text
crates/target/debug/yaictl store tail --journal build/tmp/new2/journal.jsonl
crates/target/debug/yaictl projection summary --journal build/tmp/new2/journal.jsonl
```

Inspect the NEW.3 control journal after `make smoke-new3`:

```text
crates/target/debug/yaictl control summary --journal build/tmp/new3/journal.jsonl
crates/target/debug/yaictl decision inspect --journal build/tmp/new3/journal.jsonl
```
