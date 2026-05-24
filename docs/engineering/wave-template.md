# Wave Template

Use this template for future SPINE.N deliveries.

```text
SPINE.N - Title

Purpose:
  What this wave changes and why.

Repos touched:
  yai-core
  old yai if residue normalization is required

Spine coverage:
  WORLD / RESIDUE / DATA / VIEW / CONTROL / MEMORY / RECONCILE / HOST / MODEL / OBSERVABILITY / EXTERNAL

Scope:
  In-scope work.

Non-goals:
  Explicit exclusions.

Old-yai source audit:
  exact paths inspected
  files read
  concepts extracted
  code snippets or algorithms mined
  files explicitly not used
  dirty-worktree caveats

Extraction decision table:
  old path
  extracted concept
  target yai-core root
  action: absorb / rewrite / split / externalize / quarantine / compat_only / delete_later / migrated_concept / leave_untouched
  future_repo: yai-core / ai-environment / interfaces / console / none
  status: planned / inspected / imported / rewritten / externalized / archived / blocked_by_dirty_worktree / deferred / complete
  notes

YAI-core implementation:
  new files
  changed files
  moved files
  contracts added
  record kinds added
  commands added
  tests added
  derived_from_old_yai when applicable

Command Surface:
  Added commands:
    every new command, or "none"
  Changed commands:
    every changed command, or "none"
  Removed/deprecated commands:
    every removed or deprecated command, or "none"
  Manual command tests:
    exact commands the operator should run manually
  Expected outputs:
    exact key lines expected from manual commands
  Command docs updated:
    command docs, testing docs or manual runbooks changed

  Rule:
    If no commands changed, state explicitly: no command surface change.

Old-yai residue normalization:
  what was marked historical
  what was moved to archive
  what was documented as externalized to ai-environment
  what was left temporarily and why
  what must not be touched because user/other changes exist

Inventory update:
  docs/internal/extraction-inventory.tsv rows changed
  old-yai inventory rows changed if that repo is touched

Case-world impact:
  domains
  attachments
  bindings
  authority scopes
  sessions/context
  interaction threads

Pack materialization impact:
  pack kinds
  pack manifest/folder shape
  methods/procedures/templates
  policy material and projection rules
  authority scopes
  output schemas
  memory seeds
  actors/tools
  fixtures
  limits/budgets
  case_domain / case_attachment / case_binding materialization

Data-plane impact:
  hot state
  journal
  LMDB future
  Ladybug future
  DuckDB future
  graph
  memory
  projection
  reconcile

Projection/model impact:
  model-visible projection
  authority wording
  model_interpretation
  claim/attempt import
  thread/participant view

Observability/freshness impact:
  trace/timing/freshness
  projection stale/fresh behavior
  Case View Quality
  rebuild diagnostics
  model behavior facts
  memory quality facts

Tests:
  unit
  smoke
  manual
  rebuild/replay
  stale/freshness
  adversarial if applicable

Docs:
  active docs updated
  archive docs updated
  stale docs removed or marked historical

Validation:
  commands run
  known caveats

Commit boundaries:
  yai-core commit
  yai cleanup commit if old yai was modified
  no mixed unrelated changes
```

Operational rule:

```text
A wave is not complete until the corresponding old-yai residue has been classified.
```

Detailed rules live in `operational-extraction-contract.md`.
