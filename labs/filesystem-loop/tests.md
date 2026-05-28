# Filesystem Loop Tests

Status: canonical lab test matrix.

## Fact Plane Extraction

SPINE.47 tests the first real fact extraction from LMDB records into DuckDB:

```bash
yai facts init
yai facts extract --case case:new12-filesystem --kind receipt
yai facts extract --case case:new12-filesystem --kind decision
yai facts extract --case case:new12-filesystem --kind projection
yai facts extract --case case:new12-filesystem --kind core
yai facts summary --case case:new12-filesystem
```

Expected posture:

```text
fact_receipt > 0
fact_decision > 0
fact_projection > 0
facts_are_truth: false
```

Facts are not truth. Fact extraction is derivation, not migration. Records
remain truth in LMDB. Deterministic fact IDs use:

```text
fact:<kind>:<source_record_id>
```

Second extraction must be duplicate-aware idempotent extraction and report
`facts_duplicate` instead of inserting duplicate rows.

Bitemporal fields are populated:

```text
transaction_time
valid_time_start
valid_time_end
known_at
```

Revision fields remain present but empty until future revision work:

```text
revision_of
superseded_by
retracted_by
```

valid_time_end sentinel: 0 means open-ended. No fact revision is implemented in
SPINE.47.

## Model Behavior / Policy Outcome Facts

SPINE.48 tests derived analytical behavior facts:

```bash
yai facts extract --case case:new12-filesystem --kind model_behavior
yai facts extract --case case:new12-filesystem --kind policy_outcome
yai facts extract --case case:new12-filesystem --kind behavior
yai facts extract --case case:new12-filesystem --kind all
yai facts summary --case case:new12-filesystem
```

Expected posture:

```text
fact_policy_outcome > 0
fact_model_behavior >= 0
facts_are_truth: false
```

`core` remains receipt + decision + projection. `behavior` means
model_behavior + policy_outcome. `all` means core + behavior. Guard
vocabulary: kind model_behavior, kind policy_outcome, kind behavior.

Model behavior facts measure output posture. Policy outcome facts measure
policy/control posture. facts are not truth. model proposal is not execution.
model cannot approve. automatic proposed-op gate import is future work.
`authority_overclaim`, `unsupported_claim`, `review_required` and
`policy_outcome` are analytical fields. No LLM-based classification is used.

## Memory / Divergence / Carrier Facts

SPINE.49 tests derived analytical operational facts:

```bash
yai facts extract --case case:new12-filesystem --kind carrier_outcome
yai facts extract --case case:new12-filesystem --kind divergence
yai facts extract --case case:new12-filesystem --kind memory_quality
yai facts extract --case case:new12-filesystem --kind operational
yai facts extract --case case:new12-filesystem --kind all
yai facts summary --case case:new12-filesystem
```

Expected posture:

```text
fact_carrier_outcome > 0
fact_memory_quality > 0 when memory_candidate exists
fact_divergence >= 0
facts_are_truth: false
memory_is_truth: false
```

`operational` means carrier_outcome + divergence + memory_quality. `all` means
core + behavior + operational. Guard vocabulary: kind carrier_outcome, kind
divergence, kind memory_quality, kind operational.

carrier facts measure carrier posture. divergence facts are not reconcile
action. memory facts are not memory. facts are not truth. The extraction is
bitemporal and idempotent extraction.

## Fact Reports CLI

SPINE.50 tests compact read-only report sections over extracted facts:

```bash
yai facts report --case case:new12-filesystem
yai facts report --case case:new12-filesystem --section receipts
yai facts report --case case:new12-filesystem --section decisions
yai facts report --case case:new12-filesystem --section projections
yai facts report --case case:new12-filesystem --section policy
yai facts report --case case:new12-filesystem --section carriers
yai facts report --case case:new12-filesystem --section divergence
yai facts report --case case:new12-filesystem --section memory
yai facts report --case case:new12-filesystem --section model
```

`facts summary` is counts. `facts report` is interpreted compact sections.
facts reports are not truth, not audit packets, not reconcile actions and not
memory consolidation. `none_observed` is valid for zero divergence facts.
`no_model_records` is valid for zero model facts. Memory report output keeps
`memory_is_truth: false`.
