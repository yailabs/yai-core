# ADR 0017: Packs As Case Materialization Units

## Status

Accepted for SPINE.21.

## Context

Pack-like material exists in `yai-dev` as case packs, case materialization,
case policy, domain scope, model frame packs, source material, projections,
agent roles and capability bundles. Without a canonical boundary, this material
could collapse into marketplace product language, prompt bundles, templates,
agent execution or loose folders.

YAI needs the concept before hot state, record, graph, fact, projection
and memory planes mature, because those planes must know that pack material is
case-world material.

## Decision

Packs are case materialization units.

They may materialize domains, attachments, bindings, policy material,
projection rules, authority scopes, procedures, templates, output schemas,
memory seeds, actors/tools and validation fixtures into a case.

Native packs are YAI-owned defaults. User packs materialize personal methods.
Organization packs materialize company methods, standards and policies.
Vertical packs are pilot/reference/domain bundles, not an immediate
marketplace.

## Consequences

Pack material must be inspected, validated and materialized before it becomes
operational case residue.

Packs enter the case through case-world primitives first:

```text
case_domain
case_attachment
case_binding
policy_material / policy_rule
projection_rule
authority_scope
```

Future data planes must treat pack material as typed input, not external glue.
Projection and model-facing context may include selected pack material, but
projection is not enforcement. Control still owns gates and decisions.

## Non-goals

```text
no marketplace
no billing
no pack installer
no pack runtime
no pack registry backend
no cloud sync
no agent pack execution
no yai-dev source copy
```

## Related Docs

```text
../architecture/15-pack-materialization.md
../engineering/pack-format.md
../engineering/pack-roadmap.md
../engineering/four-repo-roadmap.md
../engineering/extraction-plan.md
```
