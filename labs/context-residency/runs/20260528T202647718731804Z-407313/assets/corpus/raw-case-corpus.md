# Raw Case Corpus


## Source: case-attach-provider.txt

provider_attachment: accepted
provider_attachment_status: attached
case_ref: case:new12-filesystem
subject_ref: subject:llm-provider
case_session: active
case_context: active
authority_scope: model interpretation_only
provider_base_url: http://127.0.0.1:43117/v1/chat/completions
provider_model: Qwen_Qwen3-8B-Q4_K_M.gguf
api_key_env: OPENCODE_LLM_API_KEY


## Source: case-enter.txt

case_entry: accepted
case_entry_status: admitted
case_session: active
case_world: materialized
case_context: active
case_ref: case:new12-filesystem
subject_ref: subject:llm-provider
participant_view: model_context
consumer: model
redaction: summary_only
raw_journal_access: not_provided
filesystem_access: not_provided
authority_scope: model interpretation_only

case_ref: case:new12-filesystem
case_world: materialized
case_context: active
consumer: model
projection_kind: model_context
redaction: summary_only
source: case_projection_graph_memory
raw_journal_access: not_provided
filesystem_access: not_provided
decision_authority: not_provided
receipt_authority: not_provided
terminal_authority: prompt_surface_only_no_decision_authority
records: 29
case_domains: 1
case_attachments: 1
case_bindings: 1
interaction_threads: 0
interaction_turns: 0
participant_view_frames: 0
model_projection_records: 1
operator_projection_records: 1
filesystem_receipts: 3
memory_candidates: 1
projection_rules: 1
authority_scopes: 3
model_interpretations: 0
graph_edges: 3

## Case World
- rec:new12-fs-case-domain kind:case_domain subject_ref:subject:none attempt_id:none decision_id:none receipt_id:none summary:case_domain domain:operational_case scope:manual-filesystem-lo family:manual-validation active:true
- rec:new12-fs-case-attachment-sandbox kind:case_attachment subject_ref:subject:filesystem-sandbox attempt_id:none decision_id:none receipt_id:none summary:case_attachment kind:runtime_subject posture:bound subject:subject:filesystem-sandbox
- rec:new12-fs-case-binding-model kind:case_binding subject_ref:subject:none attempt_id:none decision_id:none receipt_id:none summary:case_binding kind:model posture:bound target:subject:llm-provider

## Subjects
- rec:new12-fs-subject kind:subject_binding subject_ref:subject:filesystem-sandbox attempt_id:none decision_id:none receipt_id:none summary:subject:filesystem sandbox bound
- rec:new12-fs-model-subject kind:subject_binding subject_ref:subject:llm-provider attempt_id:none decision_id:none receipt_id:none summary:subject:llm-provider bound role:model_provider projection:model_context
- rec:new12-fs-terminal-subject kind:subject_binding subject_ref:subject:linenoise-terminal attempt_id:none decision_id:none receipt_id:none summary:subject:linenoise-terminal bound role:operator_interface prompt_surface:vendored_linenoise
- rec:new12-fs-policy-subject kind:subject_binding subject_ref:subject:policy-pack attempt_id:none decision_id:none receipt_id:none summary:subject:policy-pack bound source:manual-filesystem-loop-validation

## Policy
- rec:new12-fs-policy-rule-fs kind:policy_rule subject_ref:subject:policy-pack attempt_id:none decision_id:none receipt_id:none summary:policy:manual-filesystem-sandbox-v0 rule:filesystem-sandbox-boundary condition:fs.effect constraint:sandbox_only

## Projection Rules
- rec:new12-fs-projection-rule-model kind:projection_rule subject_ref:subject:policy-pack attempt_id:none decision_id:none receipt_id:none summary:projection_rule:model-context-only subject:llm-provider consume:model_context output:claim_or_proposal

## Authority Scopes
- rec:new12-fs-authority-scope-fs kind:authority_scope subject_ref:subject:filesystem-sandbox attempt_id:none decision_id:none receipt_id:none summary:authority_scope:filesystem subject:filesystem-sandbox may:fs_inside_sandbox requires:decision_receipt
- rec:new12-fs-authority-scope-model kind:authority_scope subject_ref:subject:llm-provider attempt_id:none decision_id:none receipt_id:none summary:authority_scope:model subject:llm-provider may:claim_proposal no:carrier_effects actions_become:op_attempt
- rec:new12-fs-authority-scope-terminal kind:authority_scope subject_ref:subject:linenoise-terminal attempt_id:none decision_id:none receipt_id:none summary:authority_scope:terminal subject:linenoise-terminal may:display_submit_input no:execution_authority

## Decisions
- rec:new12-fs-block-decision kind:decision subject_ref:subject:filesystem-sandbox attempt_id:attempt:new12-fs-block decision_id:decision:new12-fs-block receipt_id:none summary:decision:require_review rule:mutative_operation_requires_review
- rec:new12-fs-write-decision kind:decision subject_ref:subject:filesystem-sandbox attempt_id:attempt:new12-fs-write decision_id:decision:new12-fs-write receipt_id:none summary:decision:allow_with_constraints constraint:sandbox_only

