# REPO.-1 Surface Inventory / Old-New Cutover

Status: inventory map only.

This document inventories the current YAI repository surface before adding
new public-facing documentation. It does not rewrite the repository README, does
not create final legal text, does not implement provider or DS4 integration,
and does not delete source files.

Goal for the next waves: make the repository contact-ready for technical
readers as a source-available YAI repository. `README.md` should become
the portal; docs should be short and navigable; commands, reproducible test
cases, and backend/provider boundaries should be explicit.

## Current Top-Level Surface

Observed root entries:

```text
LICENSE
Makefile
README.md
VERSION
cmd/
docs/
engine/
examples/
include/
packaging/
proto/
system/
tests/
tools/
vendor/
```

Generated or local entries also present:

```text
build/
target/
engine/target/
```

Requested surfaces not present in this checkout:

```text
Documentation/
notebooks/
scripts/
third_party/
CONTRIBUTING*
SECURITY*
NOTICE*
LICENSE.md
NOTICE.md
THIRD_PARTY_NOTICES.md
```

## Classification Legend

- `keep`: preserve in place with small cleanup only.
- `rewrite`: keep the location but replace or compress the public-facing text.
- `move`: relocate active material into a clearer public, internal, archive, or
  test/example surface.
- `archive`: preserve as historical/internal material outside the public path.
- `delete`: remove only generated/cache artifacts or empty obsolete placeholders.

## Root And Public Directory Inventory

