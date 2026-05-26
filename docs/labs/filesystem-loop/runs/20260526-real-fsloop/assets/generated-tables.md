# Generated Run Tables

## Run evidence summary

| Field | Value | Source |
| --- | --- | --- |
| Lab | filesystem-loop | metrics.json / manifest.json |
| Run slug | 20260526-real-fsloop | metrics.json / manifest.json |
| Command exit code | 2 | metrics.json:command_exit_code |
| commands | 1 | transcript.md + assets/ |
| stdout files | 1 | transcript.md + assets/ |
| stderr files | 1 | transcript.md + assets/ |
| prompts | 1 | transcript.md + assets/ |
| responses | 0 | transcript.md + assets/ |
| receipts | 0 | transcript.md + assets/ |
| errors | 1 | transcript.md + assets/ |

## What was executed

| Item | Value | Source |
| --- | --- | --- |
| Command | `target/debug/yai prompt < assets/prompt.txt` | transcript.md |
| Exit code | 2 | metrics.json |
| Prompt id | lab-a-orientamento-01 | metrics.json:measurements |
| Resolved prompt payload | `assets/prompt.json` | assets/prompt.json |
| First stderr line | subject:llm-provider has not entered case:new12-filesystem; run `yai case enter` first | assets/command.stderr.txt |

## Metrics table

| Metric | Value | Source |
| --- | --- | --- |
| status | command_recorded | metrics.json:status |
| command_exit_code | 2 | metrics.json:command_exit_code |
| prompt_id | lab-a-orientamento-01 | metrics.json:measurements |
| prompt_kind | yai-prompt | metrics.json:measurements |

## Artifact index

| Path | Class | Present |
| --- | --- | --- |
| assets/C001-command-status.svg | generated figure | yes |
| assets/C002-execution-evidence-counts.svg | generated figure | yes |
| assets/C003-prompt-response-sequence.svg | generated figure | yes |
| assets/command.stderr.txt | log | yes |
| assets/command.stdout.txt | log | yes |
| assets/generated-figures.json | generated figure index | yes |
| assets/generated-tables.md | generated report tables | yes |
| assets/prompt.json | prompt artifact | yes |
| assets/prompt.txt | prompt artifact | yes |

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
- Pack fixture use during run: Not measured

## Interpretation

- The recorded command stopped with exit code 2.
- The first captured stderr line is: `` subject:llm-provider has not entered case:new12-filesystem; run `yai case enter` first ``.
- The run used prompt catalog item `lab-a-orientamento-01` and preserved the resolved prompt payload.
- The filesystem-loop prompt did not reach model response evidence because the case entry precondition was not satisfied.
- No model-quality or benchmark conclusion is drawn from this run.

## Next run

- Enter `subject:llm-provider` into `case:new12-filesystem` before repeating the prompt-catalog run.
- Repeat `run.sh --prompt-id lab-a-orientamento-01` and compare transcript/report evidence.
