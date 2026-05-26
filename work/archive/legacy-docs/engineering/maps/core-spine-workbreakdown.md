Historical/superseded engineering record. Not an active source of truth.

# Core Spine Work Breakdown

Status: SPINE.5 engineering map.

Each macro-spine owns a slice of the system and a required set of
subdeliveries. The flat NEW roadmap remains the milestone index; this document
is the engineering breakdown.

## WORLD

Purpose: The WORLD spine defines what exists inside a case, how it entered and
what authority it has.

Owns: `case_world`, `case_domain`, `case_attachment`, `case_binding`,
`authority_scope`, `projection_rule`, `case_session`, `case_context`,
`interaction_thread`, `participant_view_frame`.

Does not own: carrier execution, model inference, policy reasoning, database
backend or agent planning.

Old-yai audit sources: `src/case/identity`, `src/case/materialization`,
`src/case/attachments`, `src/case/subjects`, `src/case/continuity`,
`src/runtime/sessions`.

Future roots/files: `include/yai/case/*`, `system/case/*`, `engine/record`,
`engine/hot`.

Subdeliveries:

```text
WORLD.0  case_world doctrine
WORLD.1  case_domain records
WORLD.2  case_attachment records
WORLD.3  case_binding records
WORLD.4  authority_scope records
WORLD.5  projection_rule binding
WORLD.6  case_session
WORLD.7  case_context
WORLD.8  interaction_thread
WORLD.9  participant_view_frame
WORLD.10 case_world replay/rebuild
WORLD.11 case_world hot-state integration
WORLD.12 WORLD freeze
```

Quality gates: no participant without case-world material; no live operation
without case context; no model-visible prompt without thread and frame.

Related waves: NEW.18A, NEW.18B, NEW.18C, NEW.22.

## RESIDUE

Purpose: RESIDUE defines what remains after anything participates in the case.

Owns: record taxonomy, claims, attempts, decisions, receipts, observations,
transcript/audit records, evidence refs and schema posture.

Does not own: storage backend, memory consolidation, UI transcript UX or model
truth.

Old-yai audit sources: `src/substrate/records`, `src/case/evidence`,
`src/lineage`, `src/runtime/execution`.

Future roots/files: `include/yai/store/record.h`, `system/store/record.c`,
`engine/record`, `proto/schemas`.

Subdeliveries:

```text
RESIDUE.0  residue doctrine
RESIDUE.1  record taxonomy
RESIDUE.2  claim records
RESIDUE.3  model_interpretation records
RESIDUE.4  attempt records
RESIDUE.5  decision records
RESIDUE.6  receipt records
RESIDUE.7  observation records
RESIDUE.8  transcript/audit records
RESIDUE.9  evidence refs
RESIDUE.10 schema/version posture
RESIDUE.11 residue replay test
RESIDUE.12 RESIDUE freeze
```

Quality gate: no operational claim becomes truth without residue, provenance
and where needed receipt.

Related waves: NEW.0-NEW.9, NEW.18C, NEW.31.

## DATA

Purpose: DATA turns residue into live/durable operational substrate.

Owns: hot state, journal replay, LMDB record plane, Ladybug graph plane,
DuckDB fact plane, projection delta engine and rebuild paths.

Does not own: console views, external API semantics, policy decisions or model
inference.

Old-yai audit sources: `src/substrate/store`, `src/substrate/records`,
`src/substrate/graph`, `src/substrate/indexes`, `src/substrate/query`,
`src/substrate/views`, `src/analytics`.

Future roots/files: `engine/yai-engine/src/{hot,store,journal,record,graph,index,query,facts,projection,reconcile}`.

Subdeliveries:

```text
DATA.0   operational data plane doctrine
DATA.1   hot state/shared memory contracts
DATA.2   hot state implementation
DATA.3   journal replay contract
DATA.4   LMDB record schema
DATA.5   LMDB write/read path
DATA.6   journal -> LMDB rebuild
DATA.7   Ladybug graph schema
DATA.8   graph write/read path
DATA.9   graph rebuild from records
DATA.10  DuckDB fact schema
DATA.11  fact extraction from residue
DATA.12  projection delta engine
DATA.13  memory consolidation over data planes
DATA.14  reconcile consistency across hot/store/graph
DATA.15  data-plane rebuild test
DATA.16  DATA freeze
```

Quality gates: journal remains replay/audit; hot state is not truth; rebuild
from residue must be testable.

Related waves: SPINE.3R, NEW.22-NEW.30.

## VIEW

Purpose: VIEW controls what each consumer sees and how fresh, complete and
authoritative that view is.

Owns: projection frames, deltas, freshness, redaction, authority alignment,
consumer templates and Case View Quality.

Does not own: source truth, UI state, model inference or carrier authority.

