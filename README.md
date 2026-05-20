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

Status: NEW.6 operational memory candidate.

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

NEW.4 adds the first real carrier effect, confined to a test sandbox:

```text
control decision
filesystem carrier
fs.read / fs.write
hash posture before and after
effect receipt
subject state update
filesystem projection
```

NEW.5 adds the first operational graph layer:

```text
graph edge records
case/subject/op/decision/receipt relationships
receipt chain reconstruction
graph projection
```

NEW.6 adds the first residue-derived memory candidate:

```text
receipt reconstruction
decision and subject refs
basis record / receipt / graph-edge counts
memory_candidate record
memory projection
yaictl memory summary
```

Ownership:

```text
C    = public ABI, daemon bootstrap, carrier/control boundary v0
Rust = yaictl and operational data engine skeleton
```

`yaictl` is Rust. The operational data engine skeleton is Rust. NEW.6 adds
file-based memory summary inspection, but there is no real daemon IPC, no
process/network/model/database carrier, no full policy engine, no graph
database and no memory consolidation engine yet.

Build and validate:

```text
make info
make check
```

Inspect the NEW.2 smoke journal after `make smoke-new2`:

```text
crates/target/debug/yaictl store tail --journal build/tmp/new2/persistent-journal-<pid>/journal.jsonl
crates/target/debug/yaictl projection summary --journal build/tmp/new2/persistent-journal-<pid>/journal.jsonl
```

Inspect the NEW.3 control journal after `make smoke-new3`:

```text
crates/target/debug/yaictl control summary --journal build/tmp/new3/control-gate-<pid>/journal.jsonl
crates/target/debug/yaictl decision inspect --journal build/tmp/new3/control-gate-<pid>/journal.jsonl
```

Inspect the NEW.4 filesystem journal after `make smoke-new4`:

```text
crates/target/debug/yaictl receipt summary --journal build/tmp/new4/filesystem-carrier-<pid>/journal.jsonl
crates/target/debug/yaictl carrier fs-read --sandbox build/tmp/new4/filesystem-carrier-<pid>/sandbox --path build/tmp/new4/filesystem-carrier-<pid>/sandbox/input.txt
```

Inspect the NEW.5 graph journal after `make smoke-new5`:

```text
crates/target/debug/yaictl graph summary --journal build/tmp/new5/graph-reconstruction-<pid>/journal.jsonl
```

Inspect the NEW.6 memory journal after `make smoke-new6`:

```text
crates/target/debug/yaictl memory summary --journal build/tmp/new6/operational-memory-<pid>/journal.jsonl
```
