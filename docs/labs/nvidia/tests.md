# NVIDIA LAN Benchmark Tests

Status: canonical lab test expectation map.

## Repository Checks

```bash
make check-docs
```

## Provider Reachability

| Area | Expected signal |
|---|---|
| Provider `/v1/models` | HTTP success or recorded failure evidence. |
| Chat completion probe | HTTP status, curl timing and response snippet captured in raw run folder. |
| Negative port evidence | Failed probe remains visible and is not hidden. |
| Curation config | Selected run folders are explicit in `configs/labs/nvidia-report-curation.yaml`. |

## Report Generation

```bash
python3 tools/labs/generate-benchmark-report.py \
  --root benchmarks/nvidia \
  --curation configs/labs/nvidia-report-curation.yaml \
  --out docs/labs/nvidia/reports/20260525-lan-provider
```

Expected curated outputs:

```text
index.md
data/normalized-runs.json
assets/*.svg
assets/provider-topology-3d.html
notebook.ipynb
```

## Boundary Assertions

- Provider output is benchmark evidence, not YAI runtime authority.
- Raw benchmark folders remain under ignored `benchmarks/nvidia/`.
- Curated report assets under `docs/labs/nvidia/reports/` may be committed.
- Secrets, API keys and authorization headers must not appear in reports.