## Filesystem Receipts
- rec:new12-fs-read-receipt kind:filesystem_receipt subject_ref:subject:filesystem-sandbox attempt_id:attempt:new12-fs-read decision_id:none receipt_id:receipt:new12-fs-read summary:fs:read status:observed sandbox:inside
- rec:new12-fs-block-receipt kind:filesystem_receipt subject_ref:subject:filesystem-sandbox attempt_id:attempt:new12-fs-block decision_id:decision:new12-fs-block receipt_id:receipt:new12-fs-block summary:fs:write status:blocked sandbox:inside
- rec:new12-fs-write-receipt kind:filesystem_receipt subject_ref:subject:filesystem-sandbox attempt_id:attempt:new12-fs-write decision_id:decision:new12-fs-write receipt_id:receipt:new12-fs-write summary:fs:write status:executed sandbox:inside

## Memory
- rec:new12-fs-memory kind:memory_candidate subject_ref:subject:filesystem-sandbox attempt_id:attempt:new12-fs-block decision_id:decision:new12-fs-block receipt_id:receipt:new12-fs-block summary:memory:operational scope:case basis_records:22 basis_receipts:3 basis_edges:1 summary:filesystem write required review before s

## Graph
- rec:new12-fs-policy-edge kind:graph_edge subject_ref:subject:policy-pack attempt_id:none decision_id:none receipt_id:none summary:edge:policy_applies_to_subject subjects:filesystem,model,terminal
- rec:new12-fs-edge kind:graph_edge subject_ref:subject:filesystem-sandbox attempt_id:attempt:new12-fs-write decision_id:decision:new12-fs-write receipt_id:receipt:new12-fs-write summary:edge:decision_controls_op edge:receipt_records_effect
- rec:new12-fs-model-edge kind:graph_edge subject_ref:subject:llm-provider attempt_id:none decision_id:none receipt_id:none summary:edge:model_context_projects_case_graph subject:llm-provider case:new12-filesystem

## Projection Records
- rec:new12-fs-model-projection-request kind:projection_request subject_ref:subject:llm-provider attempt_id:none decision_id:none receipt_id:none summary:request:model_context redaction:summary_only subject:llm-provider
- rec:new12-fs-projection kind:projection_result subject_ref:subject:filesystem-sandbox attempt_id:none decision_id:none receipt_id:none summary:consumer:operator kind:operator_summary redaction:none freshness:fresh source_records:25 source_receipts:3 source_memory:1
- rec:new12-fs-model-projection kind:projection_result subject_ref:subject:llm-provider attempt_id:none decision_id:none receipt_id:none summary:consumer:model kind:model_context redaction:summary_only freshness:fresh source_records:26 source_receipts:3 source_memory:1

## Model Interpretations
- none

## Authority Boundaries
- case_domain, case_attachment and case_binding records define the operational case world visible to this participant.
- subject:linenoise-terminal is a vendored prompt surface only; it does not generate decisions, authorize writes, mutate receipts or own provider semantics.
- subject:llm-provider may produce claims, proposals and model_interpretation records; those are not authoritative state until checked against decisions, receipts, graph and memory.
- filesystem decisions are represented by decision records; existing decisions/receipts are historical residue, not mutable by model output.
- When answering, state authority granted by the current projection, not physical capability of the host process.
- raw_journal_access, filesystem_access, decision_authority and receipt_authority are not provided to the model participant view.


## Source: daemon-status.txt

{"ok":true,"version":"0.0.0-newcore","status":"ok","message":"yaid local ipc ready"}


## Source: facts-extract-all.txt

facts_extract:
case_ref: case:new12-filesystem
kind: all
status: completed
fact_receipt_written: 3
fact_decision_written: 2
fact_projection_written: 3
fact_model_behavior_written: 0
fact_policy_outcome_written: 7
fact_carrier_outcome_written: 3
fact_divergence_written: 0
fact_memory_quality_written: 1
facts_duplicate: 0
facts_are_truth: false


## Source: facts-init.txt

facts_init:
backend: duckdb
status: ready
facts_path: /tmp/yai-cr-h-1780000007-407327/store/facts/yai-facts.duckdb
schema: yai.fact.v1
bitemporal: yes
tables_created: 12
facts_extracted: 0


## Source: facts-report.txt

facts_report:
case_ref: case:new12-filesystem
schema: yai.fact.v1
facts_are_truth: false

sections:
  receipts:
    total: 3
    blocked: 1
    executed: 1
    observed: 1
    filesystem: 3
    execution_performed: 0
    carrier_attempted: 0
  decisions:
    total: 2
    allow_with_constraints: 1
    require_review: 1
    requires_review: 1
    review_id: 0
  projections:
    total: 3
    model: 1
    operator: 1
    unknown: 1
    request: 1
    result: 2
    fresh: 2
    unknown: 1
    none: 1
    summary_only: 2
    model_visible: 1
    operator_visible: 1
  policy:
    total: 7
    authority_scope_defined: 3
    decision_allowed: 1
    policy_rule_defined: 1
    projection_rule_defined: 1
    review_required: 1
    review_required: 1
    approved: 0
    denied: 0
    deferred: 0
    quarantined: 0
  carriers:
    total: 3
    filesystem: 3
    blocked: 1
    executed: 1
    observed: 1
    carrier_attempted: 2
    execution_performed: 2
    receipt_required: 3
  divergence:
    total: 0
    status: none_observed
  memory:
    total: 1
    memory_is_truth: false
    candidates: 1
    operational: 1
    case: 1
    basis_record_count: 22
    basis_receipt_count: 3
    basis_edge_count: 1
    requires_review: 0
  model:
    total: 0
    status: no_model_records


