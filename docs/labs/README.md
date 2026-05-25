# Labs

Status: canonical home for runnable lab procedures, benchmark plans, curated
outputs and publication reports.

`docs/labs/` owns material that can be run, repeated, curated or published.
`docs/manuals/` is retained only as a compatibility surface for older links and
required-file guards.

## Standard Lab Shape

Every lab directory should use the same top-level names:

```text
README.md              purpose, scope, status, canonical links
runbook.md             commands/operators use to run the lab
tests.md               validation checks and expected status signals
outputs.md             output/report/raw-data contract
outputs/               lightweight placeholders or curated small output notes
reports/               curated publishable reports and assets
raw/                   git-kept placeholder only; raw local logs stay ignored
policy-packs/          optional lab fixtures when policy material is required
```

Lab-specific documents may exist, but they should be linked from `README.md`
and should not replace the standard names above.

## Active Labs

| Lab | Purpose | Canonical path | Standard docs |
|---|---|---|---|
| Filesystem loop | Command-surface, case, policy-pack and provider-boundary validation. | `docs/labs/filesystem-loop/` | `README.md`, `runbook.md`, `tests.md`, `outputs.md` |
| Model behavior | Curated examples of model behavior inside a YAI case. | `docs/labs/model-behavior/` | `README.md`, `runbook.md`, `tests.md`, `outputs.md` |
| NVIDIA LAN benchmark | Provider reachability, local runtime, benchmark reports and visual assets. | `docs/labs/nvidia/` | `README.md`, `runbook.md`, `tests.md`, `outputs.md` |

## Scientific Documentation Spine

Shared rules for lab notes, experiment reports, technical reports, papers,
benchmark reports, artifacts and agent-authored lab documents live in:

```text
docs/labs/scientific-docs/
```

Test runs and generated visual artifacts must follow the shared run and visual
artifact contracts.

## Rules

- Runbooks live in labs, not manuals.
- Standard entry files use the same names in every lab.
- New numbered tests live under `docs/labs/<lab>/tests/`.
- Every numbered run owns its own raw, normalized, figures, diagrams, tables
  and exports folders.
- Benchmark reports are generated under the owning lab `reports/` directory.
- Raw benchmark output may be referenced from reports, but should not be treated
  as the report.
- Failed probes remain evidence and must not be hidden.
- No lab artifact may include API keys, authorization headers or secrets.
