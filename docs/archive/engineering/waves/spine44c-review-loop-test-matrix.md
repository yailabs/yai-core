Historical/superseded engineering record. Not an active source of truth.

# SPINE.44C Review Loop Test Matrix

Status: archived delivery note.

SPINE.44C aligns the review loop test matrix across smoke tests and lab docs.
The wave does not add review semantics; it makes the SPINE.44A/SPINE.44B loop
visible and reproducible before the graph/runtimegraph freeze.

Covered behavior:

- outside sandbox write is blocked.
- review-required write reaches `pending_operator` and prints `next_commands`.
- `approve`, `deny`, `defer` and `quarantine` are separately validated.
- non-execution states keep `carrier_attempted: false` and
  `execution_performed: false`.
- CLI `wait timeout`, resolved wait, bounded `watch`, and non-TTY interactive
  handling are validated.
- `docs/labs/filesystem-loop` and `docs/labs/filesystem-loop` describe model
  proposal boundaries.

Model boundary:

```text
model proposal observed
model cannot approve
model cannot mutate decision
model cannot execute carrier
automatic proposed-op gate import is future work
```

Guard vocabulary: approve, deny, defer, quarantine, pending_operator,
next_commands, carrier_attempted: false, execution_performed: false, wait
timeout, watch, model proposal observed, model cannot approve, automatic
proposed-op gate import is future work, docs/labs/filesystem-loop,
docs/labs/filesystem-loop.