| Surface | Current role | Public-facing | Action | Reason | Target location |
|---|---|---:|---|---|---|
| `README.md` | Main repo portal; mixes identity, architecture thesis, glossary, status, command snippets, and licensing header. | yes | rewrite | Too long and internally dense for first-contact readers. Keep it technical, not marketing. It should route to short docs instead of carrying the whole doctrine. | `README.md` |
| `LICENSE` | Source-available community source notice. References `LICENSING.md` and `COMMERCIAL.md`, which are absent. | yes | rewrite | Legal posture exists but is incomplete and currently has broken references. Do not invent final legal terms in REPO.-1. | `LICENSE.md` plus `docs/legal.md`; optionally keep `LICENSE` as compatibility pointer |
| `NOTICE*` | Missing. | yes | rewrite | Needed for ownership and repository notice separation from license terms. | `NOTICE.md` |
| `THIRD_PARTY_NOTICES*` | Missing. Third-party material currently lives under `vendor/linenoise`. | yes | rewrite | Third-party notices need a dedicated root document before outreach. | `THIRD_PARTY_NOTICES.md` |
| `CONTRIBUTING*` | Missing. | yes | rewrite | Contact-ready source-available repos need contribution posture, scope, and non-production caveats. | `CONTRIBUTING.md` |
| `SECURITY*` | Missing. | yes | rewrite | Needs a security contact/reporting surface even if the project is early. | `SECURITY.md` |
| `docs/` | Active doctrine, architecture, engineering, status, manuals, protocols, product notes, archive, internal inventory. | yes | rewrite | Content is valuable but currently exposes implementation history and product positioning too early. Needs a public index and shorter top-level docs. | `docs/index.md` plus focused public docs |
| `docs/README.md` | Current docs portal; references stale files such as `engineering/target-layout.md`, `engineering/export-matrices.md`, `engineering/rust-engine.md`, and `engineering/c-abi.md`. | yes | rewrite | Public docs entrypoint has broken reading-order targets and old phase labels. | `docs/index.md`; keep `docs/README.md` as pointer or remove after link migration |
| `docs/architecture/` | Canonical architecture doctrine, long-form model docs, repo split notes. | yes | rewrite | Keep as source material, but expose a compressed public `docs/architecture.md`. Full architecture series should be internal or advanced reference. | `docs/architecture.md`; detailed files under `docs/internal/architecture/` or `docs/architecture/reference/` |
| `docs/architecture/README.md` | Architecture portal. Contains old primitive list including `ingest` and labels future YAI. | yes | rewrite | Needs current source-available public language and no stale roots. | `docs/architecture.md` |
| `docs/engineering/` | Active implementation, command, source-surface, roadmap, testing, data-plane and pack docs. | mixed | move | Essential for maintainers, too wave-heavy for public portal. Extract public command/test/backend docs; move wave planning internal. | Public extracts to `docs/commands.md`, `docs/daemon.md`, `docs/backends.md`, `docs/test-cases.md`; planning to `docs/internal/engineering/` |
| `docs/engineering/command-surface.md` | Most complete command inventory. | yes | rewrite | Strong source for command docs, but current format is roadmap-centric and includes manual-test internals. | `docs/commands.md` |
| `docs/engineering/source-surface.md` | Current source boundary: C system roots, Rust engine target, transitional shims. | yes | rewrite | Should become public backend/source-boundary doc with less wave language. | `docs/backends.md` or `docs/architecture.md` |
| `docs/engineering/testing.md` | Test and validation guidance. | yes | rewrite | Should become concise reproducible test-case documentation. | `docs/test-cases.md` |
| `docs/status/` | Implementation status and import map. | mixed | move | Valuable for maintainers; too much wave history for public first contact. | `docs/internal/status/` |
| `docs/status/implementation-status.md` | Long wave history and current status. | no | move | Should not be first-contact public documentation. | `docs/internal/status/implementation-status.md` |
| `docs/protocols/` | First protocol shapes for records, receipts, projection, policy, memory, etc. | mixed | keep | Useful advanced technical reference, but should not be in the short public path until indexed. | `docs/protocols/` with link from `docs/architecture.md` only when stable |
| `docs/manuals/` | Operator validation manual and notebooks, with example policy packs. | mixed | move | Manual content is useful, notebooks should move to the requested notebook legacy area or be linked from test cases. | Markdown extract to `docs/quickstart.md` and `docs/test-cases.md`; notebooks to `notebooks/legacy/` |
| `docs/manuals/manual-filesystem-loop-validation.md` | Detailed local validation runbook. | yes | rewrite | Good source for quickstart/test docs, too long for public portal. | `docs/quickstart.md` and `docs/test-cases.md` |
| `docs/manuals/*.ipynb` | Manual validation notebooks, including Italian companion. | mixed | move | Notebooks are requested under `notebooks/legacy/`; keep as legacy validation assets. | `notebooks/legacy/` |
| `docs/manuals/examples/` | Policy pack fixtures for filesystem-loop validation. | yes | move | Fixtures should live with examples or tests depending on purpose. | `examples/cases/filesystem-loop/` or `tests/cases/filesystem-loop/fixtures/` |
| `docs/product/` | Positioning and wedge notes. | no | archive | Product positioning is not part of the technical public facade and risks sounding like marketing. | `docs/internal/product/` or `docs/archive/product/` |
| `docs/adr/` | Decision records and compendium. | mixed | keep | Useful for serious readers, but not primary portal material. | `docs/adr/` linked from `docs/architecture.md` as advanced context |
| `docs/archive/` | Historical engineering waves, maps, superseded docs, placeholder roots. | no | keep | Correct place for old material; keep out of main reading order. | `docs/archive/` |
| `docs/internal/` | Internal inventory area. | no | keep | Correct home for readiness maps and non-public planning. | `docs/internal/` |
| `docs/reference/figures/brand/yai-transp.png` | Brand figure used by current README. | yes | keep | Static asset. README rewrite can decide whether to keep image. | `docs/reference/figures/brand/` |
| `Documentation/` | Not present. | no | delete | Uppercase duplicate doc root should remain absent to avoid split documentation surfaces. | none |
| `examples/` | Placeholder only. | yes | rewrite | Empty placeholder is weak public surface. Needs provider and case examples or should be explicitly pending. | `examples/providers/`, `examples/cases/` |
| `notebooks/` | Not present. | yes | move | Requested target for legacy notebooks. | `notebooks/legacy/` |
| `tests/` | README placeholders plus active smoke tests. | yes | rewrite | Public readers need reproducible commands and mapping from tests to features. | `tests/cases/` plus `docs/test-cases.md` |
| `tests/smoke/` | Current reproducible C and shell smoke coverage. | yes | keep | Useful technical proof surface; expose through docs rather than moving immediately. | Keep `tests/smoke/`; later add `tests/cases/` wrappers/fixtures |
| `tests/unit/`, `tests/integration/`, `tests/adversarial/` | Placeholder READMEs. | yes | rewrite | Empty categories should be either populated, scoped, or moved out of the public path. | Keep only if roadmap/test docs explain status |
| `scripts/` | Not present. | no | keep | No action. Avoid introducing a second tooling root unless needed. | none |
| `tools/` | Repo check scripts and shell helpers. | mixed | keep | Active validation commands use `tools/checks`. Public docs should mention supported `make` targets, not internal script details. | `tools/` |
| `tools/README.md` | Placeholder. | yes | rewrite | Public-facing placeholder should describe supported checks or point to commands doc. | `tools/README.md` or `docs/commands.md` |
| `third_party/` | Not present. | no | keep | Requested target is absent; current actual third-party root is `vendor/`. Avoid adding duplicate third-party root unless legal reset chooses it. | none |
| `vendor/` | Vendored `linenoise` source and license. | yes | keep | Active dependency; third-party notice should enumerate it. | `vendor/`; notice in `THIRD_PARTY_NOTICES.md` |
| `vendor/linenoise/` | Third-party line editing library. | yes | keep | Preserve license and README. | `vendor/linenoise/` |
| `packaging/` | Placeholder. | yes | archive | No packaging target exists. Avoid public placeholder unless packaging is part of outreach. | `docs/archive/packaging/` or keep hidden until real |
| `proto/` | Schema and fixture skeleton. | yes | rewrite | Could be public protocol reference once schemas/fixtures are real; currently skeletal. | `proto/` plus `docs/architecture.md` reference when ready |
| `cmd/` | Actual command sources: Rust `yai`, C `yaid`. | yes | keep | Important source surface. Public docs should describe commands, not move code. | `cmd/` |
| `include/` | Public C ABI headers and per-module READMEs. | yes | keep | This is public ABI source. Needs backend/API caveats in docs. | `include/` |
| `system/` | C implementation, daemon, carriers, control, transitional data shims. | yes | keep | Public source surface, but docs must say which parts are transitional. | `system/`; boundary summarized in `docs/backends.md` |
| `engine/` | Rust operational data engine workspace. | yes | keep | Future data-plane home; public docs must separate it from C transitional shims. | `engine/`; summarized in `docs/backends.md` |
| `build/` | Local build output. | no | delete | Generated artifact. Should be ignored and absent from clean public tree. | none |
| `target/` | Local Cargo/build output. | no | delete | Generated artifact. Should be ignored and absent from clean public tree. | none |
| `engine/target/` | Local Cargo build output under engine. | no | delete | Generated artifact. Should be ignored and absent from clean public tree. | none |

