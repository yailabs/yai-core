# ctl

`yai` is the canonical technical control command for `yai-core`.

Transitional role: this root is a bootstrap pointer only. The current Rust
implementation lives in `crates/yai-ctl`.

Target role: NEW.15 moves the implementation to:

```text
cmd/yai
```

After `cmd/yai` exists, `ctl/` should be removed or left only as a short pointer
README until compatibility drains. Local install layout is delayed to NEW.20.

Console UX belongs outside this repository.
