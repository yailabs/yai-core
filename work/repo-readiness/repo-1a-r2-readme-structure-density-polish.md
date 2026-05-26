# REPO.1A.R2 README Structure / Density Polish

## Summary

This pass tightened `README.md` without changing the repository philosophy or
adding new public documentation. The main structural change was merging the
duplicated `Execution Boundary` and `Core Model` diagrams into one
`Operational Model` section.

## What Changed

- Strengthened the opening definition of YAI Core as a local runtime for
  binding AI-mediated activity to operational cases.
- Tightened `Status` around source availability, technical evaluation, current
  readiness, command/test stabilization, and legal posture.
- Made `Why This Exists` more direct about tools, files, services, providers,
  memory, workflows, operator decisions, and the need for case-bound controls.
- Kept `What YAI Is` compact while reducing overlap with the operational model.
- Renamed `Design Bet` to `Design Constraints`.
- Tightened provider wording in `What YAI Is Not`.
- Replaced `Execution Boundary` and `Core Model` with a single
  `Operational Model` flow.
- Made `Current Implementation Surface` concrete around `cmd/`, `system/`,
  `engine/`, `include/`, and `tests/`.
- Updated `Current Validation` to point at the current command surface until a
  future `docs/commands.md` split.
- Reordered `Documentation` around the public documentation index and current
  technical references.
- Sharpened `Current Limitations`.

## Files Read

- `README.md`
- `docs/index.md`
- `docs/technical-brief.md`
- `docs/architecture.md`
- Repository documentation file list under `docs/`

## Files Updated

- `README.md`
- `work/repo-readiness/repo-1a-r2-readme-structure-density-polish.md`

## Duplicated Diagrams Merged

The previous README had two related flows:

- `input/proposal -> case binding -> subject/effect boundary -> control decision -> effect or observation -> receipt -> record -> projection/memory`
- `case -> subject/provider -> attempted operation -> control decision -> effect/observation -> receipt -> record -> projection/memory`

They are now represented by one flow:

```text
input/proposal -> case binding -> subject/provider boundary -> control decision -> effect/observation -> receipt -> record -> projection/memory
```

## Section Changes

- `Design Bet` became `Design Constraints`.
- `Execution Boundary` and `Core Model` became `Operational Model`.
- `Current Validation`, `Current Implementation Surface`, `Documentation`, and
  `Current Limitations` were tightened.

## Material Intentionally Not Restored

- Long computational model glossary
- Integration modes
- Memory model
- Long policy and control material
- SPINE references
- NEW references
- CVQ references
- Absorbed concept history
- Full hot-state command list
- Roadmap or history dumps

## Validation Results

Validation performed for this pass:

- `git diff --check`: passed.
- README link existence check: passed.
- README forbidden-term grep for `SPINE.`, `NEW.`, `CVQ`, `intelligence layer`,
  `startup`, `agent OS`, `platform`, `open source`, and `DS4`: only allowed
  README matches remained.
- `make info`: passed.

Allowed README grep matches:

- `platform`: only in `not a cloud platform`.
- `open source`: only in `not open source`.
- `DS4`: only as a conservative provider example and as a no-integration
  limitation.

## Remaining Risks

- Engineering references still contain internal or historical material while
  public documentation continues to be split.
- Command and test surfaces are still stabilizing.
- Provider/backend wording remains intentionally conservative and does not
  claim tested provider breadth.
- Data-plane ownership continues to consolidate between transitional C paths
  and the Rust engine.
- Legal review is still required before commercial or public launch use.

## Next Wave

REPO.2 Command / Daemon Surface Reference.