Old-yai audit sources: `src/substrate/views`, `src/case/surface`,
`src/decision/projection`, `src/models/frame`.

Future roots/files: `include/yai/projection/*`, `system/projection/*`,
`engine/projection`, `engine/hot`.

Subdeliveries:

```text
VIEW.0   projection as live cognitive view doctrine
VIEW.1   projection frame schema
VIEW.2   projection delta schema
VIEW.3   freshness/staleness rules
VIEW.4   redaction posture
VIEW.5   authority alignment
VIEW.6   memory patch integration
VIEW.7   divergence exposure rules
VIEW.8   model projection template
VIEW.9   operator projection template
VIEW.10  audit projection template
VIEW.11  debug projection template
VIEW.12  projection invalidation tests
VIEW.13  Case View Quality metrics
VIEW.14  VIEW freeze
```

Quality gate: models do not access the case directly; models receive
controlled live projections.

Related waves: NEW.8, SPINE.3R, SPINE.4, NEW.18C, NEW.27.

## CONTROL

Purpose: CONTROL decides what can become effect and why.

Owns: policy source/material, policy packs, claims/rules, gates, obligations,
receipt requirements, decision basis and review posture.

Does not own: carrier execution, provider internal policy, console UX or raw
documents as enforcement.

Old-yai audit sources: `src/case/policy`, `src/runtime/decision`,
`src/runtime/execution`, `src/governance`.

Future roots/files: `include/yai/control/*`, `system/control/*`,
`engine/facts`, `engine/memory`.

Subdeliveries:

```text
CONTROL.0  control doctrine
CONTROL.1  policy source/material model
CONTROL.2  policy pack schema
CONTROL.3  policy claim extraction
CONTROL.4  policy rule normalization
CONTROL.5  gate composition
CONTROL.6  obligation model
CONTROL.7  receipt requirement model
CONTROL.8  case/subject/op/carrier/projection binding
CONTROL.9  policy conflict detection
CONTROL.10 authority review model
CONTROL.11 decision basis hardening
CONTROL.12 policy projection to model/operator
CONTROL.13 policy memory
CONTROL.14 policy-control freeze
```

Quality gate: policy is materialized cognitively through projection and
operationally through gate/decision/carrier.

Related waves: NEW.3, NEW.34, NEW.38-NEW.46.

## MEMORY

Purpose: MEMORY turns operational residue into scoped reusable experience.

Owns: memory candidates, basis, freshness/confidence, receipt-backed memory,
graph-derived memory, policy-aware memory and memory quality facts.

Does not own: chat history, hidden model state, generic knowledge or RAG.

Old-yai audit sources: `src/substrate/memory`, `src/lineage`,
`src/analytics/signals`, `src/agents/grounding`.

Future roots/files: `include/yai/memory/*`, `system/memory/*`,
`engine/memory`, `engine/facts`.

Subdeliveries:

```text
MEMORY.0   operational memory doctrine
MEMORY.1   candidate basis model
MEMORY.2   freshness/confidence
MEMORY.3   receipt-backed memory
MEMORY.4   graph-derived memory
MEMORY.5   policy-aware memory
MEMORY.6   subject-scoped memory
MEMORY.7   error/recovery memory
MEMORY.8   memory consolidation
MEMORY.9   memory contradiction check
MEMORY.10  memory projection rules
MEMORY.11  memory quality facts
MEMORY.12  MEMORY freeze
```

Quality gate: memory is receipt-backed, graph-derived, policy-aware
operational experience, not chat history.

Related waves: NEW.6, NEW.28, NEW.41.

## RECONCILE

Purpose: RECONCILE detects where observed residue contradicts expected case
trajectory.

Owns: divergence taxonomy, mismatch detection, stale projection detection,
authority overclaim detection and recovery/compensation posture.

Does not own: silent repair, carrier execution or truth rewriting.

Old-yai audit sources: `src/substrate/signals`, `src/analytics/signals`,
`src/runtime/observation`, `src/lineage`.

Future roots/files: `include/yai/reconcile/*`, `system/reconcile/*`,
`engine/reconcile`.

Subdeliveries:

```text
RECON.0  reconcile doctrine
RECON.1  divergence taxonomy
RECON.2  denied_but_executed detection
RECON.3  receipt_without_decision detection
RECON.4  claim_without_receipt detection
RECON.5  memory_without_basis detection
RECON.6  projection_stale detection
RECON.7  authority_overclaim detection
RECON.8  hot/store/graph consistency
RECON.9  recovery posture
RECON.10 compensation posture
RECON.11 RECON freeze
```

Quality gate: mismatch is explicit residue, never hidden by projection.

Related waves: NEW.7, NEW.29, NEW.54.

## HOST

Purpose: HOST is where control becomes effect, but only through sanctioned
carriers.

Owns: filesystem/process/network/database/repository/model carriers, host
observation probes, carrier receipts and recovery/compensation execution
boundary.

