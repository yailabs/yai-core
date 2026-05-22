# crates

Transitional role: after NEW.14 this root contains only the bootstrap Rust
workspace for the current `yai` command.

Target role:

```text
crates/yai-ctl             -> cmd/yai
```

NEW.14 moved the engine crates to:

```text
engine/yai-engine
engine/yai-engine-ffi
```

Owning wave: NEW.15 moves the command. After NEW.21 there should be no Rust
implementation under `crates/`.
