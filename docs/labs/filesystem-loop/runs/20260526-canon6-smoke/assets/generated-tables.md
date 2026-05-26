# Generated Run Tables

## Run evidence summary

| Field | Value | Source |
| --- | --- | --- |
| Lab | filesystem-loop | metrics.json / manifest.json |
| Run slug | 20260526-canon6-smoke | metrics.json / manifest.json |
| Command exit code | 0 | metrics.json:command_exit_code |
| commands | 1 | transcript.md + assets/ |
| stdout files | 1 | transcript.md + assets/ |
| stderr files | 1 | transcript.md + assets/ |
| prompts | 0 | transcript.md + assets/ |
| responses | 0 | transcript.md + assets/ |
| receipts | 0 | transcript.md + assets/ |
| errors | 0 | transcript.md + assets/ |

## What was executed

| Item | Value | Source |
| --- | --- | --- |
| Command | `make check-docs` | transcript.md |
| Exit code | 0 | metrics.json |

## Metrics table

| Metric | Value | Source |
| --- | --- | --- |
| status | command_recorded | metrics.json:status |
| command_exit_code | 0 | metrics.json:command_exit_code |

## Artifact index

| Path | Class | Present |
| --- | --- | --- |
| assets/C001-command-status.svg | generated figure | yes |
| assets/C002-execution-evidence-counts.svg | generated figure | yes |
| assets/command.stderr.txt | log | yes |
| assets/command.stdout.txt | log | yes |
| assets/generated-figures.json | generated figure index | yes |
| assets/generated-tables.md | generated report tables | yes |

## Pack fixture table

| Fixture category | Count | Note |
| --- | --- | --- |
| policies | 3 | Available as lab fixture; run use must be stated by transcript/report. |
| pack yaml | 1 | Available as lab fixture; run use must be stated by transcript/report. |
| doctrine mapping | 1 | Available as lab fixture; run use must be stated by transcript/report. |

## Missing measurements

- Benchmark throughput, hardware, VRAM and model-quality measurements: Not measured
- Command duration or endpoint timing: Not measured
- Filesystem receipt evidence: Not measured
- Model or command response sequence: Not measured
- Prompt ID and resolved prompt payload: Not measured

## Interpretation

- The recorded command completed successfully for this run.
- No model-quality or benchmark conclusion is drawn from this run.

## Next run

- Use a prompt-catalog run when prompt/response evidence is needed.
- Record filesystem receipt evidence when mutative operations are intentionally exercised.
