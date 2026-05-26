#!/usr/bin/env python3
"""Generate report-first run evidence tables and figures for canonical labs."""

from __future__ import annotations

import argparse
import json
import os
import re
import sys
import textwrap
from dataclasses import dataclass
from datetime import datetime, timezone
from pathlib import Path
from typing import Any


START_MARKER = "<!-- yai-generated-report:start -->"
END_MARKER = "<!-- yai-generated-report:end -->"


def repo_root() -> Path:
    for parent in Path(__file__).resolve().parents:
        if (parent / ".git").exists() or (parent / "Makefile").exists():
            return parent
    return Path.cwd()


def ensure_scientific_stack() -> tuple[Any, Any, Any]:
    os.environ.setdefault("MPLCONFIGDIR", "/tmp/yai-matplotlib")
    try:
        import numpy as np
        import pandas as pd
        import matplotlib

        matplotlib.use("Agg")
        import matplotlib.pyplot as plt

        return np, pd, plt
    except ModuleNotFoundError as exc:
        root = repo_root()
        venv_python = root / "build" / "lab-figures-venv" / "bin" / "python"
        current_python = Path(sys.executable)
        if (
            venv_python.exists()
            and current_python != venv_python
            and os.environ.get("YAI_LAB_FIGURES_VENV_REEXEC") != "1"
        ):
            os.environ["YAI_LAB_FIGURES_VENV_REEXEC"] = "1"
            os.execv(str(venv_python), [str(venv_python), *sys.argv])
        raise SystemExit(
            f"missing required scientific dependency: {exc.name}. "
            f"Install numpy, pandas and matplotlib, or create {venv_python}."
        ) from exc


np, pd, plt = ensure_scientific_stack()
plt.rcParams.update(
    {
        "font.size": 9,
        "axes.titlesize": 11,
        "axes.labelsize": 9,
        "xtick.labelsize": 8,
        "ytick.labelsize": 8,
        "svg.hashsalt": "yai-lab-report",
    }
)


@dataclass
class FigureRecord:
    figure_id: str
    title: str
    path: Path
    rel_path: str
    source_data: list[str]
    caption: str
    limitation: str
    diagnostic: bool = False


@dataclass
class RunEvidence:
    run_dir: Path
    out_assets: Path
    lab: str
    run_slug: str
    metrics: dict[str, Any]
    manifest: dict[str, Any]
    transcript: str
    report: str
    assets: list[Path]
    generated_at_utc: str


def read_text(path: Path) -> str:
    if not path.exists():
        return ""
    return path.read_text(encoding="utf-8", errors="replace")


def read_json(path: Path, default: Any) -> Any:
    if not path.exists():
        return default
    with path.open(encoding="utf-8") as fh:
        return json.load(fh)


