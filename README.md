# YAI Core

YAI Core is a local AI operational control core.

It binds operational subjects to cases, captures operation attempts,
materializes policy into machine gates, emits control decisions, executes or
observes effects through carriers, records receipts, derives operational memory,
serves controlled projections and scans residue through a minimal query
boundary. Rust can also consume the same residue through an internal engine R1
path behind the C ABI. `yaid` now exposes a local daemon IPC path that can run
the first bounded core loops.

This repository is not an agent framework, workflow engine, runtime monitor,
TUI, cloud platform or model provider.

Start with:

```text
docs/architecture/00-spine.md
docs/architecture/01-terminology.md
docs/architecture/04-subject-model.md
docs/architecture/06-control-policy-model.md
```

Status: NEW.12 daemon-backed core loop v0.

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

NEW.7 adds the first reconciliation layer:

```text
divergence detection
denied_but_executed
receipt_without_decision
reconciliation records
reconcile projection
yaictl reconcile summary
```

NEW.8 hardens projection into controlled read-model records:

```text
projection_request
projection_result
consumer kind
projection kind
provenance counts
freshness
redaction posture
yaictl projection inspect
```

NEW.9 adds the first journal query boundary:

```text
query filter
journal scan
query_result record
kind/case filtering
yaictl query summary
yaictl query records
```

NEW.10 adds Rust engine R1 behind the C ABI:

```text
opaque engine handle
JSON record append
record/kind counts
kind query
projection summary JSON
yaictl engine summary
```

NEW.11 adds the first resident daemon IPC boundary:

```text
yaid --socket <path> --foreground
yaictl daemon status
yaictl daemon info
yaictl daemon shutdown
```

NEW.12 makes the first bounded loop pass through `yaid`:

```text
yaictl daemon run-minimum-loop
yaictl daemon run-filesystem-loop
yaictl daemon journal-summary
yaictl daemon projection-summary
```

Ownership:

```text
C    = public ABI, daemon bootstrap, carrier/control boundary v0
Rust = yaictl and operational data engine skeleton
```

`yaictl` is Rust. The operational data engine skeleton is Rust. NEW.12 adds
the first daemon-backed core loop path, but there is no public API, no HTTP, no
auth, no multi-client runtime, no process/network/model/database carrier, no full policy engine, no graph
database, no vector/RAG retrieval, no automatic repair, no memory consolidation
engine, no backend switch and no full secret redaction engine yet.

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

Inspect the NEW.7 reconcile journal after `make smoke-new7`:

```text
crates/target/debug/yaictl reconcile summary --journal build/tmp/new7/reconcile-divergence-<pid>/journal.jsonl
```

Inspect the NEW.8 projection journal after `make smoke-new8`:

```text
crates/target/debug/yaictl projection inspect --journal build/tmp/new8/projection-hardening-<pid>/journal.jsonl
crates/target/debug/yaictl projection request --journal build/tmp/new8/projection-hardening-<pid>/journal.jsonl --consumer model --kind model_context
```

Inspect the NEW.9 query journal after `make smoke-new9`:

```text
crates/target/debug/yaictl query summary --journal build/tmp/new9/query-boundary-<pid>/journal.jsonl
crates/target/debug/yaictl query records --journal build/tmp/new9/query-boundary-<pid>/journal.jsonl --kind receipt --limit 10
```

Inspect a journal through Rust engine summary:

```text
crates/target/debug/yaictl engine summary --journal build/tmp/new9/query-boundary-<pid>/journal.jsonl
```

Inspect NEW.11 daemon IPC manually:

```text
build/yaid --socket build/tmp/new11/manual/yaid.sock --foreground
crates/target/debug/yaictl daemon status --socket build/tmp/new11/manual/yaid.sock
```

Inspect NEW.12 daemon-backed loops manually:

```text
build/yaid --socket build/tmp/new12/manual/yaid.sock --foreground
crates/target/debug/yaictl daemon run-minimum-loop --socket build/tmp/new12/manual/yaid.sock
crates/target/debug/yaictl daemon run-filesystem-loop --socket build/tmp/new12/manual/yaid.sock
```