## Target Public Surface

The next public surface should be small and navigable:

```text
README.md
LICENSE.md
NOTICE.md
THIRD_PARTY_NOTICES.md
CONTRIBUTING.md
SECURITY.md
docs/index.md
docs/quickstart.md
docs/technical-brief.md
docs/architecture.md
docs/commands.md
docs/daemon.md
docs/backends.md
docs/test-cases.md
docs/legal.md
docs/glossary.md
examples/providers/
examples/cases/
notebooks/legacy/
tests/cases/
```

### Public Docs Intent

| Target | Purpose | Source material |
|---|---|---|
| `README.md` | Short portal: what YAI is, what it is not, build/test entrypoints, license posture pointer. | Current `README.md`, `docs/engineering/source-surface.md`, `docs/engineering/command-surface.md` |
| `LICENSE.md` | Final source-available license text or pointer after legal reset. | Current `LICENSE`; legal review required |
| `NOTICE.md` | Copyright and repository notice. | Current `LICENSE` header language; legal review required |
| `THIRD_PARTY_NOTICES.md` | Third-party inventory. | `vendor/README.md`, `vendor/linenoise/LICENSE` |
| `CONTRIBUTING.md` | Contribution scope, non-production posture, checks, expectations. | `Makefile`, `tools/checks/`, `docs/engineering/testing.md` |
| `SECURITY.md` | Security reporting and supported scope. | New content required; legal/security owner required |
| `docs/index.md` | Documentation portal. | `docs/README.md`, corrected |
| `docs/quickstart.md` | Minimal local build/run/test path. | `README.md`, `docs/manuals/manual-filesystem-loop-validation.md`, `Makefile` |
| `docs/technical-brief.md` | Compact technical overview without marketing copy. | `README.md`, `docs/architecture/00-spine.md`, `docs/architecture/01-terminology.md` |
| `docs/architecture.md` | Public architecture summary and boundaries. | `docs/architecture/README.md`, architecture series |
| `docs/commands.md` | Supported command surface. | `docs/engineering/command-surface.md`, `Makefile` |
| `docs/daemon.md` | `yaid`, sockets, foreground/status/info/shutdown flow. | `docs/engineering/command-surface.md`, `system/daemon/`, `cmd/yaid/` |
| `docs/backends.md` | Backend/provider/data-plane boundaries. No untested DS4 claims. | `docs/engineering/source-surface.md`, `engine/README.md`, `system/*/README.md` |
| `docs/test-cases.md` | Reproducible test cases and expected commands. | `tests/smoke/`, `docs/engineering/testing.md`, manual validation doc |
| `docs/legal.md` | Plain-language licensing posture and pointers, not final legal terms. | Current `LICENSE`; legal review required |
| `docs/glossary.md` | Terms such as case, subject, op, control, carrier, receipt, projection. | Current `README.md`, `docs/architecture/01-terminology.md` |

