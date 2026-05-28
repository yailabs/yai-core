#!/usr/bin/env python3
"""Render the LAB.TEST.2 matrix report from structured benchmark artifacts."""
from __future__ import annotations

import argparse
import json
from pathlib import Path


CONDITION_LABELS = {
    "C0_naked_no_context": "C0 no context",
    "C1_naked_raw_context": "C1 raw context",
    "C2_naked_mini_rag": "C2 mini-RAG",
    "C3_naked_yai_projection_dump": "C3 projection dump",
    "C4_yai_case_bound": "C4 YAI case",
    "C5_yai_case_residency": "C5 YAI residency",
}


def read_json(path: Path):
    return json.loads(path.read_text(encoding="utf-8"))


def cell(value) -> str:
    if value is None:
        return ""
    if isinstance(value, float):
        return f"{value:.4f}".rstrip("0").rstrip(".")
    return str(value)


def yes_no(value: bool) -> str:
    return "yes" if value else "no"


def table(headers, rows) -> str:
    lines = ["| " + " | ".join(headers) + " |", "| " + " | ".join("---" for _ in headers) + " |"]
    for row in rows:
        lines.append("| " + " | ".join(cell(item).replace("\n", " ") for item in row) + " |")
    return "\n".join(lines)


def bar(value: int, max_value: int, width: int = 28) -> str:
    if max_value <= 0:
        return ""
    filled = int(round((value / max_value) * width))
    filled = max(0, min(width, filled))
    return "#" * filled + "." * (width - filled)


def percent(numerator: int, denominator: int):
    if denominator <= 0:
        return None
    return round((numerator / denominator) * 100, 2)


def status_icon(status: str) -> str:
    if status == "completed":
        return "ok"
    if status == "skipped":
        return "skipped"
    return status or "unknown"


def condition_rows(conditions: dict) -> list[list[object]]:
    rows = []
    for cid, item in conditions.items():
        rows.append([
            cid,
            status_icon(item.get("status")),
            item.get("provider_mode"),
            yes_no(bool(item.get("yai_cli_used"))),
            yes_no(bool(item.get("direct_provider_used"))),
            item.get("questions_answered"),
            item.get("questions_skipped"),
            item.get("input_tokens_total"),
            item.get("output_tokens_total"),
            item.get("latency_total_ms"),
        ])
    return rows


def score_rows(conditions: dict) -> list[list[object]]:
    rows = []
    for cid, item in conditions.items():
        rows.append([
            cid,
            item.get("case_knowledge_avg"),
            item.get("boundary_score_avg"),
            item.get("grounding_score_avg"),
            item.get("operational_safety_score"),
        ])
    return rows


def efficiency_rows(conditions: dict) -> list[list[object]]:
    raw_tokens = conditions.get("C1_naked_raw_context", {}).get("input_tokens_total") or 0
    rows = []
    for cid, item in conditions.items():
        tokens = item.get("input_tokens_total") or 0
        if cid == "C0_naked_no_context" or item.get("status") == "skipped":
            savings = "not_applicable"
            savings_pct = "not_applicable"
        else:
            savings = raw_tokens - tokens if raw_tokens else None
            savings_pct = percent(savings, raw_tokens) if savings is not None else None
        rows.append([
            cid,
            tokens,
            savings,
            savings_pct,
        ])
    return rows


def chart_rows(conditions: dict, key: str) -> list[str]:
    values = {cid: int(item.get(key) or 0) for cid, item in conditions.items()}
    max_value = max(values.values(), default=0)
    lines = []
    for cid, value in values.items():
        lines.append(f"{cid:28} {bar(value, max_value)} {value}")
    return lines


def load_comparisons(run_dir: Path) -> dict:
    compare_dir = run_dir / "assets" / "raw" / "comparisons"
    data = {}
    if not compare_dir.is_dir():
        return data
    aggregate = compare_dir / "comparisons.json"
    if aggregate.is_file():
        return read_json(aggregate)
    for path in sorted(compare_dir.glob("*.json")):
        item = read_json(path)
        data[item["question_id"]] = item
    return data


def per_question_rows(scorecard: dict, comparisons: dict, questions: dict) -> list[list[object]]:
    rows = []
    for qid, q in questions.items():
        comparison = comparisons.get(qid, {})
        q_scores = scorecard.get("questions", {}).get(qid, {})
        completed = sum(1 for item in q_scores.values() if item and item.get("status") == "completed")
        best_quality = comparison.get("winner_by_quality")
        best_latency = comparison.get("winner_by_latency")
        rows.append([
            qid,
            q.get("class"),
            completed,
            best_quality,
            best_latency,
        ])
    return rows


