# Review Loop Test Matrix

Status: SPINE.44C Review Loop Test Matrix + Lab Alignment.

SPINE.44C aligns tests and labs around the review/deferred/quarantine control
loop. It adds no new review semantics. The purpose is to prove that
`require_review` is an active control state and that CLI automation can observe
and resolve it without confusing prompt text, model proposals or carrier
execution.

The active lab paths are `labs/filesystem-loop` and
`labs/filesystem-loop`.

## Matrix

The filesystem matrix covers:

- blocked outside-sandbox write with `carrier_attempted: false`,
  `execution_performed: false` and blocked receipt residue.
- pending inside-sandbox mutative write with `pending_operator`,
  `review_id` and `next_commands`.
- `approve` with `decision: allow_with_constraints`,
  `carrier_attempted: true`, `execution_performed: true` and executed receipt.
- `deny`, `defer` and `quarantine` with `carrier_attempted: false` and
  `execution_performed: false`.
- CLI `wait timeout`, resolved wait, bounded `watch`, and non-TTY interactive
  handling.

Expected labels:

```text
review_matrix:blocked ok
review_matrix:pending ok
review_matrix:approve executed ok
review_matrix:deny no_execution ok
review_matrix:defer no_execution ok
review_matrix:quarantine no_execution ok
review_matrix:next_commands ok
review_matrix:wait_timeout ok
review_matrix:wait_resolved ok
review_matrix:watch ok
review_matrix:non_tty ok
```

## Model Boundary

Model-behavior tests must distinguish projection boundary, model proposal,
control pending/review, carrier execution and receipt. A model output is
`claim_or_proposal`, not authority.

Expected posture:

```text
model proposal observed
model cannot approve
model cannot mutate decision
model cannot execute carrier
automatic proposed-op gate import is future work
```

Until automatic proposed-op gate import exists, a forbidden filesystem proposal
from a model remains observed evidence. YAI may later import it into an
`op_attempt`, but this wave does not implement that path.

Validation:

```bash
make check-review-loop-test-matrix
make smoke-spine44c
```

Guard vocabulary: approve, deny, defer, quarantine, pending_operator,
next_commands, carrier_attempted: false, execution_performed: false, wait
timeout, watch, model proposal observed, model cannot approve, automatic
proposed-op gate import is future work, labs/filesystem-loop,
labs/filesystem-loop.
