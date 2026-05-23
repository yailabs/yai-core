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
vendor/
```

`include/` owns public C ABI contracts.
`system/` owns C system boundary code: daemon, host boundary, carriers,
control shell and engine bridge.
`engine/` owns Rust operational data engine code.
`cmd/` owns local binaries: `yai` and `yaid`.
`proto/`, `tests/`, `docs/` and `tools/` own schemas, validation, docs and
developer utilities.

## Removed Active Roots

These roots are not active implementation roots:

```text
lib/
crates/
daemon/
ctl/
```

Historical references are archived under `docs/archive/engineering/`.

## Target Runtime Layout

SPINE.20 defines the local runtime layout. It must introduce a normal local
operating environment instead of treating `build/tmp` as the user runtime.

Target concepts:

```text
YAI_HOME
run/
store/
log/
tmp/
socket path
installed yai
installed yaid
doctor output
cleanup/uninstall
manual daemon start/stop/status
```

## Transitional Caveats

The C data-spine folders under `system/{store,graph,index,memory,projection,reconcile}`
remain transitional smoke support until the Rust engine owns the mature data
planes.

The current build tree is not the target local runtime environment.
