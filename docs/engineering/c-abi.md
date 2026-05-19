# C ABI

The C ABI is the stable machine boundary for the first core.

Public C headers define stable machine contracts. Internal C files and Rust
crates may change behind those contracts.

## 1. Owns

```text
public headers
opaque refs
status and error codes
daemon bootstrap ABI
carrier request/receipt structs
FFI boundary shims
serialization entry points
```

The ABI owns public semantics. It does not require every implementation detail
to stay in C forever. `yaictl` may later be Rust, but it remains a client over
these primitives.

## 2. ABI Versioning

Every public ABI surface should expose:

```text
YAI_ABI_VERSION_MAJOR
YAI_ABI_VERSION_MINOR
YAI_ABI_VERSION_PATCH
yai_abi_version()
```

Compatibility rule:

```text
major changes can break ABI
minor changes can add fields only through size/version aware structs
patch changes must not change public layout or behavior contracts
```

Every public struct that crosses a dynamic boundary must include:

```text
uint32_t abi_version
uint32_t struct_size
```

## 3. Struct Layout Rules

```text
fixed-width integer types
explicit enum widths where possible
no compiler-dependent bitfields
no public flexible ownership surprises
no Rust types across public ABI
reserved fields for forward compatibility
struct_size checked before reading optional fields
```

Public structs carry data contracts. Internal structs can change freely.

## 4. Opaque Handle Policy

Opaque handles are preferred for long-lived objects:

```text
yai_case_t
yai_subject_t
yai_store_t
yai_projection_t
yai_engine_t
```

Rules:

```text
create/open returns handle or status
close/free is explicit
handle ownership is documented at creation
NULL handles are rejected with deterministic status
handles do not expose internal C or Rust layout
```

## 5. Memory Ownership

Memory rules must be visible in function names or docs.

Allowed patterns:

```text
caller-owned output buffer
explicit yai_free for owned returned buffers
borrowed pointer valid only during call
opaque handle with explicit close
```

Forbidden patterns:

```text
hidden allocation without free path
callee storing pointer to caller stack memory
Rust allocation freed by plain free()
C allocation freed by Rust drop without shim
```

## 6. Buffer Model

Public APIs should prefer byte buffers for serialized records:

```text
const uint8_t *input
size_t input_len
uint8_t *output
size_t output_cap
size_t *output_len
```

If output is too small, return a deterministic status and required size:

```text
YAI_STATUS_BUFFER_TOO_SMALL
```

Text is UTF-8 unless explicitly documented otherwise.

## 7. Error Model

Errors use integer status codes plus optional deterministic error strings.

Minimum status classes:

```text
YAI_STATUS_OK
YAI_STATUS_INVALID_ARGUMENT
YAI_STATUS_NOT_FOUND
YAI_STATUS_DENIED
YAI_STATUS_DEFERRED
YAI_STATUS_BUFFER_TOO_SMALL
YAI_STATUS_CONFLICT
YAI_STATUS_UNSUPPORTED
YAI_STATUS_IO_ERROR
YAI_STATUS_INTERNAL_ERROR
```

Error strings are diagnostic, not control flow. Machine behavior must branch on
status codes and structured records.

## 8. Serialization Boundary

Structured domain records can cross implementation boundaries as
schema-versioned bytes.

Allowed serialization boundary:

```text
record schema version
content type
byte length
hash/integrity posture
status code
```

The ABI should not expose Rust structs, C internal structs, allocator internals
or database-specific row layouts.

## 9. Threading And Concurrency Posture

Default posture for NEW.1:

```text
handles are not thread-safe unless documented
store append is serialized by the owning store handle
callbacks are not invoked while internal locks are held
daemon owns event loop concurrency
```

Future concurrent handles must document reentrancy, lock ownership and callback
behavior.

## 10. Binary Compatibility Expectations

Public headers are the promise. Internal implementation can move from C to Rust
behind the same ABI when:

```text
status codes stay stable
serialized record schemas stay versioned
ownership rules stay stable
struct_size checks preserve old callers
test fixtures compare old and new backend behavior
```

## 11. First Headers

```text
include/yai/base/id.h
include/yai/base/error.h
include/yai/case/case_ref.h
include/yai/subject/subject_ref.h
include/yai/subject/subject_binding.h
include/yai/op/attempt.h
include/yai/control/decision.h
include/yai/effect/receipt.h
include/yai/store/record.h
include/yai/store/journal.h
include/yai/projection/projection.h
```
