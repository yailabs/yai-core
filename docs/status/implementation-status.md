# Implementation Status

This document carries implementation history, smoke references, and wave-local
status notes that do not belong in the stable repository README.

## Current Position

NEW.18C rebases the prompt/session boundary before NEW.19: the journal remains
replay/audit, while active model context is selected interaction thread plus
participant view frame over the current case projection.

Status: NEW.18 completed the engine bridge split. NEW.18.FIX removed stale
post-NEW.18 layout drift and status drift. NEW.18A completed the case-world
binding record vertical slice. NEW.18B adds live `case_context` and
`case_session` semantics while preserving durable refs. SPINE.3R and SPINE.4
are complete. NEW.18C is complete; NEW.19 follows it. Local install
layout is delayed to NEW.20.

Model-in-case status: the daemon filesystem loop emits model subject, policy
and model-context projection evidence. `yai case enter` admits
`subject:llm-provider` into the case, `yai case attach-provider` records the
OpenAI-compatible provider route without storing secrets, and `yai` with an
active case enters the vendored Linenoise prompt surface. The prompt session
materializes a participant view frame from the active interaction thread plus
current case projection, records prompt attempts and provider output receipts,
and keeps transcript persistence separate from active thread context. Prompt
transcript retention is preview-only by default, with explicit redacted
case-local capture through `/transcript on` and derived prompt memory through
`/memory propose`.
The filesystem manual case now separates `policy_rule`, `projection_rule`,
`authority_scope`, and `model_interpretation` records so model claims can be
checked against authoritative case residue instead of treated as core truth.
Case verticalization has started inside `yai-core`: the C ABI now has
`case_domain`, `case_attachment`, and `case_binding` primitives and matching
record kinds. The filesystem manual case emits those records before subjects
and policies, so a model participant sees an operational case world rather than
a flat journal label. Registry persistence, SHM, LMDB, DuckDB, Ladybug and full
runtime carrier enforcement remain planned-not-created in this repository.
SPINE.3R also keeps shared memory, LMDB, DuckDB, Ladybug and live projection
delta implementation planned-not-created until dedicated data-plane waves.
NEW.18B normalizes the active case posture: `case_ref`, `subject_ref` and
receipt refs remain durable boundary material, while runtime operation uses a
loaded `case_context` inside a `case_session`.
SPINE.4 makes Operational Observability & Evaluation canonical: Case View
Quality, trace, freshness, provenance, rebuild posture, model behavior facts
and memory quality facts are required design inputs for future waves, but their
records and debug commands remain planned-not-created.
NEW.18C adds `interaction_thread`, `interaction_turn` and
`participant_view_frame` records. The prompt surface supports `/thread status`,
`/thread new`, `/thread list`, `/thread use` and `/thread archive`; a new
thread resets the conversational lane without deleting journal/audit history.

## Filesystem Refoundation History

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
Console or operator UX.

NEW.16 moves the daemon entrypoint and daemon support:

```text
daemon/main.c              -> cmd/yaid/main.c
daemon/ipc.c               -> system/daemon/ipc.c
daemon/core_loop.c         -> system/daemon/core_loop.c
lib/daemon/daemon_status.c -> system/daemon/daemon_status.c
```

The top-level `daemon/` root is removed after NEW.16. `yaid` remains C, and
`yai` remains Rust in `cmd/yai`.

NEW.17 moves the remaining C implementation:

```text
lib/* -> system/*
```

`lib/` and the retired `ctl/` root are removed after NEW.17. The data-spine C
folders now under `system/{store,graph,index,memory,projection,reconcile}` are
transitional and scheduled for NEW.18 thinning/splitting.

NEW.18 is the engine bridge split wave. It creates `system/engine_bridge`,
moves the Rust engine C shim there, and classifies the remaining C data-spine
folders as `keep_temporarily`.

NEW.18.FIX is the post-NEW.18 cleanup wave. It removes stale `crates/target`
build output so `crates/` is absent again, keeps `target/`, `build/` and
`.DS_Store` ignored, and updates this status record so NEW.18 is no longer
described as the next wave after NEW.17.

NEW.18A is the case-world binding record vertical slice. It adds case-world
record posture for `case_domain`, `case_attachment` and `case_binding`.

SPINE.3R follows NEW.18.FIX and NEW.18A before NEW.19. It is the documentation
rebase for case-world-first operation, live projection and operational data
plane doctrine.

SPINE.4 follows NEW.18B before NEW.19. It is the documentation rebase for
operational observability/evaluation and Case View Quality.

## Minimum Loop History

NEW.1 implemented the first in-process minimum loop.

NEW.2 makes that loop persistent and reconstructable through a file-backed
JSONL journal:

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

## Target Layout Notes

NEW.13 no longer adds the local install layout. NEW.13 is the target filesystem
doctrine/refactor plan:

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
delayed to NEW.20, and there is no public API, no HTTP, no auth, no service
manager, no multi-client runtime, no process/network/model/database carrier, no
full policy engine, no graph database, no vector/RAG retrieval, no automatic
repair, no memory consolidation engine, no backend switch, and no full secret
redaction engine yet.

## Build And Validate

```text
make info
make check
```

## Planned Local Install Layout

Planned local install layout for NEW.20:

```text
make install-local PREFIX=$HOME/.local YAI_HOME=$HOME/.yai
yai info
yai doctor
```

## Smoke Journal Inspection

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
