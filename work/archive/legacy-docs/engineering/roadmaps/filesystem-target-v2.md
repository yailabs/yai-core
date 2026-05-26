Historical/superseded engineering record. Not an active source of truth.

# Filesystem Target V2

Status: NEW.17 active target filesystem layout.

NEW.0 through NEW.12 proved the local loop with a hybrid layout:

```text
include/
lib/
daemon/
ctl/      # retired pointer only after NEW.15
cmd/
```

That shape remains historical implementation evidence. NEW.14 through NEW.17
moved the active implementation into `engine/`, `cmd/` and `system/`; local
install layout remains delayed until NEW.20.

## Target Top-Level Shape

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

## Target Engine Shape

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

## Target Command Shape

```text
cmd/
├── yai/
│   ├── Cargo.toml
│   └── src/main.rs
└── yaid/
    └── main.c
```

## Target System Shape

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

## Transitional Roots

Bootstrap roots are now retired. Transitional data logic remains under
`system/` as `keep_temporarily` after NEW.18.

| Current root | Target destination | Status |
|---|---|---|
| `lib/` | `system/` plus Rust-owned data logic in `engine/` | removed after NEW.17 |
| `crates/` | removed after NEW.15 | retired |
| `ctl/` | removed after NEW.17 | retired |
| `cmd/yai/` | Rust technical command | active after NEW.15 |
| top-level `daemon/` | removed after NEW.16 | retired |
| `cmd/yaid/` | C daemon entrypoint | active after NEW.16 |
| `system/daemon/` | C daemon support | active after NEW.16 |
| `system/{store,graph,index,memory,projection,reconcile}` | transitional data logic pending bridge split | NEW.18 target |

The target does not keep `lib/` as an implementation root.

NEW.13 turns this doctrine into an executable refactor plan:

```text
work/archive/engineering-snapshots/new13-filesystem-refactor-plan.md
work/archive/engineering-snapshots/filesystem-move-matrix.md
work/archive/engineering-snapshots/engine-ownership-map.md
work/archive/engineering-snapshots/system-ownership-map.md
```

## Ownership Doctrine

```text
C     = system / host boundary / ABI / daemon / carrier / control enforcement shell
Rust  = engine / operational data spine
```

C owns:

```text
public ABI
daemon bootstrap
host carriers
control enforcement boundary
system bridge
FFI shims
```

Rust owns:

```text
store
journal
record codec
graph
index/query
memory
projection
reconcile
retention
integrity
```

Target:

```text
C wrappers become thin.
C duplicate data logic is quarantined and eventually removed.
Rust engine becomes the operational data spine.
```

## Next Wave

The current implementation wave state is:

```text
SPINE.3R case world + live data plane rebase
```

NEW.14 moved only the Rust data engine crates into `engine/`. NEW.15 moved the
Rust technical command into `cmd/yai` and removed `crates/`. Local install
NEW.16 moved `yaid` into `cmd/yaid` and daemon support into `system/daemon`.
NEW.17 moved remaining C implementation into `system/` and removed `lib/` and
the retired `ctl/` root.
NEW.18 created `system/engine_bridge` as the active C bridge into the Rust
engine and moved the Rust engine C shim there.
NEW.18A added case-world binding records. SPINE.3R rebased the target before
NEW.19 so guards could align with case-world-first ordering and live
operational data planes. NEW.19 guard/build realignment is done. Local install
layout is next at NEW.20.

NEW.13 is retained as the planning wave. NEW.14 is the first physical move
wave; NEW.15 is the command move wave; NEW.16 is the daemon move wave; NEW.17
is the C system move wave; NEW.18 is the engine bridge split wave; SPINE.3R
preceded the completed NEW.19 Makefile/build/guards realignment.

## Model Provider Experiment Boundary

Filesystem refoundation comes before canonical model/provider experiments.
Provider scouting may begin immediately at L0 outside the core, but it is not
canonical validation and does not change the target filesystem.

The first core roadmap positions are:

```text
after NEW.30 data plane milestone freeze
```

SPINE.2 must not create a model runner, provider registry, model carrier,
policy packs, `ai-environment` repo, interface surface or console surface.

## Final Source Placement

After NEW.21:

```text
.c allowed under system/, cmd/yaid/ and tests/
.h allowed under include/yai/ and system/internal/ for private headers
.rs allowed under engine/ and cmd/yai/
```

`lib/`, `ctl/`, `crates/` and top-level `daemon/` are retired roots. The C
data logic under `system/{store,graph,index,memory,projection,reconcile}` is
classified `keep_temporarily` after NEW.18. Rust engine ownership is the target;
`system/engine_bridge` is the active C bridge only.

SPINE.3R extends the target split:

```text
system = host/control/carrier/daemon/bridge
engine = hot/store/journal/record/graph/index/query/memory/projection/reconcile
```

Shared memory, LMDB, Ladybug and DuckDB are planned-not-created until their
dedicated data-plane waves.
