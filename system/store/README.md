# store

Transitional role: C bootstrap implementation for append-only records, journal
fallback and the Rust engine backend shim.

Target role:

```text
system/store/rust_engine_backend.c -> system/engine_bridge/rust_engine_backend.c
system/store/* data logic          -> engine/yai-engine/src/store + journal + record
```

Owning wave: NEW.18 splits bridge code from Rust data-spine ownership.