## Source: facts-summary.txt

facts_summary:
case_ref: case:new12-filesystem
fact_receipt: 3
fact_decision: 2
fact_projection: 3
fact_carrier_outcome: 3
fact_divergence: 0
fact_model_behavior: 0
fact_policy_outcome: 7
fact_memory_quality: 1
facts_total: 19
facts_are_truth: false
memory_is_truth: false


## Source: filesystem-loop.txt

{"request_id":"yai-filesystem","ok":true,"version":"0.0.0-newcore","status":"completed","message":"daemon filesystem loop completed","journal_path":"build/tmp/new12/daemon-407535/filesystem/journal.jsonl","record_count":28,"receipt_count":3,"projection_count":2,"decision_outcome":"allow_with_constraints","receipt_status":"executed","fs_read":"observed","fs_write_blocked":"blocked","fs_write_allowed":"executed","case_session":"active","case_world":"loaded","case_context":"active","authority_scope":"model:interpretation_only terminal:display_only"}


## Source: graph-materialize.txt

graph_materialize:
case_ref: case:new12-filesystem
source: lmdb_records
relations_seen: 68
relations_written: 68
relations_duplicate: 0
relations_skipped: 0
schema: yai.graph_relation.v1
graph_store: lmdb_graph_relations_v0
runtime_graph_updated: false


## Source: graph-rebuild.txt

runtime_graph_rebuild:
case_ref: case:new12-filesystem
source_mode: journal
journal_path: build/tmp/new12/daemon-407535/filesystem/journal.jsonl
journal_identity: journal:0e453c4c29675ddf
lmdb_path: /tmp/yai-cr-h-1780000007-407327/store/lmdb
graph_relation_source: lmdb_graph_relations_v0
journal_replay_status: completed
graph_materialize_status: completed
runtime_graph_status: completed
records_seen: 28
records_written: 0
records_duplicate: 28
relations_seen: 68
relations_written: 0
relations_duplicate: 68
relations_skipped: 0
nodes_total: 44
edges_total: 68
outgoing_index_entries: 40
incoming_index_entries: 31
runtime_generation: 1
dirty: no
stale: no
rebuild_status: completed
report_schema: yai.runtime_graph_rebuild_report.v1
rebuild_report: /tmp/yai-cr-h-1780000007-407327/store/graph/rebuild-reports/case_new12-filesystem.runtime-graph-rebuild-report.json


## Source: graph-relations.txt

graph_relations:
case_ref: case:new12-filesystem
relations_total: 68
limit: 80
relations:
- relation_id: edge:attempt_targets_subject:rec:new12-fs-block-attempt
  edge_kind: attempt_targets_subject
  from_ref: attempt:new12-fs-block
  to_ref: subject:filesystem-sandbox
  source_record_id: rec:new12-fs-block-attempt
- relation_id: edge:attempt_targets_subject:rec:new12-fs-read-attempt
  edge_kind: attempt_targets_subject
  from_ref: attempt:new12-fs-read
  to_ref: subject:filesystem-sandbox
  source_record_id: rec:new12-fs-read-attempt
- relation_id: edge:attempt_targets_subject:rec:new12-fs-write-attempt
  edge_kind: attempt_targets_subject
  from_ref: attempt:new12-fs-write
  to_ref: subject:filesystem-sandbox
  source_record_id: rec:new12-fs-write-attempt
- relation_id: edge:decision_controls_attempt:rec:new12-fs-block-decision
  edge_kind: decision_controls_attempt
  from_ref: decision:new12-fs-block
  to_ref: attempt:new12-fs-block
  source_record_id: rec:new12-fs-block-decision
- relation_id: edge:decision_controls_attempt:rec:new12-fs-write-decision
  edge_kind: decision_controls_attempt
  from_ref: decision:new12-fs-write
  to_ref: attempt:new12-fs-write
  source_record_id: rec:new12-fs-write-decision
- relation_id: edge:policy_constrains_subject:rec:new12-fs-authority-scope-fs
  edge_kind: policy_constrains_subject
  from_ref: rec:new12-fs-authority-scope-fs
  to_ref: subject:filesystem-sandbox
  source_record_id: rec:new12-fs-authority-scope-fs
- relation_id: edge:policy_constrains_subject:rec:new12-fs-authority-scope-model
  edge_kind: policy_constrains_subject
  from_ref: rec:new12-fs-authority-scope-model
  to_ref: subject:llm-provider
  source_record_id: rec:new12-fs-authority-scope-model
- relation_id: edge:policy_constrains_subject:rec:new12-fs-authority-scope-terminal
  edge_kind: policy_constrains_subject
  from_ref: rec:new12-fs-authority-scope-terminal
  to_ref: subject:linenoise-terminal
  source_record_id: rec:new12-fs-authority-scope-terminal
