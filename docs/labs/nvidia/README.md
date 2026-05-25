# NVIDIA LAN Benchmark Lab

Status: canonical benchmark lab.

Purpose: measure and publish LAN provider readiness, local runtime benchmark
signals and visualization-ready benchmark reports.

## Standard Files

| Concern | Path |
|---|---|
| Purpose and map | `docs/labs/nvidia/README.md` |
| Runbook | `docs/labs/nvidia/runbook.md` |
| Tests | `docs/labs/nvidia/tests.md` |
| Output contract | `docs/labs/nvidia/outputs.md` |
| Lightweight outputs placeholder | `docs/labs/nvidia/outputs/` |
| Curated reports | `docs/labs/nvidia/reports/` |

## Supporting Files

| Concern | Path |
|---|---|
| Benchmark matrix | `docs/labs/nvidia/local-runtime-benchmark-matrix.md` |
| Result template | `docs/labs/nvidia/benchmark-results-template.md` |
| Visual language | `docs/labs/nvidia/benchmark-visual-language.md` |
| Report dependencies | `docs/labs/nvidia/requirements-benchmark-report.txt` |
| Report generator | `tools/labs/generate-benchmark-report.py` |
| Curation config | `configs/labs/nvidia-report-curation.yaml` |
| Legacy runbook link | `docs/labs/nvidia/benchmark-runbook.md` |

## Output Separation

```text
benchmarks/nvidia/                 raw local run folders, ignored by git
configs/labs/nvidia-report-*.yaml  curation and benchmark selection
docs/labs/nvidia/reports/          curated publication artifacts
docs/labs/nvidia/generated-*.md     generated snapshots or one-off lab notes
```

The current live provider port is `43117`. Port `44317` is retained only as
negative evidence in the 2026-05-25 report.
