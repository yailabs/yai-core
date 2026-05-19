# C / Rust Boundary

Canonical decision:

```text
C owns public ABI, daemon bootstrap, carrier boundary and control v0.
Rust owns advanced operational data engine behind FFI.
yaictl may be Rust, but it remains a client over primitives.
```

Rust is not the new semantic owner of the core. Public contracts stay in the C
ABI and protocol docs.

## Owner Table

| Concern | Owner |
|---|---|
| daemon lifecycle | C |
| carrier v0 | C |
| control v0 | C |
| public ABI | C |
| `yaictl` | Rust allowed, client only |
| advanced store engine | Rust |
| graph/index/memory | Rust |
| projection cache | Rust |

## FFI Rules

```text
opaque handles
explicit alloc/free
caller-owned buffers or byte buffers
integer status codes
schema-versioned records
deterministic error strings
no Rust types across public ABI
```

## Non-Goal

Do not port the whole core to Rust before NEW.1 proves the loop. Rust is the
operational data engine, not the first daemon/control shell.
