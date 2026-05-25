# NVIDIA LAN Benchmark Runbook

Status: local lab material.

Purpose: prepare a Linux NVIDIA provider host on the LAN while YAI stays on the
MacBook operator host. These steps produce local benchmark evidence only. They
do not make provider execution part of the canonical YAI runtime surface.

## Shape

```text
MacBook operator host
  YAI repo
  docs, scripts, summary
  optional curl probe

Linux LAN provider host
  NVIDIA GPU
  provider process
  OpenAI-compatible endpoint, if available
```

YAI treats model output as provider-boundary observation. Provider output is
candidate material, not authority over decisions, receipts, records or durable
case state.

## One-Time Repo Setup

From the repo root:

```bash
chmod +x tools/labs/*.sh tools/labs/*.py
```

Benchmark outputs are written under `benchmarks/nvidia/` and ignored by git.

## Linux Provider Host Baseline

Run this on the Linux NVIDIA host:

```bash
tools/labs/collect-gpu-baseline.sh benchmarks/nvidia/baseline-$(date +%Y%m%d-%H%M%S)
```

If the Linux host does not have the YAI repo checked out, copy the `tools/labs`
directory there or run equivalent commands manually and keep the output folder.

## Provider Launch

Launch any OpenAI-compatible provider manually on Linux. For llama.cpp, the
shape is commonly:

```bash
llama-server \
  -m /path/to/model.gguf \
  --host 0.0.0.0 \
  --port 43117
```

Then check from the MacBook:

```bash
curl http://<linux-lan-ip>:43117/v1/models
```

Use the exact base URL exposed by the provider. Do not assume every provider
implements all OpenAI-compatible endpoints.

## Model Benchmark

Run on the provider host when measuring local wall time and GPU sampling:

```bash
tools/labs/run-local-model-benchmark.sh \
  --model qwen3-8b-q4 \
  --workload w3-long-report \
  --out benchmarks/nvidia/qwen3-8b-w3 \
  -- \
  llama-cli -m /path/to/model.gguf -p "Scrivi un report tecnico breve su YAI." -n 512
```

The script captures:

- command line;
- stdout and stderr;
- wall time;
- estimated output token count;
- `nvidia-smi` samples, when available;
- peak VRAM observed in samples.

If the backend prints real token counts or tok/s, add those values to
`benchmark-results-template.md`.

## LAN Provider Probe

When the provider is already running on the LAN:

```bash
tools/labs/probe-lan-provider.sh \
  --host 172.20.10.3 \
  --port 43117 \
  --model qwen-local \
  --out benchmarks/nvidia/lan-provider-$(date +%Y%m%d-%H%M%S)
```

This records `/v1/models`, a non-streaming `/v1/chat/completions` call, HTTP
codes, curl timings and a formatted `summary.md`.

## HNSW Synthetic Benchmark

Run where Python and NumPy are available:

```bash
python3 tools/labs/run-hnsw-benchmark.py \
  --n 20000 \
  --dim 768 \
  --queries 100 \
  --k 10 \
  --m 16 \
  --ef-construction 100 \
  --ef-search 64 \
  --out benchmarks/nvidia/hnsw-synthetic
```

The script uses `hnswlib` when installed. If it is missing, it falls back to a
NumPy brute-force baseline and records that method in `summary.json`.

## Summary

For a quick raw table after collecting runs:

```bash
python3 tools/labs/summarize-benchmarks.py \
  --root benchmarks/nvidia \
  --out docs/labs/nvidia/generated-benchmark-summary.md
```

This is only a raw helper. It is not the publishing path.

## Publishable Report

Generate the curated report layer after selecting the run folders in
`configs/labs/nvidia-report-curation.yaml`:

```bash
python3 tools/labs/generate-benchmark-report.py \
  --root benchmarks/nvidia \
  --curation configs/labs/nvidia-report-curation.yaml \
  --out docs/labs/nvidia/reports/20260525-lan-provider
```

The report generator writes:

```text
index.md
data/normalized-runs.json
assets/endpoint-latency.svg
assets/endpoint-status-matrix.svg
assets/hnsw-query-latency.svg
assets/provider-topology-3d.html
notebook.ipynb
```

Install optional report-layer dependencies when you want Plotly/Pandas notebook
work locally:

```bash
python3 -m pip install -r docs/labs/nvidia/requirements-benchmark-report.txt
```

Do not commit raw benchmark outputs unless a future qualification package
explicitly asks for a curated evidence subset. Curated report directories under
`docs/labs/nvidia/reports/` may be committed as publication artifacts.
