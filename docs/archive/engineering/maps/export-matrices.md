Historical/superseded engineering record. Not an active source of truth.

# Export Matrices

Exports are controlled projections over core truth. They are not new owners of
truth, and they must not turn `interfaces`, `console` or a future cloud layer
into semantic authorities.

## Rule

Every export must name:

```text
source primitive
export artifact
consumer
freshness/provenance rule
non-exported material
failure signal
```

If an export cannot name its source primitive, it is not ready. If an export
cannot name material that must stay out, it is not safe.

## Core Value To Export

| Value | Core primitive | Export artifact | Primary consumer | Must not export |
|---|---|---|---|---|
| What was attempted | `op` | attempt record | audit, debug, integration kit | hidden prompt state |
| Why it was allowed or denied | `control` | decision packet | operator, audit, policy review | unenforced policy text as proof |
| What really happened | `effect` + `store` | receipt packet | audit, graph, projection | unverifiable external claim as receipt |
| Which thing was affected | `subject` + `case` | subject binding manifest | console, integration kit | credentials or private material |
| How events relate | `graph` | edge list with record refs | audit, memory, debug | duplicated record payloads without refs |
| What can be remembered | `memory` | scoped memory record set | memory pack, projection | cross-case recall without scope grant |
| What can be shown | `projection` | redacted read model | console, API, model context | authority decisions not backed by records |
| What diverged | `reconcile` | divergence report | operator, recovery, audit | silent repair without record trail |

## Product Export Matrix

| Product step | Export required | Purpose | Guardrail |
|---|---|---|---|
| YAI Core Developer / Local | local case packet | prove the NEW.1 loop on one machine | no team/cloud assumption |
| YAI Integration Kit | schemas, fixtures, conformance packet | let adapters speak core primitives | adapters do not redefine semantics |
| Operational Memory / Audit Pack | audit-ready case export | reconstruct operations without enforcing first | no certified-compliance promise |
| Professional / Team | multi-case export and policy packet | coordinate review, policy and memory across a team | no hidden organization control plane |
| Enterprise / Embedded | evidence package and carrier report | embed control into regulated or industrial stacks | custom integrations keep receipt rules |
| Platform / Cloud later | synced projection bundles | collaborate over core truth | cloud is consumer, not source of truth |

## Wedge Export Matrix

| Wedge | Minimal export | Test case | Success shape |
|---|---|---|---|
| Agentic coding | repo case packet | `TC-LOCAL-001` | file attempts, decisions, receipts and patch-linked memory |
| Dangerous shell | denied attempt packet | `TC-LOCAL-002` | blocked command with decision basis and no fake receipt |
| MCP/tool server | tool drift divergence | `TC-MCP-001` | descriptor mismatch becomes reviewable divergence |
| AI workflow | missing-receipt report | `TC-WF-001` | observed effect without receipt is visible |
| Model invocation | model op packet | `TC-MODEL-001` | model call links to case, patch and receipt |
| Database mutation | mutation receipt packet | `TC-DB-001` | write has subject, policy decision and observed state delta |
| Kubernetes rollout | rollout case packet | `TC-K8S-001` | admission/observation trail reconstructs rollout outcome |
| Memory scope | scoped memory packet | `TC-MEM-001` | recall stays inside granted case scope |
| Audit | case audit export | `TC-AUDIT-001` | attempts, decisions, receipts and graph refs export together |

## Extraction Export Matrix

| Current material | Future destination | Export type | First action |
|---|---|---|---|
| `src/case` | `case`, `subject`, `projection` | case packet, subject binding manifest | mine identity, boundary, evidence and subject concepts |
| `src/decision` | `control` | decision packet, policy binding packet | absorb basis/outcome/conflict into control docs first |
| `src/runtime/operator` | `ctl` and later `console` | command result, operator projection | split technical commands from human UX |
| `src/runtime/execution` | `effect` | receipt packet | extract attempt-to-carrier-to-receipt path |
| `src/substrate/store` | `store` | record stream | keep C store first, prepare Rust dual-write |
| `src/substrate/graph` | `graph` | edge list | define edge records before graph engine migration |
| `src/substrate/memory` | `memory` | scoped memory records | migrate after receipts and graph refs stabilize |
| `src/substrate/views` | `projection` | redacted read model | rename view ownership to projection |
| `src/models` | `subject/model`, `effect/model carrier`, `ai-environment` | model invocation packet | keep model provider behavior outside core semantics |
| `src/agents` | `subject/agent`, `projection/agent`, `ai-environment` | actor/agent projection | do not recreate an agent root |

## Export Failure Rules

An export should fail closed or mark itself incomplete when:

```text
decision_ref is missing for an enforced effect
receipt_ref is missing for a claimed effect
subject_ref is missing for a targeted operation
policy provenance is missing for a decision basis
freshness cannot be computed
redaction cannot be proven
memory scope cannot be verified
```

These are not cosmetic errors. They are divergence signals that must be visible
to `reconcile` and to audit/debug projections.
