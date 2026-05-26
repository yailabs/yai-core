# REPO.1A README Technical Portal Pass

Status: complete for the README-only scope.

REPO.1A rewrites the root README into a short technical portal. It does not
create `docs/index.md`, create technical-brief/quickstart/architecture/glossary
skeletons, move notebooks, move product/status docs, add examples, alter
command docs, alter source code, or change build files.

REPO.1A.R1 adds a focused technical density pass. It keeps the README compact
while adding the concrete problem, design bet, execution boundary, and current
implementation surface.

## What Changed

- Replaced the overloaded root README with a compact technical portal.
- Kept the source-available legal header from REPO.0.
- Kept the logo block and source-available license badge.
- Defined YAI as a local control runtime for case-bound AI operation.
- Added an honest status section.
- Made project boundaries explicit.
- Reduced the core model to one compact flow and short term explanations.
- Kept validation to `make info`, `make check`, and a pointer to the existing
  command-surface document.
- Added a compressed repository layout.
- Linked only to documentation and legal files that exist today.
- Added concise current limitations.

REPO.1A.R1:

- Added `Why This Exists`.
- Added `Design Bet`.
- Added `Execution Boundary`.
- Added `Current Implementation Surface`.
- Strengthened current validation language without adding a full command
  reference.
- Expanded limitations to call out data-plane consolidation.

## Files Read

```text
README.md
LICENSE.md
docs/legal.md
work/spines/command-surface.md
work/spines/source-surface.md
work/spines/testing.md
legacy manuals surface/manual-filesystem-loop-validation.md
docs/architecture/README.md
work/repo-readiness/surface-inventory.md
work/repo-readiness/repo-0-legal-reset.md
```

## Files Updated

```text
README.md
work/repo-readiness/repo-1a-readme-technical-portal-pass.md
```

## README Structure Chosen

```text
Project header / badges
One-paragraph definition
Status
What YAI Is
What YAI Is Not
Core Model
Current Validation
Current Implementation Surface
Repository Layout
Documentation
License And Contributions
Current Limitations
```

## Material Removed From README

The previous README embedded material that belongs in focused documentation:

- long computational glossary
- integration-mode doctrine
- model-boundary essay
- memory-model explanation
- policy/control explanation
- SPINE and NEW wave history
- detailed LMDB wave notes
- case-view quality vector details
- absorbed-concepts list
- full hot-state and record-store command list
- long internal documentation pointer list
- notebook references as central README material

The material remains available in existing architecture, engineering, status,
manual, and archive documents until later documentation waves decide whether to
compress, move, or archive it.

Relevant current locations:

```text
docs/architecture/
work/archive/engineering-snapshots/
work/archive/status/
legacy manuals surface/
work/archive/legacy-docs/
work/repo-readiness/surface-inventory.md
```

## Legal And Source Posture Preserved

The README keeps the REPO.0 posture:

```text
YAI Core
source-available
not open source by default
public technical evaluation and review
commercial/public launch use requires explicit permission and legal review
```

The README links to:

```text
LICENSE.md
NOTICE.md
THIRD_PARTY_NOTICES.md
CONTRIBUTING.md
SECURITY.md
docs/legal.md
```

## Known Limitations

- The README now points to existing docs that still contain older internal
  planning and implementation material.
- Public documentation skeletons are not created in this wave.
- Provider/backend examples remain intentionally absent.
- DS4 is mentioned only as an external provider/server example, with no tested
  support claim.
- The worktree contains unrelated dirty source and docs changes that were not
  touched by this README pass.
- `make check` currently fails in `check-repository-identity.sh` because that
  guard still classifies `YAI Core` as an old repository identity. REPO.0 and
  REPO.1A intentionally use `YAI Core` as the legal/public name, so the guard
  needs a later identity-check update rather than a README wording rollback.

## Validation

```text
git diff --check: passed
README link existence check: passed
README forbidden/internal phrase review: passed with acceptable contextual hits
make info: passed
make check: failed on stale repository-identity guard for `YAI Core`
```

REPO.1A.R1 validation:

```text
git diff --check: passed
README link existence check: passed
README internal phrase review: passed with acceptable contextual hits
README length after density pass: within target range
```

## Next Wave

REPO.1B Documentation Landing Skeletons:

```text
docs/index.md
docs/technical-brief.md
docs/quickstart.md skeleton
docs/architecture.md skeleton
docs/glossary.md skeleton
```