Does not own: policy truth, model inference, UI commands or unconstrained host
access.

Old-yai audit sources: `src/runtime/carriers`, `src/runtime/execution`,
`src/runtime/observation`.

Future roots/files: `include/yai/effect/*`, `system/effect/*`,
`system/daemon`, `cmd/yaid`.

Subdeliveries:

```text
HOST.0   host/control doctrine
HOST.1   filesystem carrier hardening
HOST.2   path canonicalization
HOST.3   sandbox proof
HOST.4   permission posture
HOST.5   process carrier doctrine
HOST.6   process carrier v0
HOST.7   network/http carrier
HOST.8   database carrier
HOST.9   repository/git carrier
HOST.10  model carrier
HOST.11  carrier receipt hardening
HOST.12  host observation probes
HOST.13  recovery/compensation execution boundary
HOST.14  HOST freeze
```

Quality gate: no effect claim without carrier/observation receipt.

Related waves: NEW.4, NEW.47-NEW.56.

## MODEL

Purpose: MODEL studies and governs models as subjects, not owners of the
system.

Owns: provider/model subject posture, provider attachment, model output claim
import, interpretation hardening, behavior facts and model carrier evidence.

Does not own: core truth, policy truth, receipt authority or agent framework
planning.

Old-yai audit sources: `src/models`, `src/runtime/provider`,
`src/agents/grounding`, `src/case/policy`, `src/substrate/views`.

Future roots/files: `system/case`, `system/subject`, `engine/facts`,
`engine/projection`, `system/effect`.

Subdeliveries:

```text
MODEL.0   naked model doctrine
MODEL.1   provider subject posture
MODEL.2   model subject posture
MODEL.3   local provider attachment
MODEL.4   remote provider attachment
MODEL.5   model output claim import
MODEL.6   model_interpretation hardening
MODEL.7   model behavior facts
MODEL.8   refusal/overclaim/unsupported-claim classifier
MODEL.9   model carrier v0
MODEL.10  naked local model experiment
MODEL.11  multi-model comparison
MODEL.12  unknown provider policy handling
MODEL.13  MODEL freeze
```

Quality gate: first AI behavior experiment is naked model inside a case, not
agent framework integration.

Related waves: NEW.32-NEW.37, NEW.42-NEW.43.

## OBSERVABILITY

Purpose: OBS measures whether the live case remains knowable enough for
controlled action.

Owns: trace/span records, timing samples, freshness state, projection freshness
facts, rebuild diagnostics, model behavior facts, memory quality facts,
analytics facts and CVQ reports.

Does not own: case truth, model inference, repair execution or UI dashboards.

Old-yai audit sources: `src/analytics`, `src/substrate/signals`,
`src/runtime/observation`, `src/runtime/execution`.

Future roots/files: `engine/observability`, `engine/facts`,
`work/archive/engineering-snapshots/case-view-quality.md`.

Subdeliveries:

```text
OBS.0   operational observability doctrine
OBS.1   trace/span records
OBS.2   timing samples
OBS.3   freshness state
OBS.4   projection freshness facts
OBS.5   rebuild diagnostics
OBS.6   provenance completeness checks
OBS.7   model behavior facts
OBS.8   memory quality facts
OBS.9   DuckDB analytics facts
OBS.10  yai debug trace/freshness/rebuild commands
OBS.11  Case View Quality reports
OBS.12  OBS freeze
```

Quality gate: every implementation wave declares observability/freshness
impact.

Related waves: SPINE.4, NEW.26, NEW.30, NEW.36.

## EXTERNAL

Purpose: EXTERNAL keeps console, interfaces and ai-environment downstream of
yai-core truth.

Owns: interface alignment, SDK projection boundary, console projection-first UX,
ai-environment harness, protocol fixtures and conformance handoff.

Does not own: core semantics, daemon truth, store truth or carrier authority.

Old-yai audit sources: old `yai` concept mine, `interfaces`, `console`.

Future roots/files: sibling repos plus `proto/fixtures` and conformance docs in
`yai-core`.

Subdeliveries:

```text
EXT.0   four-repo doctrine
EXT.1   interfaces primitive alignment
EXT.2   interfaces transport alignment
EXT.3   SDK projection boundary
EXT.4   console projection-first UX
EXT.5   console review/inspection UX
EXT.6   ai-environment harness bootstrap
EXT.7   agentic QA/release rehearsal lab
EXT.8   protocol fixtures from core journals
EXT.9   core conformance harness
EXT.10  local core milestone freeze
```

Quality gate: interfaces, console and ai-environment consume yai-core truth;
they do not define it.

Related waves: SPINE.0, NEW.57-NEW.62, ENV.CANON.*, INTF.CANON.*,
CONSOLE.CANON.*.