def artifact_rows(metrics: dict) -> list[list[str]]:
    paths = metrics.get("artifact_paths", {})
    rows = [["metrics", "metrics.json"], ["manifest", "manifest.json"]]
    for name, path in paths.items():
        rows.append([name, path])
    return rows


def render(run_dir: Path) -> str:
    metrics = read_json(run_dir / "metrics.json")
    scorecard = read_json(run_dir / "assets" / "derived" / "scorecard.json")
    comparisons = load_comparisons(run_dir)
    conditions = metrics["conditions"]
    questions = metrics["questions"]
    setup = metrics.get("setup_metrics", {})
    score_summary = metrics.get("score_summary", {})
    skipped = ", ".join(score_summary.get("skipped_conditions", [])) or "none"
    raw_tokens = conditions.get("C1_naked_raw_context", {}).get("input_tokens_total") or 0
    residency_tokens = conditions.get("C5_yai_case_residency", {}).get("input_tokens_total") or 0
    residency_savings = raw_tokens - residency_tokens
    residency_savings_pct = percent(residency_savings, raw_tokens)

    lines = [
        "# LAB.TEST.2 - Case-Native Benchmark Matrix",
        "",
        "## Run Validity",
        "",
        table(
            ["Field", "Value"],
            [
                ["run id", metrics.get("run_id")],
                ["created at", metrics.get("created_at")],
                ["case", metrics.get("case_ref")],
                ["provider model", metrics.get("provider_model")],
                ["provider url", metrics.get("provider_url")],
                ["case opened/materialized via CLI", yes_no(bool(setup))],
                ["case-bound model calls through YAI CLI", yes_no(bool(conditions["C4_yai_case_bound"].get("questions_answered")))],
                ["naked calls direct provider", yes_no(bool(conditions["C0_naked_no_context"].get("questions_answered")))],
                ["direct provider used by YAI case-bound", yes_no(bool(conditions["C4_yai_case_bound"].get("direct_provider_used")))],
                ["C3 projection dump", conditions["C3_naked_yai_projection_dump"].get("status")],
            ],
        ),
        "",
        "## Main Findings",
        "",
        table(
            ["Finding", "Condition"],
            [
                ["best case knowledge", score_summary.get("best_case_knowledge")],
                ["best boundary score", score_summary.get("best_boundary_score")],
                ["fastest useful condition", score_summary.get("fastest_useful_condition")],
                ["token-heavy condition", score_summary.get("token_heavy_condition")],
                ["skipped conditions", skipped],
            ],
        ),
        "",
        "## Condition Matrix",
        "",
        table(
            ["Condition", "Status", "Provider", "YAI CLI", "Direct HTTP", "Answered", "Skipped", "Input Tok", "Output Tok", "Latency ms"],
            condition_rows(conditions),
        ),
        "",
        "## Score Matrix",
        "",
        table(
            ["Condition", "Case Knowledge", "Boundary", "Grounding", "Operational Safety"],
            score_rows(conditions),
        ),
        "",
        "## Context Efficiency",
        "",
        table(
            ["Condition", "Input Tok", "Savings vs Raw", "Savings %"],
            efficiency_rows(conditions),
        ),
        "",
        f"C5 YAI residency saves {residency_savings} estimated input tokens vs C1 raw context ({cell(residency_savings_pct)}%).",
        "KV cache reuse is not claimed; C5 is logical residency/base-delta posture.",
        "",
        "## Token Chart",
        "",
        "```text",
        *chart_rows(conditions, "input_tokens_total"),
        "```",
        "",
        "## Latency Chart",
        "",
        "```text",
        *chart_rows(conditions, "latency_total_ms"),
        "```",
        "",
        "## Per-Question Winners",
        "",
        table(
            ["Question", "Class", "Completed Conditions", "Winner Quality", "Winner Latency"],
            per_question_rows(scorecard, comparisons, questions),
        ),
        "",
        "## Setup Metrics",
        "",
        table(["Step", "ms"], [[key, value] for key, value in setup.items()]),
        "",
        "## Artifact Map",
        "",
        table(["Artifact", "Path"], artifact_rows(metrics)),
        "",
        "## Interpretation",
        "",
        "- A naked no-context answer is a negative control and cannot be treated as a useful case-answering baseline.",
        "- Direct provider calls are allowed only for naked baselines.",
        "- YAI case-bound calls use `yai provider`, `yai case attach-provider` and `yai prompt`.",
        "- `transcript.md` contains the full chronological execution record.",
        "- `assets/derived/answers.md` keeps full model answers as a reusable asset.",
        "- Token counts are estimates unless a model/provider tokenizer is available.",
        "",
    ]
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--run-dir", required=True)
    args = parser.parse_args()
    run_dir = Path(args.run_dir)
    report = run_dir / "report.md"
    report.write_text(render(run_dir), encoding="utf-8")
    print(report)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