- relation_id: edge:policy_constrains_subject:rec:new12-fs-policy-rule-fs
  edge_kind: policy_constrains_subject
  from_ref: rec:new12-fs-policy-rule-fs
  to_ref: subject:policy-pack
  source_record_id: rec:new12-fs-policy-rule-fs
- relation_id: edge:projection_exposes_record:rec:new12-fs-model-projection
  edge_kind: projection_exposes_record
  from_ref: rec:new12-fs-model-projection
  to_ref: rec:new12-fs-model-projection
  source_record_id: rec:new12-fs-model-projection
- relation_id: edge:projection_exposes_record:rec:new12-fs-model-projection-request
  edge_kind: projection_exposes_record
  from_ref: rec:new12-fs-model-projection-request
  to_ref: rec:new12-fs-model-projection-request
  source_record_id: rec:new12-fs-model-projection-request
- relation_id: edge:projection_exposes_record:rec:new12-fs-projection
  edge_kind: projection_exposes_record
  from_ref: rec:new12-fs-projection
  to_ref: rec:new12-fs-projection
  source_record_id: rec:new12-fs-projection
- relation_id: edge:receipt_records_effect:rec:new12-fs-block-receipt
  edge_kind: receipt_records_effect
  from_ref: receipt:new12-fs-block
  to_ref: effect:attempt:new12-fs-block
  source_record_id: rec:new12-fs-block-receipt
- relation_id: edge:receipt_records_effect:rec:new12-fs-read-receipt
  edge_kind: receipt_records_effect
  from_ref: receipt:new12-fs-read
  to_ref: effect:attempt:new12-fs-read
  source_record_id: rec:new12-fs-read-receipt
- relation_id: edge:receipt_records_effect:rec:new12-fs-write-receipt
  edge_kind: receipt_records_effect
  from_ref: receipt:new12-fs-write
  to_ref: effect:attempt:new12-fs-write
  source_record_id: rec:new12-fs-write-receipt
- relation_id: edge:record_materializes_node:rec:new12-fs-authority-scope-fs
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-authority-scope-fs
  to_ref: rec:new12-fs-authority-scope-fs
  source_record_id: rec:new12-fs-authority-scope-fs
- relation_id: edge:record_materializes_node:rec:new12-fs-authority-scope-model
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-authority-scope-model
  to_ref: rec:new12-fs-authority-scope-model
  source_record_id: rec:new12-fs-authority-scope-model
- relation_id: edge:record_materializes_node:rec:new12-fs-authority-scope-terminal
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-authority-scope-terminal
  to_ref: rec:new12-fs-authority-scope-terminal
  source_record_id: rec:new12-fs-authority-scope-terminal
- relation_id: edge:record_materializes_node:rec:new12-fs-block-attempt
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-block-attempt
  to_ref: attempt:new12-fs-block
  source_record_id: rec:new12-fs-block-attempt
- relation_id: edge:record_materializes_node:rec:new12-fs-block-decision
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-block-decision
  to_ref: decision:new12-fs-block
  source_record_id: rec:new12-fs-block-decision
- relation_id: edge:record_materializes_node:rec:new12-fs-block-receipt
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-block-receipt
  to_ref: receipt:new12-fs-block
  source_record_id: rec:new12-fs-block-receipt
- relation_id: edge:record_materializes_node:rec:new12-fs-case
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-case
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-case
- relation_id: edge:record_materializes_node:rec:new12-fs-case-attachment-sandbox
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-case-attachment-sandbox
  to_ref: rec:new12-fs-case-attachment-sandbox
  source_record_id: rec:new12-fs-case-attachment-sandbox
- relation_id: edge:record_materializes_node:rec:new12-fs-case-binding-model
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-case-binding-model
  to_ref: rec:new12-fs-case-binding-model
  source_record_id: rec:new12-fs-case-binding-model
- relation_id: edge:record_materializes_node:rec:new12-fs-case-domain
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-case-domain
  to_ref: rec:new12-fs-case-domain
  source_record_id: rec:new12-fs-case-domain
- relation_id: edge:record_materializes_node:rec:new12-fs-edge
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-edge
  to_ref: rec:new12-fs-edge
  source_record_id: rec:new12-fs-edge
- relation_id: edge:record_materializes_node:rec:new12-fs-memory
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-memory
  to_ref: rec:new12-fs-memory
  source_record_id: rec:new12-fs-memory
- relation_id: edge:record_materializes_node:rec:new12-fs-model-edge
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-model-edge
  to_ref: rec:new12-fs-model-edge
  source_record_id: rec:new12-fs-model-edge
- relation_id: edge:record_materializes_node:rec:new12-fs-model-projection
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-model-projection
  to_ref: rec:new12-fs-model-projection
  source_record_id: rec:new12-fs-model-projection
- relation_id: edge:record_materializes_node:rec:new12-fs-model-projection-request
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-model-projection-request
  to_ref: rec:new12-fs-model-projection-request
  source_record_id: rec:new12-fs-model-projection-request
- relation_id: edge:record_materializes_node:rec:new12-fs-model-subject
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-model-subject
  to_ref: subject:llm-provider
  source_record_id: rec:new12-fs-model-subject
- relation_id: edge:record_materializes_node:rec:new12-fs-policy-edge
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-policy-edge
  to_ref: rec:new12-fs-policy-edge
  source_record_id: rec:new12-fs-policy-edge
