# Filesystem Target

This is the active filesystem/layout document.

## Current Root Layout

```text
include/
system/
engine/
cmd/
proto/
tests/
docs/
tools/
packaging/
examples/
vendor/
```

`include/` owns public C ABI contracts.
`system/` owns C system boundary code: daemon, host boundary, carriers,
control shell and engine bridge.
`engine/` owns Rust operational data engine code.
`cmd/` owns local binaries: `yai` and `yaid`.
`proto/`, `tests/`, `docs/` and `tools/` own schemas, validation, docs and
developer utilities. `packaging/` owns package source material. `examples/`
owns non-authoritative examples. `vendor/` owns vendored support code.

Pack format doctrine lives under `docs/engineering/pack-format.md`. SPINE.21
does not create pack source roots, pack registries, pack installer paths or
pack runtime directories.

## Removed Active Roots

These roots are not active implementation roots:

```text
lib/
crates/
daemon/
ctl/
```

Historical references are archived under `docs/archive/engineering/`.

## Local Runtime Layout

SPINE.20 defines the local runtime layout and is now the active installed
runtime baseline.

Rule:

```text
build/tmp = test/lab
YAI_HOME = real local runtime
```

Target concepts:

```text
YAI_HOME
run/
store/
log/
tmp/
cases/
sockets/
config/
run/yaid.sock
installed yai
installed yaid
doctor output
cleanup/uninstall
manual daemon start/stop/status
```

Default local paths:

```text
PREFIX=$HOME/.local
YAI_HOME=$HOME/.yai
$(PREFIX)/bin/yai
$(PREFIX)/bin/yaid
$(YAI_HOME)/run
$(YAI_HOME)/store
$(YAI_HOME)/log
$(YAI_HOME)/tmp
$(YAI_HOME)/cases
$(YAI_HOME)/sockets
$(YAI_HOME)/config
$(YAI_HOME)/run/yaid.sock
$(YAI_HOME)/run/hot-state.json
```

`run/` is the ephemeral daemon/runtime directory and owns the default socket
and hot-state snapshot.
`store/` is the future durable data-plane root. `cases/` is the future
case-session storage root. `log/`, `tmp/` and `config/` hold local runtime
logs, scratch files and configuration material.

Future data-plane locations may live under:

```text
$(YAI_HOME)/store/journal/
$(YAI_HOME)/store/lmdb/
$(YAI_HOME)/store/ladybug/
$(YAI_HOME)/store/duckdb/
```

SPINE.20 creates only the top-level runtime directories. It does not implement
LMDB, Ladybug, DuckDB or shared memory.

SPINE.22 freezes this host/runtime layout. SPINE.23 adds
`YAI_HOME/run/hot-state.json` as runtime cache material. It is not durable
truth and may be rebuilt from residue.

SPINE.24 hardens that runtime file as a schema-versioned snapshot:

```text
$(YAI_HOME)/run/hot-state.json
schema: yai.hot_state.v1
write: temp file then rename
```

SPINE.25 makes the snapshot case-session aware:

```text
case_session_status
case_world_status
case_context_status
active_thread_id
participant_view_frame_id
```

## Transitional Caveats

The C data-spine folders under `system/{store,graph,index,memory,projection,reconcile}`
remain transitional smoke support until the Rust engine owns the mature data
planes.

The build tree is not the target local runtime environment.
