# Ctl / Console Split

`yaictl` and Console are different products.

## `yaictl`

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

Console consumes core operations and projections. `yaictl` inspects and controls core primitives. Neither should redefine protocol truth; that belongs to core plus `interfaces`.