## Proposed Moves And Archives

| Current | Proposed target | Action | Notes |
|---|---|---|---|
| `docs/manuals/manual-filesystem-loop-validation.ipynb` | `notebooks/legacy/manual-filesystem-loop-validation.ipynb` | move | Preserve as legacy validation notebook. |
| `docs/manuals/manual-filesystem-loop-validation.it.ipynb` | `notebooks/legacy/manual-filesystem-loop-validation.it.ipynb` | move | Preserve as legacy/localized validation notebook. |
| `docs/manuals/requirements-notebook.txt` | `notebooks/legacy/requirements.txt` | move | Keep with notebooks. |
| `docs/manuals/examples/filesystem-loop/policy-packs/` | `examples/cases/filesystem-loop/policy-packs/` | move | These are better as case examples if kept public. |
| `docs/manuals/examples/filesystem-loop/` | `tests/cases/filesystem-loop/fixtures/` | alternative move | If they are primarily validation fixtures, prefer test location instead of examples. Choose one owner. |
| `docs/product/` | `docs/internal/product/` | move | Keep product notes out of public technical facade. |
| `docs/status/` | `docs/internal/status/` | move | Keep implementation history available but not first-contact public. |
| Long-form `docs/engineering/*.md` wave/planning files | `docs/internal/engineering/` | move | Extract concise public docs first. Do not strand links. |
| `docs/archive/` | `docs/archive/` | keep | Already correctly separated. |
| `packaging/README.md` | `docs/archive/packaging/README.md` | archive | Only if packaging remains placeholder after REPO.0. |
| `tests/smoke/*` | `tests/smoke/*` | keep | Do not move active smoke tests during doc cutover. Add public index instead. |
| `tests/cases/` | new | create | Add reproducible case fixtures/wrappers once docs define stable cases. |
| `examples/providers/` | new | create | Provider examples only when provider boundaries are explicit and tested. |
| `examples/cases/` | new | create | Case examples should be runnable and tied to test docs. |

## Public / Internal / Archive Separation

Public docs should include only:

