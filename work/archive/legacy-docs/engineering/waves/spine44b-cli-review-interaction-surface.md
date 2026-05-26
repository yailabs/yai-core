Historical/superseded engineering record. Not an active source of truth.

# SPINE.44B CLI Review Interaction Surface

Status: delivered.

SPINE.44B adds practical CLI interaction over the durable SPINE.44A review
state. `control pending` includes `next_commands`, `control show` exposes the
decision context and authority boundary, `control review --interactive` prompts
only in a TTY, `control watch` polls review state and `control wait` supports
automation timeouts.

The backend creates durable review/control state. The CLI renders and resolves
that state. Backend operations do not block on terminal input.

Authority boundary:

```text
subject:linenoise-terminal is prompt surface
subject:operator-reviewer is review authority
```

`approve` may execute the safe filesystem carrier. `deny`, `defer` and
`quarantine` keep `carrier_attempted: false` and `execution_performed: false`.

Validation:

```bash
make check-cli-review-interaction-surface
make smoke-spine44b
```

Guard vocabulary: control pending, control show, control review --interactive,
control watch, control wait, next_commands, not_a_tty, pending_operator,
approve, deny, defer, quarantine, carrier_attempted: false,
execution_performed: false, subject:linenoise-terminal is prompt surface,
subject:operator-reviewer is review authority, labs/filesystem-loop.
