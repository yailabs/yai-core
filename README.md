# YAI Core

YAI Core is a local AI operational control core.

It binds operational subjects to cases, captures operation attempts,
materializes policy into machine gates, emits control decisions, executes or
observes effects through carriers, records receipts, derives operational memory,
serves controlled projections and scans residue through a minimal query
boundary. Rust can also consume the same residue through an internal engine R1
path behind the C ABI. `yai` is the canonical local technical command and
`yaid` is the local daemon.

SPINE.1 rebases the next phase: the repository is moving toward filesystem /
data-spine refoundation before local install layout. NEW.14 moved the Rust
operational data engine into `engine/`. NEW.15 moved the Rust technical command
into `cmd/yai` and removed `crates/`. The current `lib/`, retired `ctl/` pointer
and top-level `daemon/` roots are still bootstrap-era surfaces, not the final
filesystem doctrine.

This repository is not an agent framework, workflow engine, runtime monitor,
TUI, cloud platform or model provider.

Start with:

```text
docs/architecture/00-spine.md
docs/architecture/01-terminology.md
docs/architecture/04-subject-model.md
docs/architecture/06-control-policy-model.md
docs/engineering/filesystem-target-v2.md
docs/engineering/data-spine-refactor-roadmap.md
docs/engineering/new13-filesystem-refactor-plan.md
```

Status: NEW.15 `yai` command moved to `cmd/yai`. The next move wave is NEW.16
`yaid` entrypoint move from `daemon/main.c` to `cmd/yaid/main.c`. Local install
layout is delayed to NEW.20.

NEW.13 is a planning wave only. It creates the surgical map for NEW.14 through
NEW.21 and does not move source files.

NEW.14 is the first physical filesystem refactor wave. It moves only the Rust
engine crates:

```text
crates/yai-core-engine     -> engine/yai-engine
crates/yai-core-engine-sys -> engine/yai-engine-ffi
```

That old `crates/` path is now historical; `crates/` is removed after NEW.15.

NEW.15 moves the technical command:

```text
crates/yai-ctl -> cmd/yai
```

`crates/` is removed after NEW.15. `cmd/yai` is the core technical command, not
Console or operator UX. `yaid` still uses the top-level `daemon/` source root
until NEW.16.

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
yai memory summary
```

NEW.7 adds the first reconciliation layer:

```text
divergence detection
denied_but_executed
receipt_without_decision
reconciliation records
reconcile projection
yai reconcile summary
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
yai projection inspect
```

NEW.9 adds the first journal query boundary:

```text
query filter
journal scan
query_result record
kind/case filtering
yai query summary
yai query records
```

NEW.10 adds Rust engine R1 behind the C ABI:

```text
opaque engine handle
JSON record append
record/kind counts
kind query
projection summary JSON
yai engine summary
```

NEW.11 adds the first resident daemon IPC boundary:

```text
yaid --socket <path> --foreground
yai daemon status
yai daemon info
yai daemon shutdown
```

NEW.12 makes the first bounded loop pass through `yaid`:

```text
yai daemon run-minimum-loop
yai daemon run-filesystem-loop
yai daemon journal-summary
yai daemon projection-summary
```

NEW.13 no longer adds the local install layout. NEW.13 is the target
filesystem doctrine/refactor plan:

```text
include/ = public C ABI contracts
system/  = C system plane: daemon, host, carriers, control boundary, FFI bridges
engine/  = Rust operational data spine
cmd/     = binaries: yai and yaid
```

Ownership:

```text
C    = system / host boundary / ABI / daemon / carrier / control enforcement shell
Rust = engine / operational data spine
```

`yai` is Rust and now lives under `cmd/yai`. The bootstrap operational data
engine is Rust and now lives under `engine/`. Local command installation is
delayed to NEW.20, and there is no public API, no HTTP, no
auth, no service manager, no multi-client runtime, no process/network/model/database carrier, no full policy engine, no graph
database, no vector/RAG retrieval, no automatic repair, no memory consolidation
engine, no backend switch and no full secret redaction engine yet.

Build and validate:

```text
make info
make check
```

Planned local install layout (NEW.20, not current doctrine):

```text
make install-local PREFIX=$HOME/.local YAI_HOME=$HOME/.yai
yai info
yai doctor
```

Inspect the NEW.2 smoke journal after `make smoke-new2`:

```text
yai store tail --journal build/tmp/new2/persistent-journal-<pid>/journal.jsonl
yai projection summary --journal build/tmp/new2/persistent-journal-<pid>/journal.jsonl
```

Inspect the NEW.3 control journal after `make smoke-new3`:

```text
yai control summary --journal build/tmp/new3/control-gate-<pid>/journal.jsonl
yai decision inspect --journal build/tmp/new3/control-gate-<pid>/journal.jsonl
```

Inspect the NEW.4 filesystem journal after `make smoke-new4`:

```text
yai receipt summary --journal build/tmp/new4/filesystem-carrier-<pid>/journal.jsonl
yai carrier fs-read --sandbox build/tmp/new4/filesystem-carrier-<pid>/sandbox --path build/tmp/new4/filesystem-carrier-<pid>/sandbox/input.txt
```

Inspect the NEW.5 graph journal after `make smoke-new5`:

```text
yai graph summary --journal build/tmp/new5/graph-reconstruction-<pid>/journal.jsonl
```

Inspect the NEW.6 memory journal after `make smoke-new6`:

```text
yai memory summary --journal build/tmp/new6/operational-memory-<pid>/journal.jsonl
```

Inspect the NEW.7 reconcile journal after `make smoke-new7`:

```text
yai reconcile summary --journal build/tmp/new7/reconcile-divergence-<pid>/journal.jsonl
```

Inspect the NEW.8 projection journal after `make smoke-new8`:

```text
yai projection inspect --journal build/tmp/new8/projection-hardening-<pid>/journal.jsonl
yai projection request --journal build/tmp/new8/projection-hardening-<pid>/journal.jsonl --consumer model --kind model_context
```

Inspect the NEW.9 query journal after `make smoke-new9`:

```text
yai query summary --journal build/tmp/new9/query-boundary-<pid>/journal.jsonl
yai query records --journal build/tmp/new9/query-boundary-<pid>/journal.jsonl --kind receipt --limit 10
```

Inspect a journal through Rust engine summary:

```text
yai engine summary --journal build/tmp/new9/query-boundary-<pid>/journal.jsonl
```

Inspect NEW.11 daemon IPC manually:

```text
build/yaid --socket build/tmp/new11/manual/yaid.sock --foreground
yai daemon status --socket build/tmp/new11/manual/yaid.sock
```

Inspect NEW.12 daemon-backed loops manually:

```text
build/yaid --socket build/tmp/new12/manual/yaid.sock --foreground
yai daemon run-minimum-loop --socket build/tmp/new12/manual/yaid.sock
yai daemon run-filesystem-loop --socket build/tmp/new12/manual/yaid.sock
```
