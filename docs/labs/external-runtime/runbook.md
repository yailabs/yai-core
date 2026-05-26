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
Local provider host: OpenAI-compatible provider, usually on port 43117
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

os.environ.setdefault("YAI_PROVIDER_HOST", "localhost")
os.environ.setdefault("YAI_PROVIDER_PORT", "43117")
os.environ.setdefault("YAI_PROVIDER_MODEL", "qwen-local")
os.environ.setdefault("YAI_EXTERNAL_RUNTIME_OUT", "benchmarks/external-runtime")

print("repo", ROOT)
print("provider", f"{os.environ['YAI_PROVIDER_HOST']}:{os.environ['YAI_PROVIDER_PORT']}")
print("model", os.environ["YAI_PROVIDER_MODEL"])
```

## Prompt Catalog

Provider prompt text lives in `prompts.json`. List available prompts before
running one:

```bash
docs/labs/external-runtime/run.sh --list-prompts
```

## Current Provider Probe

This cell executes the `local-provider-ready` prompt through the lab-local run
script. `run.sh` resolves the prompt from `prompts.json`, invokes
`docs/labs/_shared/bin/probe-local-provider.sh`, and writes the request payload
and provider response under the compact run `assets/` directory.

<!-- yai-prompt-preview:local-provider-ready -->
Prompt `local-provider-ready` (provider-chat)

Local provider reachability smoke prompt.

```text
Rispondi in una frase: provider locale pronto sulla porta 43117.
```

```bash
set -eu

docs/labs/external-runtime/run.sh --slug local-provider-ready --prompt-id local-provider-ready
```

## Optional HNSW Smoke

This is a smoke workload for the measurement path, not a full benchmark. It has no language-model prompt because it is synthetic vector-search data.

```bash
set -eu

OUT="${YAI_EXTERNAL_RUNTIME_OUT:-benchmarks/external-runtime}/hnsw-smoke-$(date +%Y%m%d-%H%M%S)"

python3 docs/labs/_shared/bin/run-hnsw-benchmark.py \
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