```text
README.md
docs/index.md
docs/quickstart.md
docs/technical-brief.md
docs/architecture.md
docs/commands.md
docs/daemon.md
docs/backends.md
docs/test-cases.md
docs/legal.md
docs/glossary.md
docs/protocols/ when linked as advanced reference
docs/adr/ when linked as advanced reference
```

Internal docs should include:

```text
docs/internal/repo-readiness/
docs/internal/engineering/
docs/internal/status/
docs/internal/product/
```

Archive material should remain under:

```text
docs/archive/
```

Examples should be runnable or intentionally marked pending:

```text
examples/providers/
examples/cases/
```

Tests should separate smoke implementation checks from public cases:

```text
tests/smoke/
tests/cases/
```

Notebooks should be legacy assets unless they become maintained tutorials:

```text
notebooks/legacy/
```

## Risky Ambiguities

- `LICENSE` currently references `LICENSING.md` and `COMMERCIAL.md`, but those
  files are absent. REPO.0 must decide whether to create those exact files or
  replace the references with `LICENSE.md` and `docs/legal.md`.
- The current root README says "Community Source Tree"; the prompt target says
  "source-available YAI repo". REPO.0 should choose one canonical phrase.
- `docs/README.md` points to docs that appear to have been archived or removed.
  Link repair is required before making it a public portal.
- `docs/product/` exists inside the public docs tree. It should move internal or
  archive before outreach to avoid mixing technical facade and positioning.
- `docs/manuals/` contains notebooks and validation fixtures. Decide whether the
  primary owner is examples, tests, or legacy notebooks before moving files.
- `examples/` and several `tests/*` categories are placeholders. Empty public
  surfaces may look unfinished unless the README/docs explain their status.
- `vendor/` is the actual third-party root, while the requested target mentions
  `third_party/`. REPO.0 should choose one naming convention and document it.
- `system/store`, `system/graph`, `system/index`, `system/memory`,
  `system/projection`, and `system/reconcile` are transitional shims. Public
  docs must avoid presenting them as the final data-plane backend.
- Provider boundaries are present conceptually, but examples should not imply
  tested provider breadth. Do not claim DS4 support as tested.
- Generated directories `build/`, `target/`, and `engine/target/` are present in
  the working tree. They should remain ignored and should not be part of any
  public inventory except as generated artifacts.

## Files Read For This Inventory

Root and public docs:

```text
README.md
LICENSE
docs/README.md
docs/architecture/README.md
docs/engineering/command-surface.md
docs/engineering/source-surface.md
docs/status/implementation-status.md
examples/README.md
tests/README.md
tools/README.md
vendor/README.md
packaging/README.md
proto/README.md
engine/README.md
Makefile
```

Directory listings inspected:

```text
docs/
docs/adr/
docs/archive/
docs/architecture/
docs/engineering/
docs/internal/
docs/manuals/
docs/product/
docs/protocols/
docs/reference/
docs/status/
examples/
tests/
tests/smoke/
tools/
vendor/
cmd/
include/
system/
engine/
packaging/
proto/
```

## Files Created Or Updated In REPO.-1

```text
docs/internal/repo-readiness/surface-inventory.md
```

## Next Wave: REPO.0 Repository Identity / Legal Reset

Recommended scope:

1. Choose canonical naming: "YAI", "source-available", and the repository
   license posture.
2. Resolve legal file shape: `LICENSE.md`, `NOTICE.md`,
   `THIRD_PARTY_NOTICES.md`, `docs/legal.md`, and whether compatibility files
   `LICENSE` or `NOTICE` remain.
3. Rewrite only the root README as a concise portal.
4. Create `docs/index.md` and repair/replace stale `docs/README.md` links.
5. Extract public command, daemon, backend, test-case, architecture, quickstart,
   technical-brief, and glossary pages from existing docs.
6. Move product/status/wave planning into `docs/internal/` or keep history in
   `docs/archive/`.
7. Decide the owner for manual notebooks and fixtures:
   `notebooks/legacy/`, `examples/cases/`, or `tests/cases/`.
8. Add contribution and security surfaces without implying open-source status.
9. Run link checks, `git diff --check`, and the relevant repository doc guards.
