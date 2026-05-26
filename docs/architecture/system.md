# System Architecture

YAI is a local operational control runtime for case-bound AI activity. It does
not treat model output, provider behavior or terminal input as authority by
itself. It binds activity to cases, subjects, operations, policy decisions,
receipts and durable records.

## Core Terms

- `case`: the operational boundary for records, policy, subjects, attempts and
  receipts.
- `world`: the scoped environment a case can describe or affect.
- `subject`: an actor, file, process, provider, operator, model, artifact or
  system boundary inside a case.
- `operation`: an attempted action that must be evaluated before execution or
  import.
- `receipt`: structured evidence of what happened at a boundary.

## Runtime Boundary

The CLI renders commands and invokes the local daemon/runtime. The daemon owns
local runtime coordination and carrier dispatch. Data-plane persistence is
served through the Rust engine boundary where current record, journal, LMDB,
graph and RuntimeGraph behavior is implemented.

The C/Rust split is intentionally explicit:

```text
system/  C operational boundary, daemon, control and carrier surfaces
engine/  Rust durable data and query engine
cmd/     local command binaries
include/ public headers
net/     network substrate boundary
```

The repository root is not a single runtime component. `docs/` is curated
architecture documentation, `labs/` is experiment/run material and `work/` is
execution history, spines, inventories and agent notes.

