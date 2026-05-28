# Filesystem Loop Outputs

Status: canonical output anchors.

Related benchmark:

```text
Test 2 - labs/context-residency
```

Test 2 is the case-native benchmark matrix: naked no-context, naked raw-context,
naked mini-RAG, YAI case-bound and logical YAI residency.

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

## Fact Reports CLI

Default report:

```text
facts_report:
case_ref: case:new12-filesystem
schema: yai.fact.v1
facts_are_truth: false
sections:
  receipts:
  decisions:
  projections:
  policy:
  carriers:
  divergence:
  memory:
  model:
```

Section postures:

```text
yai facts report --case case:new12-filesystem --section receipts
yai facts report --case case:new12-filesystem --section decisions
yai facts report --case case:new12-filesystem --section projections
yai facts report --case case:new12-filesystem --section policy
yai facts report --case case:new12-filesystem --section carriers
yai facts report --case case:new12-filesystem --section divergence
yai facts report --case case:new12-filesystem --section memory
yai facts report --case case:new12-filesystem --section model
```

Expected zero/source-absence posture:

```text
divergence:
  status: none_observed
memory:
  memory_is_truth: false
model:
  status: no_model_records
```

facts reports are not truth. facts reports are not audit packets. facts reports
are not reconcile actions. facts reports are not memory consolidation.

## Fact Plane Freeze

Frozen filesystem-loop summary after installed matrix:

```text
facts_summary:
case_ref: case:new12-filesystem
fact_receipt: 3
fact_decision: 2
fact_projection: 3
fact_carrier_outcome: 3
fact_divergence: 0
fact_model_behavior: 0
fact_policy_outcome: 7
fact_memory_quality: 1
facts_total: 19
facts_are_truth: false
memory_is_truth: false
```

Frozen report absence posture:

```text
divergence:
  total: 0
  status: none_observed

model:
  total: 0
  status: no_model_records

memory:
  memory_is_truth: false
```

DuckDB `yai.fact.v1` facts remain bitemporal and provenance-bearing:
`transaction_time`, `valid_time_start`, `valid_time_end`, `known_at`,
`revision_of`, `superseded_by`, `retracted_by`. Status posture includes
current, superseded, retracted, stale, contested, historical_only, branch_only,
counterfactual, needs_review and unknown. Fact Plane Freeze keeps
`fact_receipt`, `fact_decision`, `fact_projection`, `fact_carrier_outcome`,
`fact_divergence`, `fact_model_behavior`, `fact_policy_outcome` and
`fact_memory_quality`. `facts summary` and `facts report` are analytical only.
Extraction is idempotent extraction.

## CaseHandle / CapabilityLease Outputs

SPINE.51B adds runtime-resolved boundary outputs for the filesystem loop:

```bash
yai case resolve --case case:new12-filesystem --subject subject:llm-provider
yai case scope --case case:new12-filesystem --subject subject:llm-provider
yai capability derive --case case:new12-filesystem --subject subject:llm-provider --operation filesystem.write --resource sandbox/output.txt
```

Expected posture:

```text
CaseHandle
SubjectHandle
AuthorityScope
VisibilityScope
ResourceScope
CapabilityLease
refs are identifiers, not authority
bindings are relations, not capabilities
carrier dispatch allowed
subject_lacks_execute_authority
resource_outside_scope
```

`subject:llm-provider` cannot execute or approve filesystem writes. The
filesystem sandbox subject can read inside sandbox, requires review for
inside-sandbox writes and is denied outside sandbox.
