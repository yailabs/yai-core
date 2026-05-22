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

Bootstrap roots are now retired except for transitional data logic that remains
under `system/` until NEW.18.

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
docs/engineering/new13-filesystem-refactor-plan.md
docs/engineering/filesystem-move-matrix.md
docs/engineering/engine-ownership-map.md
docs/engineering/system-ownership-map.md
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
NEW.17 C implementation moved to system/
```

NEW.14 moved only the Rust data engine crates into `engine/`. NEW.15 moved the
Rust technical command into `cmd/yai` and removed `crates/`. Local install
NEW.16 moved `yaid` into `cmd/yaid` and daemon support into `system/daemon`.
NEW.17 moved remaining C implementation into `system/` and removed `lib/` and
the retired `ctl/` root.
Local install layout is delayed to NEW.20. Do not install a filesystem shape
that is already known to be transitional.

NEW.13 is retained as the planning wave. NEW.14 is the first physical move
wave; NEW.15 is the command move wave; NEW.16 is the daemon move wave; NEW.17
is the C system move wave.

## Model Provider Experiment Boundary

Filesystem refoundation comes before canonical model/provider experiments.
Provider scouting may begin immediately at L0 outside the core, but it is not
canonical validation and does not change the target filesystem.

The first core roadmap positions are:

```text
NEW.26 naked local model case experiment v0
NEW.28 model carrier v0
NEW.29 agent trace/tool call import v0
NEW.30 policy compliance experiment harness v0
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

`lib/`, `ctl/`, `crates/` and top-level `daemon/` are retired roots. The
remaining temporary exception is duplicated data logic under
`system/{store,graph,index,memory,projection,reconcile}` until NEW.18.