- relation_id: edge:record_materializes_node:rec:new12-fs-policy-rule-fs
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-policy-rule-fs
  to_ref: rec:new12-fs-policy-rule-fs
  source_record_id: rec:new12-fs-policy-rule-fs
- relation_id: edge:record_materializes_node:rec:new12-fs-policy-subject
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-policy-subject
  to_ref: subject:policy-pack
  source_record_id: rec:new12-fs-policy-subject
- relation_id: edge:record_materializes_node:rec:new12-fs-projection
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-projection
  to_ref: rec:new12-fs-projection
  source_record_id: rec:new12-fs-projection
- relation_id: edge:record_materializes_node:rec:new12-fs-projection-rule-model
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-projection-rule-model
  to_ref: rec:new12-fs-projection-rule-model
  source_record_id: rec:new12-fs-projection-rule-model
- relation_id: edge:record_materializes_node:rec:new12-fs-read-attempt
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-read-attempt
  to_ref: attempt:new12-fs-read
  source_record_id: rec:new12-fs-read-attempt
- relation_id: edge:record_materializes_node:rec:new12-fs-read-receipt
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-read-receipt
  to_ref: receipt:new12-fs-read
  source_record_id: rec:new12-fs-read-receipt
- relation_id: edge:record_materializes_node:rec:new12-fs-subject
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-subject
  to_ref: subject:filesystem-sandbox
  source_record_id: rec:new12-fs-subject
- relation_id: edge:record_materializes_node:rec:new12-fs-terminal-subject
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-terminal-subject
  to_ref: subject:linenoise-terminal
  source_record_id: rec:new12-fs-terminal-subject
- relation_id: edge:record_materializes_node:rec:new12-fs-write-attempt
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-write-attempt
  to_ref: attempt:new12-fs-write
  source_record_id: rec:new12-fs-write-attempt
- relation_id: edge:record_materializes_node:rec:new12-fs-write-decision
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-write-decision
  to_ref: decision:new12-fs-write
  source_record_id: rec:new12-fs-write-decision
- relation_id: edge:record_materializes_node:rec:new12-fs-write-receipt
  edge_kind: record_materializes_node
  from_ref: rec:new12-fs-write-receipt
  to_ref: receipt:new12-fs-write
  source_record_id: rec:new12-fs-write-receipt
- relation_id: edge:subject_participates_in_case:rec:new12-fs-authority-scope-fs
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-authority-scope-fs
- relation_id: edge:subject_participates_in_case:rec:new12-fs-authority-scope-model
  edge_kind: subject_participates_in_case
  from_ref: subject:llm-provider
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-authority-scope-model
- relation_id: edge:subject_participates_in_case:rec:new12-fs-authority-scope-terminal
  edge_kind: subject_participates_in_case
  from_ref: subject:linenoise-terminal
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-authority-scope-terminal
- relation_id: edge:subject_participates_in_case:rec:new12-fs-block-attempt
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-block-attempt
- relation_id: edge:subject_participates_in_case:rec:new12-fs-block-decision
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-block-decision
- relation_id: edge:subject_participates_in_case:rec:new12-fs-block-receipt
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-block-receipt
- relation_id: edge:subject_participates_in_case:rec:new12-fs-case-attachment-sandbox
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-case-attachment-sandbox
- relation_id: edge:subject_participates_in_case:rec:new12-fs-edge
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-edge
- relation_id: edge:subject_participates_in_case:rec:new12-fs-memory
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-memory
- relation_id: edge:subject_participates_in_case:rec:new12-fs-model-edge
  edge_kind: subject_participates_in_case
  from_ref: subject:llm-provider
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-model-edge
- relation_id: edge:subject_participates_in_case:rec:new12-fs-model-projection
  edge_kind: subject_participates_in_case
  from_ref: subject:llm-provider
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-model-projection
- relation_id: edge:subject_participates_in_case:rec:new12-fs-model-projection-request
  edge_kind: subject_participates_in_case
  from_ref: subject:llm-provider
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-model-projection-request
- relation_id: edge:subject_participates_in_case:rec:new12-fs-model-subject
  edge_kind: subject_participates_in_case
  from_ref: subject:llm-provider
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-model-subject
- relation_id: edge:subject_participates_in_case:rec:new12-fs-policy-edge
  edge_kind: subject_participates_in_case
  from_ref: subject:policy-pack
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-policy-edge
- relation_id: edge:subject_participates_in_case:rec:new12-fs-policy-rule-fs
  edge_kind: subject_participates_in_case
  from_ref: subject:policy-pack
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-policy-rule-fs
- relation_id: edge:subject_participates_in_case:rec:new12-fs-policy-subject
  edge_kind: subject_participates_in_case
  from_ref: subject:policy-pack
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-policy-subject
- relation_id: edge:subject_participates_in_case:rec:new12-fs-projection
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-projection
- relation_id: edge:subject_participates_in_case:rec:new12-fs-projection-rule-model
  edge_kind: subject_participates_in_case
  from_ref: subject:policy-pack
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-projection-rule-model
- relation_id: edge:subject_participates_in_case:rec:new12-fs-read-attempt
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-read-attempt
- relation_id: edge:subject_participates_in_case:rec:new12-fs-read-receipt
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-read-receipt
- relation_id: edge:subject_participates_in_case:rec:new12-fs-subject
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-subject
- relation_id: edge:subject_participates_in_case:rec:new12-fs-terminal-subject
  edge_kind: subject_participates_in_case
  from_ref: subject:linenoise-terminal
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-terminal-subject
- relation_id: edge:subject_participates_in_case:rec:new12-fs-write-attempt
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-write-attempt
- relation_id: edge:subject_participates_in_case:rec:new12-fs-write-decision
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-write-decision
- relation_id: edge:subject_participates_in_case:rec:new12-fs-write-receipt
  edge_kind: subject_participates_in_case
  from_ref: subject:filesystem-sandbox
  to_ref: case:new12-filesystem
  source_record_id: rec:new12-fs-write-receipt


