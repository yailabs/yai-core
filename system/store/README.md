# store

Transitional role: C bootstrap implementation for append-only records and
journal fallback.

Target role:

```text
system/store/* data logic          -> engine/yai-engine/src/store + journal + record
```

Owning wave: NEW.18 moved the Rust bridge shim to `system/engine_bridge`.
The remaining store files are `keep_temporarily` until Rust data-spine parity.
