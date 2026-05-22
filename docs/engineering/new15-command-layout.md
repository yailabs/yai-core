# NEW.15 Command Layout

Status: NEW.15 physical refactor wave.

NEW.15 moves the canonical Rust technical command from the transitional crate
root into the target command root:

```text
crates/yai-ctl -> cmd/yai
```

`cmd/yai` is the local technical control command for `yai-core`. It is not the
Console, not an operator TUI and not the product UX command surface. Console
command UX remains outside this repository.

## Current Layout

```text
engine/
  yai-engine/
  yai-engine-ffi/

cmd/
  yai/
    Cargo.toml
    src/main.rs
```

`crates/` is removed after NEW.15. No Rust source should remain under
`crates/`.

## Build Shape

NEW.15 keeps separate manifests to avoid nested workspace confusion:

```text
engine/Cargo.toml      # engine workspace
cmd/yai/Cargo.toml     # standalone technical command crate
```

The Makefile builds both with `CARGO_TARGET_DIR=target`, so the dev command is:

```text
target/debug/yai
```

## Boundaries

`yai` is Rust. NEW.16 moves `yaid` to `cmd/yaid` and daemon support to
`system/daemon`.

No daemon protocol, C ABI, C source, engine source or smoke behavior changes in
NEW.15.

## Audit Result

Read-only audit of old operator/runtime material classifies operator-facing
manuals and Console command UX as future `console` material. Runtime tooling
concepts may inform `cmd/yai` or daemon tooling, but no old source is copied.