def write_json(path: Path, value: Any) -> None:
    path.write_text(json.dumps(value, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")


def run_relative(run: RunEvidence, path: Path) -> str:
    return path.relative_to(run.run_dir).as_posix()


def asset_relative(run_dir: Path, path: Path) -> str:
    return path.relative_to(run_dir).as_posix()


def parse_key_value_file(path: Path) -> dict[str, str]:
    values: dict[str, str] = {}
    for line in read_text(path).splitlines():
        if "=" not in line:
            continue
        key, value = line.split("=", 1)
        values[key.strip()] = value.strip()
    return values


def numeric(value: Any) -> float | None:
    if value is None:
        return None
    if isinstance(value, (int, float)):
        return float(value)
    if isinstance(value, str):
        try:
            return float(value)
        except ValueError:
            return None
    return None


def integer(value: Any) -> int | None:
    number = numeric(value)
    if number is None:
        return None
    return int(number)


def load_run(run_dir: Path, out_assets: Path | None) -> RunEvidence:
    run_dir = run_dir.resolve()
    out_assets = (out_assets or run_dir / "assets").resolve()
    metrics = read_json(run_dir / "metrics.json", {})
    manifest = read_json(run_dir / "manifest.json", {})
    lab = str(metrics.get("lab") or manifest.get("lab") or run_dir.parent.parent.name)
    run_slug = str(metrics.get("run_slug") or manifest.get("run_slug") or run_dir.name)
    assets = sorted((run_dir / "assets").rglob("*")) if (run_dir / "assets").exists() else []
    return RunEvidence(
        run_dir=run_dir,
        out_assets=out_assets,
        lab=lab,
        run_slug=run_slug,
        metrics=metrics,
        manifest=manifest,
        transcript=read_text(run_dir / "transcript.md"),
        report=read_text(run_dir / "report.md"),
        assets=[path for path in assets if path.is_file()],
        generated_at_utc=datetime.now(timezone.utc).replace(microsecond=0).isoformat().replace("+00:00", "Z"),
    )


def remove_previous_generated(run: RunEvidence) -> None:
    for path in run.out_assets.glob("C[0-9][0-9][0-9]-*.svg"):
        path.unlink()


def classify_asset(path: Path) -> str:
    name = path.name
    suffix = path.suffix.lower()
    if name.startswith("C") and suffix == ".svg":
        return "generated figure"
    if name == "generated-figures.json":
        return "generated figure index"
    if name == "generated-tables.md":
        return "generated report tables"
    if "prompt" in name:
        return "prompt artifact"
    if suffix == ".json":
        return "json data"
    if suffix in {".txt", ".log"}:
        return "log"
    if suffix == ".md":
        return "markdown artifact"
    if suffix == ".svg":
        return "figure"
    return "asset"


def manifest_assets(run: RunEvidence) -> list[str]:
    listed = [str(item) for item in run.manifest.get("assets", []) if isinstance(item, str)]
    actual = [asset_relative(run.run_dir, path) for path in run.assets if path.exists()]
    values = []
    for item in sorted(set(listed + actual)):
        path = run.run_dir / item
        if re.match(r"assets/C[0-9][0-9][0-9]-.*\.svg$", item) and not path.exists():
            continue
        if item in {"assets/generated-figures.json", "assets/generated-tables.md"} and not path.exists():
            continue
        values.append(item)
    return values


def command_exit_code(run: RunEvidence) -> int | None:
    return integer(run.metrics.get("command_exit_code"))


def measurements(run: RunEvidence) -> dict[str, Any]:
    value = run.metrics.get("measurements", {})
    return value if isinstance(value, dict) else {}


def provider_probe_dir(run: RunEvidence) -> Path:
    return run.run_dir / "assets" / "local-provider-probe"


def provider_meta(run: RunEvidence) -> dict[str, Any]:
    return read_json(provider_probe_dir(run) / "meta.json", {})


def provider_chat(run: RunEvidence) -> dict[str, Any]:
    return read_json(provider_probe_dir(run) / "chat.json", {})


def provider_times(run: RunEvidence) -> list[dict[str, Any]]:
    probe = provider_probe_dir(run)
    rows: list[dict[str, Any]] = []
    for label, rel in [
        ("models", "models.curl-meta.txt"),
        ("chat", "chat.curl-meta.txt"),
    ]:
        meta = parse_key_value_file(probe / rel)
        total = numeric(meta.get("time_total"))
        if total is not None:
            rows.append(
                {
                    "endpoint": label,
                    "time_seconds": total,
                    "http_code": meta.get("http_code", "unknown"),
                    "source": f"assets/local-provider-probe/{rel}",
                }
            )
    return rows


def provider_endpoint_rows(run: RunEvidence) -> list[dict[str, Any]]:
    values = measurements(run)
    probe = provider_probe_dir(run)
    rows: list[dict[str, Any]] = []
    for label, key, rel in [
        ("models", "models_http_code", "models.curl-meta.txt"),
        ("chat", "chat_http_code", "chat.curl-meta.txt"),
    ]:
        code = values.get(key)
        source = "metrics.json:measurements"
        if code is None:
            meta = parse_key_value_file(probe / rel)
            code = meta.get("http_code")
            source = f"assets/local-provider-probe/{rel}"
        code_int = integer(code)
        if code_int is None:
            continue
        rows.append(
            {
                "endpoint": label,
                "http_code": code_int,
                "ready": 1 if 200 <= code_int < 300 else 0,
                "source": source,
            }
        )
    return rows


def chat_usage_rows(run: RunEvidence) -> list[tuple[str, str, str]]:
    chat = provider_chat(run)
    rows: list[tuple[str, str, str]] = []
    usage = chat.get("usage")
    if isinstance(usage, dict):
        for key in ["prompt_tokens", "completion_tokens", "total_tokens"]:
            if key in usage:
                rows.append((key, str(display_value(usage[key])), "assets/local-provider-probe/chat.json:usage"))
    timings = chat.get("timings")
    if isinstance(timings, dict):
        for key in ["prompt_ms", "predicted_ms", "prompt_per_second", "predicted_per_second"]:
            if key in timings:
                rows.append((key, str(display_value(timings[key])), "assets/local-provider-probe/chat.json:timings"))
    return rows


def file_present(run: RunEvidence, rel_path: str) -> bool:
    return (run.run_dir / rel_path).exists()


def file_nonempty(run: RunEvidence, rel_path: str) -> bool:
    path = run.run_dir / rel_path
    return path.exists() and path.stat().st_size > 0


def count_error_signals(run: RunEvidence) -> int:
    texts = [
        run.transcript,
        read_text(run.run_dir / "assets" / "command.stderr.txt"),
        read_text(run.run_dir / "assets" / "command.stdout.txt"),
    ]
    count = 0
    for text in texts:
        if not text:
            continue
        count += len(re.findall(r"\b(error|failed|failure|unauthorized|invalid|panic)\b", text, re.IGNORECASE))
    if file_nonempty(run, "assets/command.stderr.txt") and count == 0:
        count = 1
    return count


def has_receipt_evidence(text: str) -> bool:
    return bool(re.search(r"\b(filesystem_receipt|receipt_id|receipt:)\b", text, re.IGNORECASE))


def evidence_counts(run: RunEvidence) -> dict[str, int]:
    prompt_count = 0
    if file_present(run, "assets/prompt.json"):
        prompt_count += 1
    if "- Prompt id:" in run.transcript or '"prompt_id"' in json.dumps(measurements(run)):
        prompt_count = max(prompt_count, 1)

    chat = provider_chat(run)
    response_count = 0
    choices = chat.get("choices")
    if isinstance(choices, list):
        response_count += len(choices)
    elif prompt_count and file_nonempty(run, "assets/command.stdout.txt"):
        response_count += 1

    command_blocks = run.transcript.count("```sh")
    if "## Command Execution" in run.transcript:
        command_blocks = max(command_blocks, 1)

    joined = "\n".join(
        [
            run.transcript,
            read_text(run.run_dir / "assets" / "command.stdout.txt"),
            read_text(run.run_dir / "assets" / "command.stderr.txt"),
        ]
    )

    return {
        "commands": command_blocks,
        "stdout_files": 1 if file_present(run, "assets/command.stdout.txt") else 0,
        "stderr_files": 1 if file_present(run, "assets/command.stderr.txt") else 0,
        "prompts": prompt_count,
        "responses": response_count,
        "receipts": 1 if has_receipt_evidence(joined) else 0,
        "errors": count_error_signals(run),
    }


def provider_artifact_presence(run: RunEvidence) -> list[dict[str, Any]]:
    checks = [
        ("request payload", "assets/local-provider-probe/chat-request.json"),
        ("models response", "assets/local-provider-probe/models.json"),
        ("chat response", "assets/local-provider-probe/chat.json"),
        ("probe summary", "assets/local-provider-probe/summary.md"),
        ("probe metadata", "assets/local-provider-probe/meta.json"),
    ]
    return [
        {
            "artifact": label,
            "path": rel,
            "present": 1 if file_present(run, rel) else 0,
        }
        for label, rel in checks
    ]


def prompt_sequence_events(run: RunEvidence) -> list[tuple[str, str]]:
    events: list[tuple[str, str]] = []
    prompt_id = measurements(run).get("prompt_id")
    if prompt_id:
        events.append(("prompt id", str(prompt_id)))
    if file_present(run, "assets/prompt.json"):
        events.append(("prompt payload", "assets/prompt.json"))
    if file_present(run, "assets/prompt.txt"):
        events.append(("prompt text", "assets/prompt.txt"))
    if "## Command Execution" in run.transcript:
        events.append(("command", "transcript.md"))
    if file_present(run, "assets/local-provider-probe/chat-request.json"):
        events.append(("provider request", "assets/local-provider-probe/chat-request.json"))
    if file_present(run, "assets/local-provider-probe/chat.json"):
        events.append(("provider response", "assets/local-provider-probe/chat.json"))
    elif measurements(run).get("prompt_kind") and file_nonempty(run, "assets/command.stdout.txt"):
        events.append(("command response", "assets/command.stdout.txt"))
    if measurements(run).get("prompt_kind") and file_nonempty(run, "assets/command.stderr.txt"):
        events.append(("command error", "assets/command.stderr.txt"))
    if has_receipt_evidence(run.transcript + read_text(run.run_dir / "assets" / "command.stdout.txt")):
        events.append(("receipt mention", "transcript/stdout"))
    return events


def pack_fixture_table(run: RunEvidence) -> list[list[str]]:
    lab_dir = run.run_dir.parent.parent
    fixture = lab_dir / "pack-fixture"
    if not fixture.exists():
        return [["pack-fixture", "absent", "No pack fixture directory is present for this lab."]]
    rows: list[list[str]] = []
    categories = [
        ("policies", fixture / "policies"),
        ("pack yaml", fixture / "pack.yaml"),
        ("doctrine mapping", fixture / "pack-doctrine-mapping.md"),
    ]
    for label, path in categories:
        if path.is_dir():
            rows.append([label, str(len(list(path.glob("*")))), "Available as lab fixture; run use must be stated by transcript/report."])
        elif path.exists():
            rows.append([label, "1", "Available as lab fixture; run use must be stated by transcript/report."])
        else:
            rows.append([label, "0", "Not present."])
    return rows


def save_bar(
    path: Path,
    labels: list[str],
    values: list[float],
    title: str,
    ylabel: str,
    color: str,
    overwrite: bool,
    annotations: list[str] | None = None,
) -> bool:
    if path.exists() and not overwrite:
        return False
    fig, ax = plt.subplots(figsize=(6.5, 3.4))
    x = np.arange(len(labels))
    ax.bar(x, values, color=color, edgecolor="#1f2937", linewidth=0.6)
    ax.set_title(title)
    ax.set_ylabel(ylabel)
    ax.set_xticks(x)
    ax.set_xticklabels(labels, rotation=18, ha="right")
    upper = max([1.0, *[float(value) for value in values]]) * 1.25
    ax.set_ylim(0, upper)
    ax.grid(axis="y", alpha=0.25, linewidth=0.7)
    for idx, value in enumerate(values):
        label = annotations[idx] if annotations else f"{value:g}"
        ax.text(idx, float(value) + upper * 0.03, label, ha="center", va="bottom", fontsize=8)
    fig.tight_layout()
    fig.savefig(path, format="svg", metadata={"Date": None})
    plt.close(fig)
    return True


def command_status_figure(run: RunEvidence, overwrite: bool) -> FigureRecord | None:
    exit_code = command_exit_code(run)
    if exit_code is None:
        return None
    labels = ["success", "failure"]
    values = [1, 0] if exit_code == 0 else [0, 1]
    path = run.out_assets / "C001-command-status.svg"
    save_bar(
        path,
        labels,
        values,
        "Command status",
        "Observed status (1=yes)",
        "#2f6f8f" if exit_code == 0 else "#a63d40",
        overwrite,
        [str(value) for value in values],
    )
    return FigureRecord(
        figure_id="C001",
        title="Command status",
        path=path,
        rel_path=run_relative(run, path),
        source_data=["metrics.json:command_exit_code"],
        caption=f"The recorded command exit code is {exit_code}.",
        limitation="Diagnostic execution status only; this is not a benchmark or quality measurement.",
        diagnostic=True,
    )


def execution_counts_figure(run: RunEvidence, overwrite: bool) -> FigureRecord | None:
    counts = evidence_counts(run)
    if sum(counts.values()) == 0:
        return None
    labels = list(counts.keys())
    values = [float(value) for value in counts.values()]
    path = run.out_assets / "C002-execution-evidence-counts.svg"
    save_bar(
        path,
        labels,
        values,
        "Execution evidence counts",
        "Count",
        "#426b69",
        overwrite,
        [str(int(value)) for value in values],
    )
    return FigureRecord(
        figure_id="C002",
        title="Execution evidence counts",
        path=path,
        rel_path=run_relative(run, path),
        source_data=["transcript.md", "assets/command.stdout.txt", "assets/command.stderr.txt", "metrics.json"],
        caption="Counts summarize observable evidence channels captured by the run.",
        limitation="Counts are inferred from run files and do not measure correctness, latency or model quality.",
        diagnostic=True,
    )


def prompt_response_sequence_figure(run: RunEvidence, overwrite: bool) -> FigureRecord | None:
    events = prompt_sequence_events(run)
    if len(events) < 3:
        return None
    path = run.out_assets / "C003-prompt-response-sequence.svg"
    if not path.exists() or overwrite:
        fig, ax = plt.subplots(figsize=(7.4, 2.6))
        xs = np.arange(1, len(events) + 1)
        ax.plot(xs, np.ones(len(events)), color="#6b7280", linewidth=1.0, zorder=1)
        ax.scatter(xs, np.ones(len(events)), color="#7b4f2f", s=70, zorder=2)
        for x, (label, detail) in zip(xs, events):
            wrapped = textwrap.fill(label, 13)
            ax.text(x, 1.08, wrapped, ha="center", va="bottom", fontsize=8)
            ax.text(x, 0.90, textwrap.fill(detail, 18), ha="center", va="top", fontsize=7, color="#374151")
        ax.set_title("Prompt execution sequence")
        ax.set_xlabel("Chronological step")
        ax.set_yticks([])
        ax.set_xlim(0.5, len(events) + 0.5)
        ax.set_ylim(0.65, 1.35)
        ax.spines[["left", "right", "top"]].set_visible(False)
        fig.tight_layout()
        fig.savefig(path, format="svg", metadata={"Date": None})
        plt.close(fig)
    return FigureRecord(
        figure_id="C003",
        title="Prompt execution sequence",
        path=path,
        rel_path=run_relative(run, path),
        source_data=[
            "transcript.md",
            "assets/prompt.json",
            "assets/prompt.txt",
            "assets/command.stdout.txt",
            "assets/command.stderr.txt",
        ],
        caption="The sequence shows prompt/request evidence and the captured command outcome.",
        limitation="The sequence is categorical and chronological; it does not score response quality.",
        diagnostic=False,
    )


def provider_endpoint_status_figure(run: RunEvidence, overwrite: bool) -> FigureRecord | None:
    rows = provider_endpoint_rows(run)
    if not rows:
        return None
    labels = [row["endpoint"] for row in rows]
    values = [float(row["ready"]) for row in rows]
    annotations = [str(row["http_code"]) for row in rows]
    path = run.out_assets / "C002-provider-endpoint-status.svg"
    save_bar(
        path,
        labels,
        values,
        "Provider endpoint status",
        "2xx status observed (1=yes)",
        "#3a6f43",
        overwrite,
        annotations,
    )
    return FigureRecord(
        figure_id="C002",
        title="Provider endpoint status",
        path=path,
        rel_path=run_relative(run, path),
        source_data=sorted({row["source"] for row in rows}),
        caption=", ".join(f"{row['endpoint']} HTTP {row['http_code']}" for row in rows),
        limitation="Endpoint reachability is provider evidence, not provider quality or model quality.",
        diagnostic=False,
    )


def provider_artifact_presence_figure(run: RunEvidence, overwrite: bool) -> FigureRecord | None:
    rows = provider_artifact_presence(run)
    if not any(row["present"] for row in rows):
        return None
    labels = [row["artifact"] for row in rows]
    values = [float(row["present"]) for row in rows]
    path = run.out_assets / "C003-response-artifact-presence.svg"
    save_bar(
        path,
        labels,
        values,
        "Provider artifact availability",
        "Present (1=yes)",
        "#4d6f9f",
        overwrite,
        [str(int(value)) for value in values],
    )
    present = sum(row["present"] for row in rows)
    return FigureRecord(
        figure_id="C003",
        title="Provider response artifact availability",
        path=path,
        rel_path=run_relative(run, path),
        source_data=["manifest.json:assets", "assets/local-provider-probe/"],
        caption=f"{present} of {len(rows)} checked provider artifacts are present.",
        limitation="Diagnostic presence chart only; prefer the artifact index table for exact paths.",
        diagnostic=True,
    )


def provider_response_time_figure(run: RunEvidence, overwrite: bool) -> FigureRecord | None:
    rows = provider_times(run)
    if not rows:
        return None
    labels = [row["endpoint"] for row in rows]
    values = [float(row["time_seconds"]) for row in rows]
    path = run.out_assets / "C004-provider-response-time.svg"
    save_bar(
        path,
        labels,
        values,
        "Provider curl total time",
        "Seconds",
        "#7a5c2e",
        overwrite,
        [f"{value:.6f}" for value in values],
    )
    return FigureRecord(
        figure_id="C004",
        title="Provider response time",
        path=path,
        rel_path=run_relative(run, path),
        source_data=[row["source"] for row in rows],
        caption=", ".join(f"{row['endpoint']} {row['time_seconds']:.6f}s" for row in rows),
        limitation="Curl total time is a probe observation, not a throughput or hardware benchmark.",
        diagnostic=False,
    )


def generate_figures(run: RunEvidence, overwrite: bool) -> list[FigureRecord]:
    if overwrite:
        remove_previous_generated(run)
    run.out_assets.mkdir(parents=True, exist_ok=True)

    figures: list[FigureRecord] = []
    status = command_status_figure(run, overwrite)
    if status:
        figures.append(status)

    if run.lab == "external-runtime":
        for factory in [
            provider_endpoint_status_figure,
            provider_artifact_presence_figure,
            provider_response_time_figure,
        ]:
            record = factory(run, overwrite)
            if record:
                figures.append(record)
    elif run.lab == "filesystem-loop":
        counts = execution_counts_figure(run, overwrite)
        if counts:
            figures.append(counts)
        sequence = prompt_response_sequence_figure(run, overwrite)
        if sequence:
            figures.append(sequence)

    return figures


def md_escape(value: Any) -> str:
    text = str(value)
    text = text.replace("|", "\\|").replace("\n", "<br>")
    return text


def display_value(value: Any) -> Any:
    if isinstance(value, float):
        return f"{value:.6g}"
    return value


def inline_code(value: Any) -> str:
    text = str(value)
    if "`" in text:
        return f"`` {text} ``"
    return f"`{text}`"


def markdown_table(headers: list[str], rows: list[list[Any]]) -> str:
    if not rows:
        return "Not measured\n"
    lines = [
        "| " + " | ".join(md_escape(header) for header in headers) + " |",
        "| " + " | ".join("---" for _ in headers) + " |",
    ]
    for row in rows:
        lines.append("| " + " | ".join(md_escape(cell) for cell in row) + " |")
    return "\n".join(lines) + "\n"


def evidence_summary_rows(run: RunEvidence) -> list[list[Any]]:
    rows: list[list[Any]] = []
    exit_code = command_exit_code(run)
    rows.append(["Lab", run.lab, "metrics.json / manifest.json"])
    rows.append(["Run slug", run.run_slug, "metrics.json / manifest.json"])
    rows.append(["Command exit code", "Not measured" if exit_code is None else exit_code, "metrics.json:command_exit_code"])
    counts = evidence_counts(run)
    for key, value in counts.items():
        rows.append([key.replace("_", " "), value, "transcript.md + assets/"])
    if run.lab == "external-runtime":
        for row in provider_endpoint_rows(run):
            rows.append([f"{row['endpoint']} endpoint HTTP code", row["http_code"], row["source"]])
    return rows


def metrics_table_rows(run: RunEvidence) -> list[list[Any]]:
    rows: list[list[Any]] = []
    rows.append(["status", run.metrics.get("status", "Not measured"), "metrics.json:status"])
    rows.append(["command_exit_code", run.metrics.get("command_exit_code", "Not measured"), "metrics.json:command_exit_code"])
    for key, value in measurements(run).items():
        rows.append([key, value, "metrics.json:measurements"])
    for key, value, source in chat_usage_rows(run):
        rows.append([key, value, source])
    if not rows:
        return [["Not measured", "Not measured", "No metrics.json values were available."]]
    return rows


def artifact_index_rows(run: RunEvidence, figures: list[FigureRecord]) -> list[list[Any]]:
    rows: list[list[Any]] = []
    listed = set(manifest_assets(run))
    for rel in sorted(listed):
        path = run.run_dir / rel
        rows.append([rel, classify_asset(path), "yes" if path.exists() else "listed only"])
    for figure in figures:
        if figure.rel_path not in listed:
            rows.append([figure.rel_path, "generated figure", "yes"])
    generated_tables = run_relative(run, run.out_assets / "generated-tables.md")
    if generated_tables not in listed:
        rows.append([generated_tables, "generated report tables", "yes"])
    generated_index = run_relative(run, run.out_assets / "generated-figures.json")
    if generated_index not in listed:
        rows.append([generated_index, "generated figure index", "yes"])
    return rows


def missing_measurements(run: RunEvidence) -> list[str]:
    missing: list[str] = []
    if command_exit_code(run) is None:
        missing.append("Command exit code: Not measured")
    if "duration" not in json.dumps(run.metrics).lower() and not provider_times(run):
        missing.append("Command duration or endpoint timing: Not measured")
    if not measurements(run).get("prompt_id") and not file_present(run, "assets/prompt.json"):
        missing.append("Prompt ID and resolved prompt payload: Not measured")
    if run.lab == "filesystem-loop":
        counts = evidence_counts(run)
        if counts["responses"] == 0:
            missing.append("Model or command response sequence: Not measured")
        if counts["receipts"] == 0:
            missing.append("Filesystem receipt evidence: Not measured")
        if "pack-fixture" not in run.transcript and "pack fixture" not in run.transcript.lower():
            missing.append("Pack fixture use during run: Not measured")
    if run.lab == "external-runtime":
        if not provider_endpoint_rows(run):
            missing.append("Provider endpoint HTTP status: Not measured")
        if not provider_times(run):
            missing.append("Provider endpoint timing: Not measured")
        if not chat_usage_rows(run):
            missing.append("Provider token/timing usage fields: Not measured")
    missing.append("Benchmark throughput, hardware, VRAM and model-quality measurements: Not measured")
    return sorted(set(missing))


def executed_command(run: RunEvidence) -> str:
    match = re.search(r"## Command Execution\s+```sh\n(.*?)\n```", run.transcript, re.DOTALL)
    if not match:
        return "Not recorded"
    return match.group(1).strip()


def first_stderr_line(run: RunEvidence) -> str:
    for line in read_text(run.run_dir / "assets" / "command.stderr.txt").splitlines():
        stripped = line.strip()
        if stripped:
            return stripped
    return "Not recorded"


def what_was_executed_rows(run: RunEvidence) -> list[list[Any]]:
    values = measurements(run)
    rows = [
        ["Command", f"`{executed_command(run)}`", "transcript.md"],
        ["Exit code", "Not measured" if command_exit_code(run) is None else command_exit_code(run), "metrics.json"],
    ]
    if values.get("prompt_id"):
        rows.append(["Prompt id", values["prompt_id"], "metrics.json:measurements"])
    if file_present(run, "assets/prompt.json"):
        rows.append(["Resolved prompt payload", "`assets/prompt.json`", "assets/prompt.json"])
    if file_nonempty(run, "assets/command.stderr.txt"):
        rows.append(["First stderr line", first_stderr_line(run), "assets/command.stderr.txt"])
    return rows


def interpretation(run: RunEvidence) -> list[str]:
    exit_code = command_exit_code(run)
    values = measurements(run)
    lines: list[str] = []
    if exit_code == 0:
        lines.append("The recorded command completed successfully for this run.")
    elif exit_code is not None:
        lines.append(f"The recorded command stopped with exit code {exit_code}.")
        stderr_line = first_stderr_line(run)
        if stderr_line != "Not recorded":
            lines.append(f"The first captured stderr line is: {inline_code(stderr_line)}.")
    else:
        lines.append("No command exit code was available in `metrics.json`.")

    if run.lab == "external-runtime":
        endpoint_rows = provider_endpoint_rows(run)
        if endpoint_rows:
            endpoint_text = ", ".join(f"{row['endpoint']} HTTP {row['http_code']}" for row in endpoint_rows)
            lines.append(f"The provider probe recorded endpoint status evidence: {endpoint_text}.")
        if chat_usage_rows(run):
            lines.append("The provider response includes token/timing fields that can support future measurement reports.")
    if run.lab == "filesystem-loop":
        if values.get("prompt_id"):
            lines.append(f"The run used prompt catalog item `{values['prompt_id']}` and preserved the resolved prompt payload.")
        if "case enter" in first_stderr_line(run):
            lines.append("The filesystem-loop prompt did not reach model response evidence because the case entry precondition was not satisfied.")
    lines.append("No model-quality or benchmark conclusion is drawn from this run.")
    return lines


def next_run(run: RunEvidence) -> list[str]:
    if run.lab == "filesystem-loop":
        if "case enter" in first_stderr_line(run):
            return [
                "Enter `subject:llm-provider` into `case:new12-filesystem` before repeating the prompt-catalog run.",
                "Repeat `run.sh --prompt-id lab-a-orientamento-01` and compare transcript/report evidence.",
            ]
        return [
            "Use a prompt-catalog run when prompt/response evidence is needed.",
            "Record filesystem receipt evidence when mutative operations are intentionally exercised.",
        ]
    if run.lab == "external-runtime":
        return [
            "Repeat the provider probe only when endpoint state changes.",
            "Keep any throughput or model-quality evaluation in a separate benchmark run with explicit measurements.",
        ]
    return ["Repeat the run with explicit metrics for any missing measurement."]


def limitations(run: RunEvidence, figures: list[FigureRecord]) -> list[str]:
    values = [
        "Generated evidence is derived only from existing run metrics and assets.",
        "The report makes no model-quality or benchmark superiority claim.",
    ]
    if any(figure.diagnostic for figure in figures):
        values.append("Diagnostic figures show evidence availability or status; they do not measure quality.")
    if run.lab == "filesystem-loop":
        values.append("Filesystem-loop interpretation is limited to captured prompt, command, output and receipt evidence.")
    if run.lab == "external-runtime":
        values.append("Provider probe observations describe endpoint behavior for this run only.")
    return values


def write_generated_tables(run: RunEvidence, figures: list[FigureRecord]) -> Path:
    path = run.out_assets / "generated-tables.md"
    content = [
        "# Generated Run Tables",
        "",
        "## Run evidence summary",
        "",
        markdown_table(["Field", "Value", "Source"], evidence_summary_rows(run)).rstrip(),
        "",
        "## What was executed",
        "",
        markdown_table(["Item", "Value", "Source"], what_was_executed_rows(run)).rstrip(),
        "",
        "## Metrics table",
        "",
        markdown_table(["Metric", "Value", "Source"], metrics_table_rows(run)).rstrip(),
        "",
        "## Artifact index",
        "",
        markdown_table(["Path", "Class", "Present"], artifact_index_rows(run, figures)).rstrip(),
        "",
        "## Pack fixture table",
        "",
        markdown_table(["Fixture category", "Count", "Note"], pack_fixture_table(run)).rstrip(),
        "",
        "## Missing measurements",
        "",
    ]
    missing = missing_measurements(run)
    if missing:
        content.extend(f"- {item}" for item in missing)
    else:
        content.append("- None recorded.")
    content.extend(["", "## Interpretation", ""])
    content.extend(f"- {item}" for item in interpretation(run))
    content.extend(["", "## Next run", ""])
    content.extend(f"- {item}" for item in next_run(run))
    content.append("")
    path.write_text("\n".join(content), encoding="utf-8")
    return path


def figure_index(run: RunEvidence, figures: list[FigureRecord], tables_path: Path) -> dict[str, Any]:
    return {
        "schema": "yai.run.generated-evidence.v1",
        "source_run": run.run_slug,
        "lab": run.lab,
        "generated_at_utc": run.generated_at_utc,
        "source_files": [
            "transcript.md",
            "manifest.json",
            "metrics.json",
            "assets/",
        ],
        "tables": run_relative(run, tables_path),
        "figures": [
            {
                "id": figure.figure_id,
                "type": "figure",
                "title": figure.title,
                "source_run": run.run_slug,
                "source_data": figure.source_data,
                "generation_method": "labs/shared/bin/generate-run-figures.py",
                "output_path": figure.rel_path,
                "format": "svg",
                "created_at": run.generated_at_utc,
                "caption": figure.caption,
                "limitation": figure.limitation,
                "diagnostic": figure.diagnostic,
            }
            for figure in figures
        ],
        "missing_measurements": missing_measurements(run),
    }


def update_manifest(run: RunEvidence, figures: list[FigureRecord], tables_path: Path, index_path: Path) -> None:
    manifest = dict(run.manifest)
    old_assets = [str(item) for item in manifest.get("assets", []) if isinstance(item, str)]
    kept = [
        item
        for item in old_assets
        if not re.match(r"assets/C[0-9][0-9][0-9]-.*\.svg$", item)
        and item not in {"assets/generated-figures.json", "assets/generated-tables.md"}
    ]
    generated = [figure.rel_path for figure in figures]
    generated.extend([run_relative(run, tables_path), run_relative(run, index_path)])
    actual = [asset_relative(run.run_dir, path) for path in run.assets if path.exists()]
    actual = [
        item
        for item in actual
        if not re.match(r"assets/C[0-9][0-9][0-9]-.*\.svg$", item)
        and item not in {"assets/generated-figures.json", "assets/generated-tables.md"}
    ]
    manifest["assets"] = sorted(set(kept + actual + generated))
    manifest["generated_evidence"] = {
        "schema": "yai.run.generated-evidence.v1",
        "figures_index": run_relative(run, index_path),
        "tables": run_relative(run, tables_path),
        "figures": [figure.rel_path for figure in figures],
    }
    write_json(run.run_dir / "manifest.json", manifest)


def report_generated_section(run: RunEvidence, figures: list[FigureRecord], tables_path: Path) -> str:
    parts: list[str] = [
        START_MARKER,
        "## Run evidence summary",
        "",
        "This generated section is composed from existing run files only: `transcript.md`, `metrics.json`, `manifest.json` and `assets/`.",
        "",
        markdown_table(["Field", "Value", "Source"], evidence_summary_rows(run)).rstrip(),
        "",
        "## What was executed",
        "",
        markdown_table(["Item", "Value", "Source"], what_was_executed_rows(run)).rstrip(),
        "",
        "## Metrics table",
        "",
        markdown_table(["Metric", "Value", "Source"], metrics_table_rows(run)).rstrip(),
        "",
        "## Generated figures",
        "",
    ]
    if figures:
        for figure in figures:
            diagnostic = " Diagnostic figure." if figure.diagnostic else ""
            parts.extend(
                [
                    f"### {figure.figure_id} - {figure.title}",
                    "",
                    f"![{figure.title}]({figure.rel_path})",
                    "",
                    f"Caption: {figure.caption}{diagnostic}",
                    "",
                    "Source data: " + ", ".join(f"`{source}`" for source in figure.source_data),
                    "",
                    f"Limitation: {figure.limitation}",
                    "",
                ]
            )
    else:
        parts.extend(["Not measured. No SVG figure was generated because the run does not contain enough source data.", ""])
    parts.extend(
        [
            "## Artifact index",
            "",
            markdown_table(["Path", "Class", "Present"], artifact_index_rows(run, figures)).rstrip(),
            "",
            "## Missing measurements",
            "",
        ]
    )
    missing = missing_measurements(run)
    if missing:
        parts.extend(f"- {item}" for item in missing)
    else:
        parts.append("- None recorded.")
    parts.extend(
        [
            "",
            "## Interpretation",
            "",
        ]
    )
    parts.extend(f"- {item}" for item in interpretation(run))
    parts.extend(
        [
            "",
            "## Limitations",
            "",
        ]
    )
    parts.extend(f"- {item}" for item in limitations(run, figures))
    parts.extend(
        [
            "",
            "## Next run",
            "",
        ]
    )
    parts.extend(f"- {item}" for item in next_run(run))
    parts.extend(
        [
            "",
            f"Generated table attachment: `{run_relative(run, tables_path)}`",
            END_MARKER,
            "",
        ]
    )
    return "\n".join(parts)


def update_report(run: RunEvidence, figures: list[FigureRecord], tables_path: Path) -> None:
    report_path = run.run_dir / "report.md"
    text = read_text(report_path).rstrip()
    section = report_generated_section(run, figures, tables_path).rstrip()
    if START_MARKER in text and END_MARKER in text:
        pattern = re.compile(re.escape(START_MARKER) + r".*?" + re.escape(END_MARKER), re.DOTALL)
        updated = pattern.sub(section, text)
    elif "\n## Generated Figures\n" in text:
        updated = text.split("\n## Generated Figures\n", 1)[0].rstrip() + "\n\n" + section
    else:
        updated = text + "\n\n" + section
    report_path.write_text(updated.rstrip() + "\n", encoding="utf-8")


def run(args: argparse.Namespace) -> int:
    run_evidence = load_run(args.run_dir, args.out_assets)
    figures = generate_figures(run_evidence, args.overwrite)
    tables_path = write_generated_tables(run_evidence, figures)
    index_path = run_evidence.out_assets / "generated-figures.json"
    write_json(index_path, figure_index(run_evidence, figures, tables_path))
    update_manifest(run_evidence, figures, tables_path, index_path)
    if args.update_report:
        update_report(run_evidence, figures, tables_path)
    print(f"generated {len(figures)} figure(s) for {run_evidence.run_slug}")
    for figure in figures:
        print(f"- {figure.rel_path}")
    print(f"- {run_relative(run_evidence, tables_path)}")
    print(f"- {run_relative(run_evidence, index_path)}")
    return 0


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate report-first tables and SVG evidence figures for a canonical lab run."
    )
    parser.add_argument("--run-dir", required=True, type=Path, help="Path to a canonical run directory.")
    parser.add_argument(
        "--out-assets",
        type=Path,
        default=None,
        help="Output assets directory. Defaults to <run-dir>/assets.",
    )
    parser.add_argument("--update-report", action="store_true", help="Insert or replace generated report section.")
    parser.add_argument("--overwrite", action="store_true", help="Overwrite generated SVG evidence files.")
    return parser.parse_args(argv)


if __name__ == "__main__":
    raise SystemExit(run(parse_args(sys.argv[1:])))
