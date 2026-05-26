# CLI Review Interaction Surface

Status: SPINE.44B CLI Review Interaction Surface.

SPINE.44B keeps review truth in durable records and makes the CLI a rendering
and resolution surface. The backend does not wait on terminal input; scripts
can inspect pending work and choose a separate command.

Commands:

```bash
yai control pending --case <case_ref>
yai control show <review_id>
yai control review --case <case_ref> --interactive
yai control watch --case <case_ref> --interval-ms 500 --max-events 5
yai control wait <review_id> --timeout 1
yai control approve <review_id> --reason "<reason>"
yai control deny <review_id> --reason "<reason>"
yai control defer <review_id> --reason "<reason>"
yai control quarantine <review_id> --reason "<reason>"
```

Automation mode prints `pending_operator`, `review_id` and `next_commands` and
exits. Interactive mode is TTY-scoped; without a TTY it returns
`not_a_tty` and points the user back to `yai control pending --case <case_ref>`.
Watch mode is polling only. Wait mode reports resolved or timeout state for
automation.

Plain output remains stable and parseable. `--format plain|json|pretty` is
reserved for future expansion; v0 does not require JSON.

Authority boundary:

```text
subject:linenoise-terminal is prompt surface
subject:operator-reviewer is review authority
```

`approve` may execute the safe reviewed filesystem write. `deny`, `defer` and
`quarantine` keep `carrier_attempted: false` and `execution_performed: false`.

Validation:

```bash
make check-cli-review-interaction-surface
make smoke-spine44b
```

The active lab path is `docs/labs/filesystem-loop`.

Guard vocabulary: control pending, control show, control review --interactive,
control watch, control wait, next_commands, not_a_tty, pending_operator,
approve, deny, defer, quarantine, carrier_attempted: false,
execution_performed: false, subject:linenoise-terminal is prompt surface,
subject:operator-reviewer is review authority, docs/labs/filesystem-loop.

SPINE.45 freezes the graph-facing visibility of CLI review state:
`review_request`, `review_decision` and `control_pending` are available to
graph materialize and RuntimeGraph query when actual refs exist.