## Source: graph-runtime-summary.txt

runtime_graph_summary:
case_ref: case:new12-filesystem
source: graph_relations
nodes_total: 44
edges_total: 68
outgoing_index_entries: 40
incoming_index_entries: 31
generation: 1
dirty: no
stale: no
durable_truth: graph_persistence
resident: false
resident_service: planned
hnsw: future_candidate_index
context_compiler: future_consumer


## Source: install-local.txt

CARGO_TARGET_DIR=target cargo build --manifest-path engine/Cargo.toml -p yai-engine
CARGO_TARGET_DIR=target cargo build --manifest-path engine/Cargo.toml --workspace
CARGO_TARGET_DIR=target cargo test --manifest-path engine/Cargo.toml --workspace

running 14 tests
test journal::tests::inspect_reports_valid_duplicate_and_invalid_lines ... ok
test tests::build_authority_projection_summary ... ok
test tests::build_filesystem_projection_summary ... ok
test tests::build_control_projection_summary ... ok
test tests::append_record_and_build_projection_summary ... ok
test tests::build_hardened_projection_summary ... ok
test tests::build_graph_projection_summary ... ok
test tests::build_memory_projection_summary ... ok
test tests::build_reconcile_projection_summary ... ok
test tests::query_records_by_kind_and_case ... ok
test tests::parse_jsonl_and_build_projection_summary ... ok
test tests::rust_engine_ffi_append_and_count ... ok
test store::lmdb::tests::journal_import_report_is_idempotent ... ok
test store::lmdb::tests::freeze_supports_control_carrier_and_divergence_records ... ok

test result: ok. 14 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s


running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s


running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s


running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

CARGO_TARGET_DIR=target cargo build --manifest-path cmd/yai/Cargo.toml
CARGO_TARGET_DIR=target cargo test --manifest-path cmd/yai/Cargo.toml

running 0 tests

test result: ok. 0 passed; 0 failed; 0 ignored; 0 measured; 0 filtered out; finished in 0.00s

installed: /tmp/yai-cr-i-1780000007-407327/bin/yai
installed: /tmp/yai-cr-i-1780000007-407327/bin/yaid
yai local doctor
PREFIX: /tmp/yai-cr-i-1780000007-407327
YAI_HOME: /tmp/yai-cr-h-1780000007-407327
YAI_HOME_status: ok
binary_path: /tmp/yai-cr-i-1780000007-407327/bin/yai
yaid_path: /tmp/yai-cr-i-1780000007-407327/bin/yaid
yai_version: yai 0.0.0-newcore
run_dir: /tmp/yai-cr-h-1780000007-407327/run
store_dir: /tmp/yai-cr-h-1780000007-407327/store
record_store_path: /tmp/yai-cr-h-1780000007-407327/store/lmdb
record_store_status: not_initialized
record_store_backend: lmdb
log_dir: /tmp/yai-cr-h-1780000007-407327/log
tmp_dir: /tmp/yai-cr-h-1780000007-407327/tmp
cases_dir: /tmp/yai-cr-h-1780000007-407327/cases
sockets_dir: /tmp/yai-cr-h-1780000007-407327/sockets
config_dir: /tmp/yai-cr-h-1780000007-407327/config
daemon_socket_default: /tmp/yai-cr-h-1780000007-407327/run/yaid.sock
hot_state_path: /tmp/yai-cr-h-1780000007-407327/run/hot-state.json
hot_state_status: missing
runtime_layout_status: ok
PATH_status: warning add /tmp/yai-cr-i-1780000007-407327/bin to PATH


## Source: journal-replay.txt

journal_replay: completed
journal_path: build/tmp/new12/daemon-407535/filesystem/journal.jsonl
journal_identity: journal:0e453c4c29675ddf
lmdb_path: /tmp/yai-cr-h-1780000007-407327/store/lmdb
record_schema: yai.record.v1
journal_schema: yai.store.record.v0
compatibility: compatible
lines_total: 28
lines_replayed: 28
valid_entries: 28
invalid_entries: 0
unsupported_entries: 0
duplicate_entries: 0
records_seen: 28
records_written: 0
records_duplicate: 28
records_skipped: 0
cursor_line: 28
replay_status: completed
replay_ready: yes
replay_report: /tmp/yai-cr-h-1780000007-407327/store/replay/reports/journal:0e453c4c29675ddf.replay-report.json
record_store_status: ready
idempotent: yes


## Source: provider-runtime-status.txt

