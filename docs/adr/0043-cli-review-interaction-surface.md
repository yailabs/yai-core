# ADR 0043: CLI Review Interaction Surface

Status: accepted.

## Decision

The review loop is a durable control state. CLI interaction is a rendering and
resolution surface over that state. Backend operations must not block on
terminal input; interactive CLI is optional and TTY-scoped.

`yai control pending` and `yai control show` are automation-friendly views.
`yai control review --interactive` is available only when stdin is a TTY and
returns `not_a_tty` otherwise. `yai control watch` uses bounded polling and
`yai control wait` supports scriptable timeout behavior.

Authority boundary:

```text
subject:linenoise-terminal is prompt surface
subject:operator-reviewer is review authority
```

`approve` may execute the reviewed filesystem carrier. `deny`, `defer` and
`quarantine` keep `carrier_attempted: false` and `execution_performed: false`.

The active lab path is `docs/labs/filesystem-loop`.

Guard vocabulary: control pending, control show, control review --interactive,
control watch, control wait, next_commands, not_a_tty, pending_operator,
approve, deny, defer, quarantine, carrier_attempted: false,
execution_performed: false, subject:linenoise-terminal is prompt surface,
subject:operator-reviewer is review authority, docs/labs/filesystem-loop.
