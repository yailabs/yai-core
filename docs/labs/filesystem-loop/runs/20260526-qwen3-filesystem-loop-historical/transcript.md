# Transcript - Qwen3 Filesystem Loop Historical Run

## Execution Record

This run records the historical Qwen3 8B Q4 filesystem-loop material in the
compact filesystem-loop layout. It does not execute a new model run.

The source material was previously kept as model-behavior lab material and is
now represented as a filesystem-loop run because the protocol under test is the
bounded filesystem loop. The folded historical files are treated as source
artifacts for this compact package.

## Scope

- Historical Qwen3 8B Q4 filesystem-loop lab note.
- Historical prompt artifacts extracted from that note.
- Historical model-behavior output contract reference.
- Derived compact manifest and metrics package.

## Commands

No command was executed for this compact run. The historical note states that a
temporary local `llama-server`, `target/debug/yai` and `build/yaid` were used
during the original rerun, but this compact package does not reconstruct that
execution.

## Inputs

| Input | Status |
|---|---|
| `assets/model-behavior-filesystem-loop-qwen3-8b-q4.md` | Historical artifact |
| `assets/model-behavior-outputs.md` | Historical artifact |
| `assets/P001-subjects.md` through `assets/P006-attempts-and-receipts.md` | Historical prompt artifacts |

## Pack Fixture Used

This historical run is now associated with the current filesystem-loop
`../../pack-fixture/` as lab input posture:

- `../../pack-fixture/pack.yaml`
- `../../pack-fixture/policies/filesystem-sandbox-policy.json`
- `../../pack-fixture/policies/linenoise-terminal-interface-policy.json`
- `../../pack-fixture/policies/model-case-projection-policy.json`

The historical material records equivalent policy, projection and authority
residue. It does not prove that a generic pack runtime was executed.

## Outputs

| Output | Status |
|---|---|
| `report.md` | Derived compact run report |
| `manifest.json` | Derived compact run manifest |
| `metrics.json` | Derived compact run metrics |
| `assets/D001-filesystem-loop-flow.md` | Historical diagram artifact |
| `assets/TBL001-run-summary.md` | Historical table artifact |

## Measurements

| Metric | Value |
|---|---|
| Prompt count | 6 |
| Model outputs | 6 |
| Filesystem receipts | 3 |
| Latency | Not measured |
| Tokens per second | Not measured |
| VRAM peak | Not measured |
| New model execution | No |
| Quantitative benchmark | No |

## Limitations

- Historical baseline only.
- No new model execution was performed.
- Raw execution capture was not committed with the historical material.
- Quantitative runtime metrics are not available.
- This run does not claim model quality, model safety or benchmark superiority.
