# Filesystem Loop Policy Fixture / Pack Doctrine Mapping

Status: doctrine mapping.

## Purpose

This document maps the current filesystem-loop `pack-fixture/` material to the
existing pack doctrine.

It does not define a pack runtime, migration target, source root, installer,
registry backend or generic pack template.

The current files in this directory are filesystem-loop lab input fixtures.
They are shaped toward pack materialization doctrine because they represent
policy, projection and authority posture that the filesystem-loop case later
materializes as case residue.

## Current Fixture Inventory

| Fixture | Current lab role | Observed posture | Pack doctrine material class |
|---|---|---|---|
| `policies/filesystem-sandbox-policy.json` | Describes filesystem effect constraints for `case:new12-filesystem`. | Policy posture for sandboxed filesystem effects, decision consequence and receipt evidence. | `pack.policy` partial; validation/evaluation fixture partial. |
| `policies/linenoise-terminal-interface-policy.json` | Describes terminal interface authority boundaries. | Interface posture and authority scope for operator prompt submission. | `pack.authority` partial; `pack.material` partial for interface posture. |
| `policies/model-case-projection-policy.json` | Describes model-provider projection and output boundaries. | Projection rule and authority scope for model context consumption and model output import. | `pack.projection` partial; `pack.authority` partial; `pack.policy` partial. |

The current implementation posture is documented in `README.md`: these JSON
files are lab fixtures, not an installed policy registry and not a general
policy engine.

## Pack Doctrine Mapping

Existing repo doctrine defines packs as case materialization units:

- `docs/architecture/15-pack-materialization.md`
- `docs/adr/0017-packs-as-case-materialization-units.md`
- `docs/engineering/pack-format.md`
- `docs/architecture/03A-case-world-model.md`

The table below compares the current filesystem-loop fixtures against the
material areas listed by `docs/engineering/pack-format.md`.

| Pack doctrine area | Represented by current fixture? | Evidence path | Notes |
|---|---|---|---|
| `identity` | Partial | `pack.yaml`; `policy_ref`, `case_ref`, `subject_ref` in each JSON fixture | Fixture identity exists, but there is no production pack identity. |
| `kind` | No | `docs/engineering/pack-format.md` | No production pack kind is declared; `pack.yaml` declares `role: lab_fixture`. |
| `owner` | No | `docs/engineering/pack-format.md` | No production pack owner field is declared. |
| `version` | Partial | `schema: yai.lab.policy_pack.v0` and `policy_ref` values | Fixture schema/policy version exists, but no pack version exists. |
| `status` | No | `docs/engineering/pack-format.md` | No pack lifecycle status is declared. |
| `compatibility` | No | `docs/engineering/pack-format.md` | No YAI version compatibility is declared. |
| `domains` | Partial | `case_ref: case:new12-filesystem` | The fixture is tied to one case, but no formal pack domain section exists. |
| `materials` | Partial | Fixture summaries and rule bodies | The JSON files are material, but they are not listed under a pack manifest `materials` section. |
| `policies` | Yes, fixture-local | `policies/filesystem-sandbox-policy.json`; `rule_class: policy_rule` | Policy posture is represented for this lab fixture. This does not prove that packs are policy-only. |
| `projection_rules` | Yes, fixture-local | `policies/model-case-projection-policy.json`; `rule_class: projection_rule` | Model projection posture is represented for this lab fixture. |
| `authority_scopes` | Yes, fixture-local | `policies/linenoise-terminal-interface-policy.json`; `policies/model-case-projection-policy.json`; `rule_class: authority_scope` | Authority posture is represented for terminal and model-provider boundaries. |
| `procedures` | No | `docs/engineering/pack-format.md` | No procedure material is represented by these fixtures. |
| `templates` | No | `docs/engineering/pack-format.md` | No template material is represented. |
| `output_schemas` | No | `docs/engineering/pack-format.md` | No output schema material is represented. |
| `memory_seeds` | No | `docs/engineering/pack-format.md` | No memory seed material is represented. |
| `actors` | No | `docs/engineering/pack-format.md` | The fixtures mention subjects, but do not define pack actor material. |
| `tools` | No | `docs/engineering/pack-format.md` | No tool material is represented. |
| `fixtures` | Yes | This directory and `README.md` | The files are lab fixtures for filesystem-loop validation. |
| `limits` | No | `docs/engineering/pack-format.md` | No material, policy, projection or fixture budgets are declared. |
| `provenance` | Partial | `source_ref: filesystem-loop-lab` | Fixture source is declared; full pack provenance is not. |
| `validation` | Partial | `evidence_required` fields | Evidence requirements exist, but no pack validation section exists. |
| `retirement` | No | `docs/engineering/pack-format.md` | No retirement or replacement posture is declared. |

## What Current Fixtures Are

- Filesystem-loop lab fixtures.
- Policy, projection and authority posture material for one bounded case.
- Case-materialization-adjacent evidence.
- Concrete evidence that the filesystem-loop case can represent
  `subject:policy-pack`, `policy_rule`, `projection_rule`, `authority_scope`
  and graph evidence.
- Fixture material that can be compared to pack doctrine without treating it as
  a complete pack.

## What Current Fixtures Are Not

- These fixtures are not a generic pack.
- Not a pack runtime.
- Not a pack installer.
- Not a pack registry backend.
- Not a complete pack manifest.
- Not evidence that every pack must include policy material.
- Not proof that packs are policy-only.
- Not a production pack source root.
- Not a validation CLI or materialization API.

## Undefined / Open Questions

- Whether these fixtures should later be wrapped by a formal pack manifest is
  unknown.
- Where future pack source roots live is not defined by current implementation
  evidence.
- How pack validation will work is not implemented here.
- How a pack materialization CLI or API will work is not implemented here.
- Whether lab fixture names should later change is not established.
- Whether this fixture schema should remain `yai.lab.policy_pack.v0` or become
  subordinate to a broader pack manifest is unknown.
- How future pack provenance, limits, retirement and compatibility fields map
  onto this lab fixture material is unknown.

## Safe Next Steps

- Keep this directory as lab fixture material until a separate wave explicitly
  defines runtime pack behavior.
- Use this mapping when comparing fixture material to pack doctrine.
- If pack templates are needed later, create them in a separate
  evidence-grounded design wave.
- If runtime materialization is needed later, define it in a separate
  implementation wave after doctrine and format boundaries are settled.
