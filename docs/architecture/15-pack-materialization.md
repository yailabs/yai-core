# Pack Materialization

Status: SPINE.21 doctrine.

Packs are case materialization units.

A pack is not a marketplace product, DLC, prompt bundle or template folder. A
pack is a bounded way to materialize methods, procedures, policies, templates,
projection rules, output schemas, memory seeds, actors/tools and validation
fixtures into a case.

YAI does not force a method on the user. YAI materializes the user's method
inside controlled AI cases.

## Spine Position

```text
ingest
-> pack inspection / validation
-> pack materialization
-> case_world
-> case_domain
-> case_attachment
-> case_binding
-> policy_material / policy_rule
-> projection_rule
-> authority_scope
-> case_session / case_context
-> projection
-> control
-> receipt
-> record store / graph / facts / memory
```

Pack material is not operational until it has been inspected, validated,
materialized and bound to case-world residue.

## Pack Kinds

`native_yai`
: YAI-owned operational packs included with product tiers or local defaults.

`user`
: Personal methods, prompts, templates, procedures, checklists, writing style,
scripts, local folders and personal standards.

`organization`
: Company procedures, policies, templates, quality standards, escalation rules,
internal knowledge, approved providers and review rules.

`vertical`
: Domain-specific packs for legal, accounting, business administration,
software release, audit/evidence, research and coding.

`pilot`
: Pack created or refined from a real pilot/reference case.

`reference`
: Stable demonstration pack used for docs, tests, demos and conformance.

`experimental`
: Lab material, usually future ai-environment material, not stable core pack.

`compat`
: Historical or legacy pack-like material retained only for transition.

## Materialization Mapping

```text
pack.domain        -> case_domain
pack.material      -> case_attachment
pack.binding       -> case_binding
pack.policy        -> policy_material / policy_rule
pack.projection    -> projection_rule
pack.authority     -> authority_scope
pack.procedure     -> future case_procedure / procedure_record
pack.template      -> case_attachment or projection material
pack.output_schema -> projection/output contract
pack.memory_seed   -> memory seed / future memory candidate
pack.actor         -> future subject/actor posture, usually ai-environment if agentic
pack.tool          -> future tool/carrier/ai-environment material
pack.fixture       -> observability/evaluation fixture
```

## Lifecycle

```text
discover
inspect
validate
materialize
bind_to_case
project_to_consumer
observe_outputs
derive_memory
evaluate
version
retire
archive
```

`discover` means a pack exists and can be inspected. `inspect` reads manifest,
structure, provenance and limits. `validate` checks manifest, paths, schemas,
policy, projection and output shape.

`materialize` converts pack material into case-world material. `bind_to_case`
creates case domains, attachments, bindings, authority scopes, projection
rules and policy material. `project_to_consumer` exposes selected material to
model, operator, audit and debug views.

`observe_outputs`, `derive_memory` and `evaluate` create future evidence about
how the pack behaves. `version`, `retire` and `archive` preserve provenance and
replay without forcing new cases to keep using stale pack material.

## Data-Plane Impact

SPINE.21 does not implement these planes. It fixes the future input shape.

```text
hot plane:
  active pack bindings
  active pack authority scopes
  current pack projection posture

record plane:
  pack manifest record
  pack material record
  pack binding record
  pack validation record

graph plane:
  pack_materializes_domain
  pack_attaches_material
  pack_defines_policy
  pack_defines_projection_rule
  pack_seeds_memory
  pack_used_by_projection

fact plane:
  pack_usage_count
  pack_policy_trigger_count
  pack_output_acceptance
  pack_review_burden
  pack_violation_rate

memory plane:
  pack-derived memory
  user-method memory
  organization-method memory

projection plane:
  pack-derived model/operator/audit views
```

## Projection And Model Impact

Packs affect what the model sees, allowed and forbidden actions, expected
output shape, review obligations, memory seeds, authority wording and model
behavior experiments.

Projection remains a controlled view. Control still evaluates proposed
operations and receipts still prove effects.

## Observability Impact

Future pack facts include:

```text
pack_usage_count
pack_policy_trigger_count
pack_projection_used
pack_memory_reuse
pack_output_acceptance
pack_violation_rate
pack_review_burden
pack_effectiveness
pack_stale_material_count
pack_conflict_count
```

## Commercial Boundary

Native packs are included in plans and reference flows. User packs materialize
the user's own method. Organization packs materialize company method and
policy into shared cases. Vertical packs are pilot/reference/domain bundles.

Marketplace, billing, installer, cloud sync and pack registry backends are
explicitly deferred.

## Non-Goals

```text
no marketplace
no billing
no pack installer
no pack registry backend
no pack runtime
no pack execution engine
no agent pack runtime
no copying old yai packs wholesale
no pack directories under yai-core source in SPINE.21
```
