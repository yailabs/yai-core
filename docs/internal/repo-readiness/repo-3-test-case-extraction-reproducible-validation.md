# REPO.3 -- Test Case Extraction / Reproducible Validation

Status: completed locally

## What Changed

Created the first public test-case layer for repository validation:

- repository health;
- local runtime inspection;
- filesystem loop manual validation;
- optional model-behavior lab reference.

The model-behavior lab is treated as optional and experimental. It is not part
of the default public validation baseline.

## Files Read

- `README.md`
- `docs/index.md`
- `docs/quickstart.md`
- `docs/engineering/testing.md`
- `docs/engineering/command-surface.md`
- `legacy manuals surface/manual-filesystem-loop-validation.md`
- `docs/labs/filesystem-loop/README.md`
- `docs/labs/filesystem-loop/prompts.json`
- `tests/README.md`
- `tests/smoke/`
- `Makefile`

`docs/commands.md` and `docs/daemon.md` were expected from the prior command
surface wave but were not present in this checkout. Public test-case docs
therefore link to the existing engineering command and testing references.

## Files Created

- `docs/test-cases.md`
- `tests/cases/README.md`
- `tests/cases/00-repository-health/README.md`
- `tests/cases/01-local-runtime-inspection/README.md`
- `tests/cases/02-filesystem-loop-manual/README.md`
- `tests/cases/03-model-behavior-lab/README.md`
- `docs/internal/repo-readiness/repo-3-test-case-extraction-reproducible-validation.md`

## Files Updated

- `README.md`
- `docs/index.md`
- `docs/quickstart.md`

## Test Cases Added

- `00-repository-health`: current, no model, no daemon.
- `01-local-runtime-inspection`: manual, no model, daemon optional.
- `02-filesystem-loop-manual`: manual, daemon-backed filesystem loop.
- `03-model-behavior-lab`: experimental, optional model/provider lab.

## Status Labels Used

- `current`
- `manual`
- `experimental`
- `internal`
- `planned`

## Commands Used Or Documented

Repository health:

```sh
make info
make check
```

Local runtime inspection:

```sh
yai doctor
yai hot status
yai store status
yai store summary
```

Daemon inspection, when a local daemon socket is prepared:

```sh
yai daemon status --socket <path>
yai daemon info --socket <path>
yai daemon shutdown --socket <path>
```

## Known Failures Or Caveats

- The test-case layer documents existing surfaces; it does not add new test
  harnesses.
- Local runtime inspection may require built or installed binaries.
- Filesystem loop validation remains a manual path.
- The model-behavior lab requires external provider/model setup and model
  output is non-deterministic.
- `docs/commands.md` and `docs/daemon.md` were not present in this checkout.

## Validation Results

Passed:

```sh
git diff --check
make info
```

Additional checks:

- local markdown link targets for new and updated public docs exist;
- `docs/index.md` links to `docs/test-cases.md`;
- `docs/quickstart.md` points to `docs/test-cases.md` without becoming a full
  test reference;
- forbidden/internal term grep over updated public docs found only allowed
  legal/status wording already present in README and docs index:
  `not open source` and `not a cloud platform`.

Current branch caveat:

```sh
make check
```

failed after unrelated SPINE.33A work appeared in the worktree. The latest
failure was:

```text
required control/carrier file missing: docs/architecture/18-control-carrier-substrate.md
make: *** [Makefile:161: check-docs] Error 1
```

This REPO.3 wave did not alter `Makefile`, source files, or check scripts.

## Remaining Risks

- Public command and daemon references still need to be restored or split if
  the previous wave is not present in the branch.
- Manual filesystem-loop docs remain more detailed than the public wrapper.
- Command output shape may still change while the command surface stabilizes.

## Next Wave

REPO.4 Backend / Provider Boundary Reference.
