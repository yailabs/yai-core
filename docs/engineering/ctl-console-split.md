# Ctl / Console Split

`yai` and Console are different products.

## `yai`

Technical core control CLI.

Owns:

```text
daemon status
case open/list/status
subject bind/list
op submit
control decide
decision inspect
receipt show
store tail
memory status/query
projection request
carrier probe
debug dump
```

## Console

Human terminal client in the sibling `console` repo.

Owns:

```text
CLI UX
TUI UX
help and output format UX
interactive shell UX
runtime attachment UX
```

## Rule

Console consumes core operations and projections. `yai` inspects and controls core primitives. Neither should redefine protocol truth; that belongs to core plus `interfaces`.

## NEW.11 Daemon IPC

NEW.11 lets `yai` talk to `yaid` for technical daemon status, info and
shutdown over a local Unix socket.

This does not make `yai` the Console UX and does not expose case/op execution
over IPC. It proves the local service boundary that Console and interfaces can
consume later.

## NEW.12 Daemon-Backed Loop

NEW.12 lets `yai` request bounded daemon operations:

```text
daemon run-minimum-loop
daemon run-filesystem-loop
daemon journal-summary
daemon projection-summary
```

`yai` is still only the technical client. `yaid` invokes the C core path and
returns structured residue counts. Console remains a sibling repo concern and
must consume future interfaces/projections instead of absorbing this technical
CLI shape as product UX.

## NEW.13 Local Command

NEW.13 makes `yai` the installed technical command:

```text
make install-local
yai info
yai doctor
yai daemon status --socket <path>
```

This is a local developer command surface, not Console. Console may later wrap
or consume interfaces/projections, but it does not define core command truth in
this wave.
