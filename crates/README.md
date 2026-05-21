# crates

Transitional role: this root is the bootstrap Rust workspace for the current
`yai` command and Rust engine R1.

Target role:

```text
crates/yai-core-engine     -> engine/yai-engine
crates/yai-core-engine-sys -> engine/yai-engine-ffi
crates/yai-ctl             -> cmd/yai
```

Owning waves: NEW.14 moves the engine workspace, and NEW.15 moves the command.
After NEW.21 there should be no Rust implementation under `crates/`.
