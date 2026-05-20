# NEW.12 Daemon-Backed Core Loop

NEW.12 moves the first controlled core loop behind `yaid`.

The daemon is still local-only and minimal. It does not expose a public API,
HTTP transport, auth, multi-client runtime, process carrier or model carrier.
It proves that `yaictl` can ask a resident `yaid` process to run a bounded core
path and receive a structured response.

## Loop

```text
yaictl
-> Unix socket IPC
-> yaid
-> case open
-> subject bind
-> op attempt
-> control decision
-> receipt
-> journal append
-> graph edge
-> memory candidate
-> projection result
-> response envelope
```

## IPC Requests

NEW.12 extends the NEW.11 request set:

```text
status
info
shutdown
run_minimum_loop
run_filesystem_loop
journal_summary
projection_summary
```

The line-oriented request envelope carries:

```text
request_id
request_kind
case_ref
subject_ref
payload
```

The response envelope reports:

```text
request_id
ok
version
status
message
journal_path
record_count
receipt_count
projection_count
decision_outcome
receipt_status
```

Filesystem loop responses also report `fs_read`, `fs_write_blocked` and
`fs_write_allowed`.

## Boundaries

`yaid` owns the daemon endpoint and invokes C core primitives. `yaictl` remains
a Rust client and does not own the loop semantics.

The daemon-backed filesystem loop writes only inside an isolated
`build/tmp/new12/.../filesystem/sandbox/` directory. The journal remains the
truth path for this wave.

## Commands

```text
build/yaid --socket build/tmp/new12/manual/yaid.sock --foreground
crates/target/debug/yaictl daemon run-minimum-loop --socket build/tmp/new12/manual/yaid.sock
crates/target/debug/yaictl daemon run-filesystem-loop --socket build/tmp/new12/manual/yaid.sock
crates/target/debug/yaictl daemon journal-summary --socket build/tmp/new12/manual/yaid.sock --journal <journal>
crates/target/debug/yaictl daemon projection-summary --socket build/tmp/new12/manual/yaid.sock --journal <journal>
```

`interfaces` and `console` are not touched in NEW.12. They will later consume
daemon/projection contracts instead of redefining core truth.
