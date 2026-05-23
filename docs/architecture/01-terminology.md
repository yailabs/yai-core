# Terminology

This file freezes core vocabulary for DOC.CORE.0 and NEW.1. Terms here are
machine terms, not product labels.

| Term | Meaning |
|---|---|
| `case` | Operational boundary that binds subjects, policy, evidence, attempts, decisions, receipts and projections. |
| `subject` | Observable, controllable, invokable, queryable or targetable entity bound to a case. |
| `source` | Origin of material, claim, observation, policy or receipt. A source can also be a subject. |
| `actor` | Subject attempting or requesting an operation. |
| `target` | Subject affected by an operation attempt or carrier effect. |
| `locator` | Address or access hint for a subject. Locator is not identity. |
| `binding` | Case-scoped record that connects subject identity, locator, mode, policy and receipt obligations. |
| `op` | Operation domain: normalized attempts and effect classification. |
| `attempt` | Proposed operation over subjects before execution or observation is claimed. |
| `effect_class` | Machine class of intended effect, replacing old capability taxonomy as a root concept. |
| `control` | Machine plane for policy materialization, gates, decisions, obligations and receipt requirements. |
| `policy source` | Origin of policy material, such as file, operator, package, integration or imported rule set. |
| `policy material` | Raw policy content with provenance. Not enforcement by itself. |
| `policy claim` | Extracted statement from policy material that may become a rule. |
| `policy rule` | Normalized, scoped control object that can participate in gates. |
| `gate` | Evaluation point that applies policy to case, subject, actor, op, carrier, projection or retention posture. |
| `decision` | Structured outcome emitted by control for an attempt. Decision is not a root. |
| `obligation` | Required follow-up created by a decision or policy rule. |
| `effect` | Executed, observed or imported operational consequence. |
| `carrier` | Mechanism that executes, observes or imports an effect over subjects. |
| `receipt` | Durable evidence connecting attempt, decision posture, carrier or observation path and affected subjects. |
| `store` | Durable record, journal, blob and integrity owner. |
| `index` | Technical retrieval structures over stored residue. |
| `graph` | Relationship and causality reconstruction over subjects, ops, policy, decisions and receipts. |
| `memory` | Scoped operational experience derived from records, receipts and graph paths. |
| `projection` | Controlled read model served to models, agents, operators, APIs, audit and debug surfaces. |
| `interaction_thread` | Case-bound conversational lane selected for a participant. |
| `interaction_turn` | One user/model/system exchange or event inside an interaction thread. |
| `participant_view_frame` | Model/operator-visible context assembled for a specific turn from projection, thread, memory, delta and authority posture. |
| `transcript` | Audit persistence of prompt/output material. It is not automatically model context. |
| `reconcile` | Divergence detection and recovery/compensation posture. |
| `divergence` | Mismatch between claim, decision, expected effect, receipt, observation, policy or memory. |
| `daemon` | Local resident process, `yaid`, for boot, loop, IPC, status and supervision. |
| `ctl` | Technical control CLI, `yai`, over core primitives. It is not Console UX. |

## Old-Term Mapping

| Old term | Frozen mapping |
|---|---|
| capability | `effect_class` / op shape |
| agent | subject, source or actor external to core |
| runtime | `daemon` + `effect` |
| substrate | `store` + `index` + `graph` + `memory` + `projection` |
| lineage | graph reconstruction |
| view | projection |
| chat | interaction thread at the core boundary; console may use chat UX language |

Old terms may appear in extraction and ADR history. They must not become future
core roots.
