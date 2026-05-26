# Pack Format

Status: SPINE.21 doctrine. No pack runtime or installer is implemented here.

## Canonical Shape

```text
pack/
├── pack.yaml
├── README.md
├── materials/
├── policies/
├── procedures/
├── projections/
├── templates/
├── outputs/
├── memory/
├── actors/
├── tools/
├── fixtures/
└── tests/
```

This shape is a format contract, not a source tree to create in `yai`
during SPINE.21.

## Minimal Manifest Example

```yaml
pack_id: yai.native.audit-evidence.v1
name: Audit Evidence Pack
kind: native_yai
version: 0.1.0
owner: yai
status: draft

compatibility:
  yai_core_min: 0.0.0-newcore

domains:
  - audit
  - evidence

materializes:
  case_domains:
    - audit_evidence
  attachments:
    - document
    - receipt_bundle
  policies:
    - evidence_required
  projection_rules:
    - audit_summary
  authority_scopes:
    - model_interpretation_only
  output_schemas:
    - evidence_packet

limits:
  max_material_files: 64
  max_policy_rules: 128
  max_projection_templates: 16
  max_output_schemas: 16
  max_memory_seeds: 64

provenance:
  source: yai
  license: internal
  created_by: yai
```

## Manifest Sections

```text
identity
kind
owner
version
status
compatibility
domains
materials
policies
projection_rules
authority_scopes
procedures
templates
output_schemas
memory_seeds
actors
tools
fixtures
limits
provenance
validation
retirement
```

`identity` includes stable id and display name. `kind`, `owner`, `version` and
`status` define classification and lifecycle posture. `compatibility` declares
the minimum YAI version and any later schema compatibility.

`domains`, `materials`, `policies`, `projection_rules`, `authority_scopes`,
`procedures`, `templates`, `output_schemas`, `memory_seeds`, `actors`, `tools`
and `fixtures` list pack material that can be validated and materialized.

`limits` records budgets. `provenance` records origin and licensing posture.
`validation` records expected checks. `retirement` records replacement or
archive posture.

## Budgets

Budgets are required metadata, not final hard limits.

```text
small user pack:
  10-50 materials
  5-20 policies
  1-5 projection templates
  1-3 output schemas

professional pack:
  50-500 materials
  20-200 policies
  5-30 projection templates
  5-50 tests/fixtures

organization pack:
  larger, but indexed, versioned and governed by YAI_HOME/data-plane limits
```

Pack manifests must support material, policy, projection, memory, tool/actor
and fixture/test budgets.

## Validation Posture

SPINE.21 defines validation categories only:

```text
manifest schema
folder shape
path containment
provenance
license posture
kind/status compatibility
material budget
policy budget
projection budget
output schema shape
memory seed shape
actor/tool externalization posture
fixture/test shape
retirement metadata
```

Validation does not execute a pack. It only decides whether material is shaped
well enough to be materialized into a case by future implementation waves.
