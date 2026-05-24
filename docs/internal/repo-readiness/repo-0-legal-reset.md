# REPO.0 Repository Identity / Legal Reset

Status: complete for the narrow REPO.0 scope.

REPO.0 establishes the minimal public legal and identity surface for YAI.
It does not rewrite the README facade, create public documentation indexes,
move notebooks, move product/status docs, add examples, or change command,
daemon, backend, architecture, quickstart, glossary, or test-case docs.

## What Changed

- Chose `YAI` as the product/repository name for legal-facing files.
- Chose `source-available` as the repository posture.
- Made clear that YAI is not open source by default.
- Created a root `LICENSE.md` source-available notice.
- Reduced root `LICENSE` to a compatibility pointer.
- Created `NOTICE.md`.
- Created `THIRD_PARTY_NOTICES.md`.
- Created `CONTRIBUTING.md`.
- Created `SECURITY.md`.
- Created `docs/legal.md`.
- Updated the README legal header and license badge only.

## Files Read

```text
docs/internal/repo-readiness/surface-inventory.md
LICENSE
README.md
vendor/README.md
vendor/linenoise/README.md
vendor/linenoise/LICENSE
Makefile
```

Repository text was also searched for stale legal references, the previous
community-source phrase, and accidental open-source or permissive-license
claims.

## Files Created Or Updated

```text
LICENSE
LICENSE.md
NOTICE.md
THIRD_PARTY_NOTICES.md
CONTRIBUTING.md
SECURITY.md
docs/legal.md
docs/internal/repo-readiness/repo-0-legal-reset.md
README.md
```

## Legal Posture Chosen

```text
Product/repo name: YAI
Repository posture: source-available
Default status: not open source unless a file or component explicitly says otherwise
Allowed posture: public technical evaluation and review
Restricted posture: commercial use, redistribution, hosted use, sublicensing,
incorporation into third-party products, and production use require explicit
written permission unless another file/component license says otherwise
Contribution posture: technical feedback welcome; broad external contributions
not open yet
```

## Broken References Removed

The root `LICENSE` no longer references absent files:

```text
LICENSING.md
COMMERCIAL.md
```

Those names may still appear in internal readiness records as historical
findings from REPO.-1 and REPO.0 validation.

## Third-Party Notice Summary

Verified third-party material:

```text
vendor/linenoise/
vendor/linenoise/LICENSE
```

`THIRD_PARTY_NOTICES.md` names Linenoise, points to the vendored license file,
and does not invent unverified third-party dependencies.

## Remaining Legal Ambiguities

- Final legal entity and copyright holder need confirmation.
- `LICENSE.md` is a repository posture notice, not final lawyer-approved legal
  text.
- Commercial permission workflow is not defined yet.
- Security contact is pending.
- Existing source headers may still use older wording until a later mechanical
  cleanup wave.
- Internal docs still contain historical references to the old legal posture.

## Next Wave

REPO.1 README / Documentation Landing:

```text
README.md
docs/index.md
docs/technical-brief.md
docs/quickstart.md skeleton
docs/architecture.md skeleton
docs/glossary.md skeleton
```

Note for REPO.1:

The README/documentation landing pass should use a systems-project style:
direct identity, current status, hard boundaries, concrete commands, real tests,
known limitations, and links to focused subdocs. Do not copy another project's
text or structure verbatim; use mature technical READMEs only as a quality
reference for clarity, specificity, and restraint. The README should sound like
a serious systems project README, not a startup landing page.
