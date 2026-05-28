# SPINE.49 - Memory / Divergence / Carrier Facts

SPINE.49 completes the first operational fact extraction block by deriving:

fact_carrier_outcome
fact_divergence
fact_memory_quality

Command surface:

yai facts extract --case <case_ref> --kind carrier_outcome
yai facts extract --case <case_ref> --kind divergence
yai facts extract --case <case_ref> --kind memory_quality
yai facts extract --case <case_ref> --kind operational
yai facts extract --case <case_ref> --kind all

core = receipt + decision + projection

behavior = model_behavior + policy_outcome

operational = carrier_outcome + divergence + memory_quality

all = core + behavior + operational

carrier facts measure carrier posture. divergence facts are not reconcile
action. memory facts are not memory. facts are not truth.

`fact_memory_quality` derives `memory_candidate` basis and quality posture only.
No memory consolidation, graph patching, reconcile action, carrier behavior
change or divergence generation change is implemented.

Bitemporal fields remain `transaction_time`, `valid_time_start`,
`valid_time_end`, `known_at`, `status`, `revision_of`, `superseded_by` and
`retracted_by`. Extraction is idempotent extraction with deterministic fact IDs:

fact:carrier_outcome:<source_record_id>
fact:divergence:<source_record_id>
fact:memory_quality:<source_record_id>

The filesystem-loop smoke requires fact_carrier_outcome > 0 and
fact_memory_quality > 0 when memory_candidate exists. fact_divergence may be
zero when no divergence records exist; no fake divergence facts are created.