provider_runtime:
  status: surface_v0
  execution: dry_run_only
  provider_supervision: planned
  device_registry: active
  runtime_targets:
  - local
  - lan
  - external
local:
  supervisor: planned
lan:
  requires_paired_device: yes
  requires_trusted_device: yes
  supervisor: planned
external:
  attach_only: yes


## Source: provider-start-external.txt

provider_start_plan:
  target: external
  provider_kind: openai_compatible
  endpoint: http://127.0.0.1:43117/v1/chat/completions
  status: planned
  action: attach_only
  would_contact_device: false
  would_start_provider: false
  execution_performed: false
  logs_path: /tmp/yai-cr-h-1780000007-407327/log/providers
  receipt_required: yes


## Source: provider-targets.txt

runtime_targets:
- local
- lan
- external
local:
  provider_process: same_machine
  supervisor: planned
lan:
  device_is_runtime_target: yes
  requires_paired_device: yes
  requires_trusted_device: yes
  supervisor: planned
external:
  attach_only: yes
  supervisor: not_owned


## Source: pack-fixture/README.md

# Filesystem Loop Pack Fixture

This directory is the filesystem-loop lab input fixture shaped toward existing
pack materialization doctrine. It is not a complete pack runtime, installer,
registry backend or source-root production pack.

The current useful material is in `policies/`. It represents policy,
projection and authority posture for the bounded filesystem-loop case. See
`pack-doctrine-mapping.md` for the fixture-to-doctrine mapping.

Current implementation posture:

```text
yai daemon run-filesystem-loop
```

materializes equivalent residue as `subject:policy-pack`, `policy_rule`,
`projection_rule`, `authority_scope` and graph evidence.

## Files

| Path | Role |
|---|---|
| `pack.yaml` | Minimal fixture identity and limitations. |
| `policies/filesystem-sandbox-policy.json` | Filesystem sandbox policy posture. |
| `policies/linenoise-terminal-interface-policy.json` | Terminal/interface authority posture. |
| `policies/model-case-projection-policy.json` | Model projection and authority posture. |
| `pack-doctrine-mapping.md` | Mapping to existing pack doctrine. |


## Source: pack-fixture/pack-doctrine-mapping.md

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

- `work/archive/architecture-snapshots/15-pack-materialization.md`
- `work/archive/adr/0017-packs-as-case-materialization-units.md`
- `work/archive/engineering-snapshots/pack-format.md`
- `work/archive/architecture-snapshots/03A-case-world-model.md`

The table below compares the current filesystem-loop fixtures against the
material areas listed by `work/archive/engineering-snapshots/pack-format.md`.

| Pack doctrine area | Represented by current fixture? | Evidence path | Notes |
|---|---|---|---|
| `identity` | Partial | `pack.yaml`; `policy_ref`, `case_ref`, `subject_ref` in each JSON fixture | Fixture identity exists, but there is no production pack identity. |
| `kind` | No | `work/archive/engineering-snapshots/pack-format.md` | No production pack kind is declared; `pack.yaml` declares `role: lab_fixture`. |
| `owner` | No | `work/archive/engineering-snapshots/pack-format.md` | No production pack owner field is declared. |
| `version` | Partial | `schema: yai.lab.policy_pack.v0` and `policy_ref` values | Fixture schema/policy version exists, but no pack version exists. |
| `status` | No | `work/archive/engineering-snapshots/pack-format.md` | No pack lifecycle status is declared. |
| `compatibility` | No | `work/archive/engineering-snapshots/pack-format.md` | No YAI version compatibility is declared. |
| `domains` | Partial | `case_ref: case:new12-filesystem` | The fixture is tied to one case, but no formal pack domain section exists. |
| `materials` | Partial | Fixture summaries and rule bodies | The JSON files are material, but they are not listed under a pack manifest `materials` section. |
| `policies` | Yes, fixture-local | `policies/filesystem-sandbox-policy.json`; `rule_class: policy_rule` | Policy posture is represented for this lab fixture. This does not prove that packs are policy-only. |
| `projection_rules` | Yes, fixture-local | `policies/model-case-projection-policy.json`; `rule_class: projection_rule` | Model projection posture is represented for this lab fixture. |
| `authority_scopes` | Yes, fixture-local | `policies/linenoise-terminal-interface-policy.json`; `policies/model-case-projection-policy.json`; `rule_class: authority_scope` | Authority posture is represented for terminal and model-provider boundaries. |
| `procedures` | No | `work/archive/engineering-snapshots/pack-format.md` | No procedure material is represented by these fixtures. |
| `templates` | No | `work/archive/engineering-snapshots/pack-format.md` | No template material is represented. |
| `output_schemas` | No | `work/archive/engineering-snapshots/pack-format.md` | No output schema material is represented. |
| `memory_seeds` | No | `work/archive/engineering-snapshots/pack-format.md` | No memory seed material is represented. |
| `actors` | No | `work/archive/engineering-snapshots/pack-format.md` | The fixtures mention subjects, but do not define pack actor material. |
| `tools` | No | `work/archive/engineering-snapshots/pack-format.md` | No tool material is represented. |
| `fixtures` | Yes | This directory and `README.md` | The files are lab fixtures for filesystem-loop validation. |
| `limits` | No | `work/archive/engineering-snapshots/pack-format.md` | No material, policy, projection or fixture budgets are declared. |
| `provenance` | Partial | `source_ref: filesystem-loop-lab` | Fixture source is declared; full pack provenance is not. |
| `validation` | Partial | `evidence_required` fields | Evidence requirements exist, but no pack validation section exists. |
| `retirement` | No | `work/archive/engineering-snapshots/pack-format.md` | No retirement or replacement posture is declared. |

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


