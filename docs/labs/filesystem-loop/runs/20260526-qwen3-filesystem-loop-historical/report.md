# Qwen3 Filesystem Loop Historical Report

## Summary

This run folds the historical Qwen3 8B Q4 filesystem-loop material into the
compact filesystem-loop run layout. It preserves the prompt artifacts and
documented observations without claiming a new execution or quantitative
benchmark.

## Result

The historical note records a bounded filesystem-loop interaction in which the
model received a summary-only case projection. The note records six prompts,
six model outputs, three filesystem receipts and explicit boundaries around raw
journal access, filesystem access, decision authority and receipt authority.

## Metrics

| Field | Value |
|---|---|
| Status | Historical baseline |
| Model family | Qwen3 |
| Model size | 8B |
| Quantization | Q4 |
| Prompt count | 6 |
| Model outputs | 6 |
| Filesystem receipts | 3 |
| Latency | Not measured |
| Tokens per second | Not measured |
| VRAM peak | Not measured |
| New execution | No |
| Quantitative benchmark | No |

## Pack Material Used

The report relates the historical run to the current filesystem-loop
`../../pack-fixture/`. The fixture files provide policy, projection and
authority posture for the bounded case:

- `../../pack-fixture/pack.yaml`
- `../../pack-fixture/policies/filesystem-sandbox-policy.json`
- `../../pack-fixture/policies/linenoise-terminal-interface-policy.json`
- `../../pack-fixture/policies/model-case-projection-policy.json`

## Case Materialization Posture

The historical note records case residue equivalent to
`subject:policy-pack`, `policy_rule`, `projection_rule`, `authority_scope` and
graph evidence. This is materialization posture evidence, not proof that a
generic pack runtime or installer existed.

## Artifacts

| Artifact | Path |
|---|---|
| Historical note | `assets/model-behavior-filesystem-loop-qwen3-8b-q4.md` |
| Historical output reference | `assets/model-behavior-outputs.md` |
| Prompt artifacts | `assets/P001-subjects.md` through `assets/P006-attempts-and-receipts.md` |
| Historical flow diagram | `assets/D001-filesystem-loop-flow.md` |
| Historical run table | `assets/TBL001-run-summary.md` |
| Machine metrics | `metrics.json` |
| Run manifest | `manifest.json` |

## Interpretation

The compact filesystem-loop layout can represent both command smoke runs and
model-backed historical runs without mixing protocol, execution record,
artifact attachments and notebook summaries. The run is useful as historical
evidence of artifact discipline, not as a fresh benchmark.

## Limitations

- No model command was executed during this compacting wave.
- Raw capture for the historical rerun remains unavailable in the repository.
- Runtime throughput, latency and VRAM are not measured.
- The historical model outputs are not treated as authoritative system state.
