# NVIDIA LAN Benchmark Outputs

Status: canonical output contract for the NVIDIA lab.

## Output Roots

```text
benchmarks/nvidia/                 raw local run folders, ignored by git
configs/labs/nvidia-report-*.yaml  curation and benchmark selection
docs/labs/nvidia/outputs/          lightweight placeholders or small notes
docs/labs/nvidia/reports/          curated publication artifacts
docs/labs/nvidia/generated-*.md     generated snapshots or one-off lab notes
```

## Raw Runs

Raw runs may include command lines, stdout/stderr, GPU samples, curl timing and
summary JSON. They stay out of git unless a future qualification package defines
a curated evidence subset.

## Curated Reports

Curated reports under `reports/` are the publishable layer. They must explain
which raw runs were selected and preserve failed probes as evidence.

## Templates and Supporting Docs

| Concern | Path |
|---|---|
| Result template | `docs/labs/nvidia/benchmark-results-template.md` |
| Benchmark matrix | `docs/labs/nvidia/local-runtime-benchmark-matrix.md` |
| Visual language | `docs/labs/nvidia/benchmark-visual-language.md` |
| Report dependencies | `docs/labs/nvidia/requirements-benchmark-report.txt` |
