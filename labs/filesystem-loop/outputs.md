# Filesystem Loop Outputs

Status: canonical output anchors.

## Receipt / Decision / Projection Facts

Receipt extraction:

```text
facts_extract:
kind: receipt
status: completed
table: fact_receipt
schema: yai.fact.v1
facts_are_truth: false
```

Decision extraction:

```text
facts_extract:
kind: decision
status: completed
table: fact_decision
schema: yai.fact.v1
facts_are_truth: false
```

Projection extraction:

```text
facts_extract:
kind: projection
status: completed
table: fact_projection
schema: yai.fact.v1
facts_are_truth: false
```

Core extraction:

```text
facts_extract:
kind: core
status: completed
fact_receipt_written: N
fact_decision_written: N
fact_projection_written: N
facts_duplicate: N
facts_are_truth: false
```

Summary:

```text
facts_summary:
case_ref: case:new12-filesystem
fact_receipt: N
fact_decision: N
fact_projection: N
facts_total: N
facts_are_truth: false
```

Second extraction is idempotent extraction and should report duplicates.
Deterministic fact IDs use `fact:<kind>:<source_record_id>`.
`transaction_time`, `valid_time_start`, `valid_time_end`, `known_at` and
revision fields remain part of the row shape. valid_time_end sentinel: 0 means
open-ended. Facts are not truth. No fact revision is implemented in SPINE.47.

## Model Behavior / Policy Outcome Facts

Model behavior extraction:

```text
facts_extract:
kind: model_behavior
status: completed
table: fact_model_behavior
schema: yai.fact.v1
facts_are_truth: false
```

Policy outcome extraction:

```text
facts_extract:
kind: policy_outcome
status: completed
table: fact_policy_outcome
schema: yai.fact.v1
facts_are_truth: false
```

Behavior extraction:

```text
facts_extract:
kind: behavior
status: completed
fact_model_behavior_written: N
fact_policy_outcome_written: N
facts_duplicate: N
facts_are_truth: false
```

Summary:

```text
facts_summary:
case_ref: case:new12-filesystem
fact_receipt: N
fact_decision: N
fact_projection: N
fact_model_behavior: N
fact_policy_outcome: N
facts_total: N
facts_are_truth: false
```

facts are not truth. model proposal is not execution. model cannot approve.
automatic proposed-op gate import is future work. `authority_overclaim`,
`unsupported_claim`, `review_required` and `policy_outcome` are analytical
fields. No LLM-based classification is used.

## Memory / Divergence / Carrier Facts

Carrier outcome extraction:

```text
facts_extract:
kind: carrier_outcome
status: completed
table: fact_carrier_outcome
schema: yai.fact.v1
facts_are_truth: false
```

Divergence extraction:

```text
facts_extract:
kind: divergence
status: completed
table: fact_divergence
schema: yai.fact.v1
facts_are_truth: false
```

Memory quality extraction:

```text
facts_extract:
kind: memory_quality
status: completed
table: fact_memory_quality
schema: yai.fact.v1
facts_are_truth: false
memory_is_truth: false
```

Operational extraction:

```text
facts_extract:
kind: operational
status: completed
fact_carrier_outcome_written: N
fact_divergence_written: N
fact_memory_quality_written: N
facts_duplicate: N
facts_are_truth: false
```

Summary:

```text
facts_summary:
case_ref: case:new12-filesystem
fact_carrier_outcome: N
fact_divergence: N
fact_memory_quality: N
facts_total: N
facts_are_truth: false
```

carrier facts measure carrier posture. divergence facts are not reconcile
action. memory facts are not memory. facts are not truth. Extraction is
bitemporal and idempotent extraction over memory_candidate and carrier records.
