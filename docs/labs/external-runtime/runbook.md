# External Runtime Runbook

Status: static runbook copy of `notebook.ipynb`.

The notebook is the executable source. This runbook preserves the same order, prompts and commands for terminal use.

# External Runtime Notebook

Status: canonical lab notebook.

This notebook follows the same lab-root notebook/runbook pattern used by `filesystem-loop`: configure environment once, run explicit probes, preserve prompt payloads, and read compact run metrics from `runs/*/metrics.json`.

## Execution Layout

Run the cells from top to bottom only when you intentionally execute a provider or smoke workload. Existing run summaries can be loaded without executing probes.

```text
Operator machine: this notebook, probe scripts, report packaging
LAN provider host: OpenAI-compatible provider, usually on port 43117
Run folders: docs/labs/external-runtime/runs/YYYYMMDD-<slug>/
```

## Provider Boundary

The current live-provider port is `43117` unless a future run proves otherwise. Port `44317` is negative evidence only and should not be used as the live path.

Provider prompts are benchmark inputs. A prompted provider run is incomplete unless the request payload or prompt artifact is stored under that run `assets/` directory.

## Notebook Environment

Run this once before operational cells. It finds the repository root, loads `.yai/env` if present, and sets default provider values without writing secrets to notebook output.

```python
from pathlib import Path
import json
import os


def find_repo_root(start: Path) -> Path:
    for candidate in [start, *start.parents]:
        if (candidate / "cmd/yai/Cargo.toml").is_file() and (candidate / "docs/labs/external-runtime").is_dir():
            return candidate
    raise RuntimeError("repo root not found")

ROOT = find_repo_root(Path.cwd().resolve())
os.chdir(ROOT)

env_file = ROOT / ".yai/env"
if env_file.exists():
    for line in env_file.read_text().splitlines():
        line = line.strip()
        if not line or line.startswith("#") or "=" not in line:
            continue
        key, value = line.split("=", 1)
        value = value.strip().strip('"').strip("'")
        os.environ.setdefault(key.strip(), value)

os.environ.setdefault("YAI_PROVIDER_HOST", "172.20.10.3")
os.environ.setdefault("YAI_PROVIDER_PORT", "43117")
os.environ.setdefault("YAI_PROVIDER_MODEL", "qwen-local")
os.environ.setdefault("YAI_PROVIDER_PROMPT", "Rispondi in una frase: provider LAN pronto sulla porta 43117.")
os.environ.setdefault("YAI_EXTERNAL_RUNTIME_OUT", "benchmarks/external-runtime")

print("repo", ROOT)
print("provider", f"{os.environ['YAI_PROVIDER_HOST']}:{os.environ['YAI_PROVIDER_PORT']}")
print("model", os.environ["YAI_PROVIDER_MODEL"])
print("prompt", os.environ["YAI_PROVIDER_PROMPT"])
```

## Provider Probe Prompt

The provider readiness probe uses this prompt unless a run overrides `YAI_PROVIDER_PROMPT`:

```text
Rispondi in una frase: provider LAN pronto sulla porta 43117.
```

## Current Provider Probe

This cell executes the LAN provider probe and writes raw probe output under `benchmarks/external-runtime/`. Convert the output into a compact run folder only after reviewing secrets and provenance.

```bash
set -eu

OUT="${YAI_EXTERNAL_RUNTIME_OUT:-benchmarks/external-runtime}/lan-provider-$(date +%Y%m%d-%H%M%S)"

tools/labs/probe-lan-provider.sh \
  --host "${YAI_PROVIDER_HOST:-172.20.10.3}" \
  --port "${YAI_PROVIDER_PORT:-43117}" \
  --model "${YAI_PROVIDER_MODEL:-qwen-local}" \
  --prompt "${YAI_PROVIDER_PROMPT:-Rispondi in una frase: provider LAN pronto sulla porta 43117.}" \
  --out "$OUT"

printf 'probe_out=%s\n' "$OUT"
```

## Optional HNSW Smoke

This is a smoke workload for the measurement path, not a full benchmark. It has no language-model prompt because it is synthetic vector-search data.

```bash
set -eu

OUT="${YAI_EXTERNAL_RUNTIME_OUT:-benchmarks/external-runtime}/hnsw-smoke-$(date +%Y%m%d-%H%M%S)"

python3 tools/labs/run-hnsw-benchmark.py \
  --n 1000 \
  --dim 64 \
  --queries 20 \
  --k 10 \
  --m 16 \
  --ef-construction 100 \
  --ef-search 64 \
  --out "$OUT"

printf 'hnsw_out=%s\n' "$OUT"
```

## Existing Run Metrics

This cell does not execute probes. It loads compact run metrics from `runs/*/metrics.json` for local review.

```python
from pathlib import Path
import json

run_root = Path("docs/labs/external-runtime/runs")
metrics = []
for path in sorted(run_root.glob("*/metrics.json")):
    item = json.loads(path.read_text())
    item["metrics_path"] = str(path)
    metrics.append(item)

metrics
```

## Compact Run Output

New external-runtime runs must be represented as:

```text
docs/labs/external-runtime/runs/YYYYMMDD-<slug>/
  transcript.md
  report.md
  manifest.json
  metrics.json
  assets/
```

`report.md` is the human analytical artifact for that run. `assets/` holds logs, prompt payloads, SVG, HTML, JSON and other attachments. Do not place `notebook.ipynb` inside run assets.

## Pack Fixture

No `pack-fixture/` is currently defined for this lab. If a future test needs
imported operational material, create it using
`docs/lab-standards/pack-fixture-contract.md`.
