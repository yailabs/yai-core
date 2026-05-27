# SPINE.48 - Model Behavior / Policy Outcome Facts

Status: completed.

SPINE.48 extends the DuckDB fact plane with the first behavior/control
extraction beyond receipt, decision and projection facts.

## Scope

Extracted analytical tables:

```text
fact_model_behavior
fact_policy_outcome
```

Command surface:

```text
yai facts extract --case <case_ref> --kind model_behavior
yai facts extract --case <case_ref> --kind policy_outcome
yai facts extract --case <case_ref> --kind behavior
yai facts extract --case <case_ref> --kind all
yai facts summary --case <case_ref>
```

Guard vocabulary: kind model_behavior, kind policy_outcome, kind behavior.

`core` remains receipt + decision + projection. `behavior` means
model_behavior + policy_outcome. `all` means core + behavior.

## Doctrine

Facts are not truth. Model behavior facts measure model output posture. Policy
outcome facts measure policy/control posture. These facts do not approve, deny,
execute, mutate, import model proposals as actions or replace policy authority.

Canonical boundaries:

```text
model proposal is not execution
model cannot approve
automatic proposed-op gate import is future work
```

`authority_overclaim`, `unsupported_claim`, `review_required` and
`policy_outcome` are analytical fields only. SPINE.48 uses structured record
fields and stable string markers only. No LLM-based classification is used.

## Bitemporal Posture

Extracted facts keep the `yai.fact.v1` bitemporal shape:

```text
transaction_time
valid_time_start
valid_time_end
known_at
status
revision_of
superseded_by
retracted_by
```

`valid_time_end` sentinel: 0 means open-ended. Revision fields remain present
but empty; fact revision/supersession remains future work.

## Validation

Expected smoke labels:

```text
facts_extract:model_behavior ok
facts_extract:policy_outcome ok
facts_extract:behavior ok
facts_extract:idempotent ok
facts_summary:behavior_counts ok
facts:not_truth ok
```

The filesystem-loop smoke requires `fact_policy_outcome > 0`.
`fact_model_behavior` extraction is honest and may be zero when the case has no
model records.