## Source: pack-fixture/pack.yaml

fixture_id: filesystem-loop.pack-fixture.v0
name: Filesystem Loop Pack Fixture
status: lab_fixture
role: lab_fixture
source: filesystem-loop
case_ref: case:new12-filesystem

represents:
  policies:
    - policies/filesystem-sandbox-policy.json
  projections:
    - policies/model-case-projection-policy.json
  authority_posture:
    - policies/linenoise-terminal-interface-policy.json
    - policies/model-case-projection-policy.json
  fixtures:
    - policies/filesystem-sandbox-policy.json
    - policies/linenoise-terminal-interface-policy.json
    - policies/model-case-projection-policy.json

materialization_posture:
  current: represented_by_filesystem_loop_lab
  runtime_command: not implemented
  installer: not implemented
  registry_backend: not implemented

limitations:
  - not runtime pack
  - not installer
  - not registry
  - not production source root
  - partial fixture only
  - policy material is represented but not mandatory for every pack


## Source: pack-fixture/policies/filesystem-sandbox-policy.json

{
  "schema": "yai.lab.policy_pack.v0",
  "policy_ref": "policy:manual-filesystem-sandbox-v0",
  "case_ref": "case:new12-filesystem",
  "subject_ref": "subject:filesystem-sandbox",
  "source_ref": "filesystem-loop-lab",
  "summary": "Filesystem effects are admissible only inside sandbox scope; decisions provide allow/block outcomes.",
  "rules": [
    {
      "rule_ref": "rule:filesystem-sandbox-boundary",
      "rule_class": "policy_rule",
      "condition": "effect_class in [fs.read, fs.write]",
      "target_subject": "subject:filesystem-sandbox",
      "scope": "sandbox_only",
      "constraint": "sandbox_only",
      "decision_consequence": "allow_with_constraints when sandbox_only is satisfied; block or require_review otherwise",
      "failure_mode": "deny_without_receipt_path",
      "evidence_required": ["decision", "filesystem_receipt"]
    }
  ]
}


## Source: pack-fixture/policies/linenoise-terminal-interface-policy.json

{
  "schema": "yai.lab.policy_pack.v0",
  "policy_ref": "policy:manual-linenoise-terminal-interface-v0",
  "case_ref": "case:new12-filesystem",
  "subject_ref": "subject:linenoise-terminal",
  "source_ref": "filesystem-loop-lab",
  "summary": "Vendored Linenoise is the YAI CLI display/input surface; it has no execution, policy or provider authority.",
  "rules": [
    {
      "rule_ref": "rule:terminal-interface-no-authority",
      "rule_class": "authority_scope",
      "operation": "operator.prompt.submit",
      "scope": "manual_test_interface",
      "allowed": ["display:projection", "submit:operator_input"],
      "forbidden": ["execute:host_effect", "own:decision_authority", "own:provider_semantics"],
      "requires": ["executable_command_becomes_op_attempt"],
      "constraints": ["no_policy_authority", "no_provider_semantics", "no_runtime_authorization", "no_decision_authority", "case_participant_view_required"],
      "failure_mode": "require_review",
      "evidence_required": ["prompt_text", "provider_output"]
    }
  ]
}


## Source: pack-fixture/policies/model-case-projection-policy.json

{
  "schema": "yai.lab.policy_pack.v0",
  "policy_ref": "policy:manual-model-case-projection-v0",
  "case_ref": "case:new12-filesystem",
  "subject_ref": "subject:llm-provider",
  "source_ref": "filesystem-loop-lab",
  "summary": "Model provider may consume governed projections and produce claims/proposals, not effects.",
  "rules": [
    {
      "rule_ref": "rule:model-context-projection-only",
      "rule_class": "projection_rule",
      "operation": "model.context.read",
      "scope": "case_projection_only",
      "allowed": ["consume:model_context_projection", "produce:claim", "produce:proposal", "produce:model_interpretation"],
      "forbidden": ["direct_filesystem_authority", "direct_system_authority", "direct_carrier_execution", "out_of_case_memory"],
      "requires": ["proposed_action_becomes_op_attempt"],
      "constraints": ["no_raw_journal_unless_projected", "no_out_of_case_memory", "output_not_authoritative_state"],
      "failure_mode": "defer_until_policy_available",
      "evidence_required": ["projection_result", "memory_candidate"]
    },
    {
      "rule_ref": "rule:model-output-is-observation-not-decision",
      "rule_class": "authority_scope",
      "operation": "model.output.import",
      "scope": "case_only",
      "constraints": ["output_is_claim_or_proposal", "control_retains_decision_authority", "interpretation_checked_against_residue"],
      "failure_mode": "require_review",
      "evidence_required": ["provider_prompt", "provider_output", "projection_result", "model_interpretation"]
    }
  ]
}
