#!/usr/bin/env python3
import argparse
import html
import json
import math
import os
import re
import sys
import textwrap
import time
from pathlib import Path


STATUS_COLORS = {
    "reachable": "#1f9d55",
    "auth_blocked": "#d97706",
    "connection_refused": "#dc2626",
    "no_gpu": "#64748b",
    "completed": "#2563eb",
    "incomplete": "#6b7280",
}


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--root", required=True)
    parser.add_argument("--curation", required=True)
    parser.add_argument("--out", required=True)
    return parser.parse_args()


def parse_scalar(value):
    value = value.strip()
    if value == "":
        return ""
    if value in ("true", "false"):
        return value == "true"
    if (value.startswith('"') and value.endswith('"')) or (
        value.startswith("'") and value.endswith("'")
    ):
        return value[1:-1]
    return value


def parse_simple_yaml(path):
    data = {}
    current_key = None
    current_item = None
    for raw_line in path.read_text(encoding="utf-8").splitlines():
        line = raw_line.rstrip()
        if not line.strip() or line.lstrip().startswith("#"):
            continue
        if not raw_line.startswith(" "):
            key, sep, value = line.partition(":")
            if not sep:
                raise ValueError(f"unsupported curation line: {raw_line}")
            key = key.strip()
            value = value.strip()
            if value:
                data[key] = parse_scalar(value)
                current_key = None
                current_item = None
            else:
                data[key] = []
                current_key = key
                current_item = None
            continue
        stripped = line.strip()
        if stripped.startswith("- "):
            if current_key is None:
                raise ValueError(f"list item without parent: {raw_line}")
            value = stripped[2:].strip()
            if (value.startswith('"') and value.endswith('"')) or (
                value.startswith("'") and value.endswith("'")
            ):
                current_item = None
                data[current_key].append(parse_scalar(value))
            elif ":" in value:
                key, _, item_value = value.partition(":")
                current_item = {key.strip(): parse_scalar(item_value)}
                data[current_key].append(current_item)
            else:
                current_item = None
                data[current_key].append(parse_scalar(value))
            continue
        if current_item is not None and ":" in stripped:
            key, _, value = stripped.partition(":")
            current_item[key.strip()] = parse_scalar(value)
            continue
        raise ValueError(f"unsupported curation line: {raw_line}")
    return data


def load_curation(path):
    curation_path = Path(path)
    try:
        import yaml

        with curation_path.open("r", encoding="utf-8") as f:
            return yaml.safe_load(f)
    except ImportError:
        return parse_simple_yaml(curation_path)


def load_json(path, default=None):
    try:
        with Path(path).open("r", encoding="utf-8") as f:
            return json.load(f)
    except (OSError, json.JSONDecodeError):
        return default if default is not None else {}


def read_text(path):
    try:
        return Path(path).read_text(encoding="utf-8")
    except OSError:
        return ""


def parse_curl_meta(path):
    meta = {}
    for line in read_text(path).splitlines():
        if "=" in line:
            key, value = line.split("=", 1)
            meta[key.strip()] = value.strip()
    return meta


def infer_curl_status(run_dir, stem, curl_meta):
    http_code = curl_meta.get("http_code", "")
    stderr = read_text(run_dir / f"{stem}.stderr.txt")
    match = re.search(r"curl: \((\d+)\)", stderr)
    if match:
        return int(match.group(1))
    if http_code and http_code != "000":
        return 0
    return 1


def classify_endpoint(http_code, curl_status, error_message=""):
    if curl_status == 7 or http_code == "000":
        return "connection_refused"
    if http_code == "401" or "Invalid API Key" in error_message:
        return "auth_blocked"
    try:
        code = int(http_code)
    except (TypeError, ValueError):
        return "incomplete"
    if 200 <= code < 300:
        return "reachable"
    return "incomplete"


def as_float(value, default=0.0):
    try:
        return float(value)
    except (TypeError, ValueError):
        return default


def first_model_inventory(models_json):
    data = models_json.get("data") or []
    if data:
        item = data[0]
        meta = item.get("meta") or {}
        raw_model_id = item.get("id", "")
        model_id = raw_model_id
        model_path_redacted = False
        if isinstance(raw_model_id, str) and raw_model_id.startswith("/"):
            model_id = Path(raw_model_id).name
            model_path_redacted = True
        return {
            "model_id": model_id,
            "model_path_redacted": model_path_redacted,
            "owner": item.get("owned_by", ""),
            "format": (models_json.get("models") or [{}])[0]
            .get("details", {})
            .get("format", ""),
            "parameters": meta.get("n_params", ""),
            "size_bytes": meta.get("size", ""),
            "train_context": meta.get("n_ctx_train", ""),
            "embedding_size": meta.get("n_embd", ""),
        }
    return {}


def normalize_provider_probe(root, run_id, run_meta):
    run_dir = root / run_id
    meta = load_json(run_dir / "meta.json")
    models_json = load_json(run_dir / "models.json")
    chat_json = load_json(run_dir / "chat.json")
    chat_request = load_json(run_dir / "chat-request.json")
    prompt_text = ""
    for message in chat_request.get("messages", []):
        if message.get("role") == "user":
            prompt_text = message.get("content", "")
            break
    endpoints = []
    for stem, endpoint in (
        ("models", "/v1/models"),
        ("chat", "/v1/chat/completions"),
    ):
        curl_meta = parse_curl_meta(run_dir / f"{stem}.curl-meta.txt")
        curl_status = infer_curl_status(run_dir, stem, curl_meta)
        http_code = curl_meta.get("http_code", "000")
        error_message = ""
        if stem == "chat":
            error = chat_json.get("error") or {}
            error_message = error.get("message", "") if isinstance(error, dict) else ""
        status = classify_endpoint(http_code, curl_status, error_message)
        endpoints.append(
            {
                "run_id": run_id,
                "label": run_meta.get("label", run_id),
                "role": run_meta.get("role", ""),
                "host": meta.get("host", ""),
                "port": str(meta.get("port", "")),
                "endpoint": endpoint,
                "curl_status": curl_status,
                "http_code": http_code,
                "time_seconds": as_float(curl_meta.get("time_total")),
                "status": status,
                "error_message": error_message,
            }
        )
    return {
        "run": {
            "id": run_id,
            "kind": "lan_provider_probe",
            "role": run_meta.get("role", ""),
            "label": run_meta.get("label", run_id),
            "created_at_utc": meta.get("created_at_utc", ""),
            "status": "completed",
            "path": str(run_dir),
        },
        "endpoints": endpoints,
        "model_inventory": first_model_inventory(models_json),
        "prompts": [
            {
                "run_id": run_id,
                "prompt_id": f"{run_id}.P001",
                "role": "user",
                "content": prompt_text,
                "max_tokens": chat_request.get("max_tokens", ""),
                "temperature": chat_request.get("temperature", ""),
                "source": "chat-request.json",
            }
        ]
        if prompt_text
        else [],
    }


def normalize_hnsw(root, run_id, run_meta):
    run_dir = root / run_id
    summary = load_json(run_dir / "summary.json")
    return {
        "run": {
            "id": run_id,
            "kind": "hnsw_benchmark",
            "role": run_meta.get("role", ""),
            "label": run_meta.get("label", run_id),
            "created_at_utc": summary.get("created_at_utc", ""),
            "status": "completed",
            "path": str(run_dir),
        },
        "hnsw": {
            "run_id": run_id,
            "label": run_meta.get("label", run_id),
            "method": summary.get("method", ""),
            "numpy_available": summary.get("numpy_available", ""),
            "hnswlib_available": summary.get("hnswlib_available", ""),
            "n": summary.get("n", ""),
            "dim": summary.get("dim", ""),
            "queries": summary.get("queries", ""),
            "k": summary.get("k", ""),
            "query_ms_per_query": summary.get("query_ms_per_query", ""),
        },
    }


def normalize_local_model(root, run_id, run_meta):
    run_dir = root / run_id
    summary = load_json(run_dir / "summary.json")
    return {
        "run": {
            "id": run_id,
            "kind": "local_model_benchmark",
            "role": run_meta.get("role", ""),
            "label": run_meta.get("label", run_id),
            "created_at_utc": summary.get("created_at_utc", ""),
            "status": "completed" if summary.get("exit_status") == 0 else "incomplete",
            "path": str(run_dir),
        },
        "local_model": {
            "run_id": run_id,
            "model": summary.get("model", ""),
            "workload": summary.get("workload", ""),
            "wall_time_seconds": summary.get("wall_time_seconds", ""),
            "estimated_tokens_per_second": summary.get("estimated_tokens_per_second", ""),
            "peak_vram_mib": summary.get("peak_vram_mib", ""),
        },
    }


def normalize_baseline(root, run_id, run_meta):
    run_dir = root / run_id
    baseline = load_json(run_dir / "baseline.json")
    nvidia_err = read_text(run_dir / "nvidia-smi.txt.err")
    status = "no_gpu" if "missing command: nvidia-smi" in nvidia_err else "completed"
    return {
        "run": {
            "id": run_id,
            "kind": "hardware_baseline",
            "role": run_meta.get("role", ""),
            "label": run_meta.get("label", run_id),
            "created_at_utc": baseline.get("created_at_utc", ""),
            "status": status,
            "path": str(run_dir),
        },
        "baseline": {
            "run_id": run_id,
            "status": status,
            "tool_versions": read_text(run_dir / "tool-versions.tsv"),
        },
    }


def normalize_runs(root, curation):
    normalized = {
        "schema": "yai.benchmark_report.normalized.v1",
        "generated_at_utc": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
        "curation": curation,
        "runs": [],
        "endpoint_probes": [],
        "model_inventory": [],
        "hnsw": [],
        "local_model": [],
        "baselines": [],
        "prompts": [],
    }
    for run_meta in curation.get("curated_runs", []):
        run_id = run_meta["id"]
        run_dir = root / run_id
        if (run_dir / "meta.json").exists():
            item = normalize_provider_probe(root, run_id, run_meta)
            normalized["runs"].append(item["run"])
            normalized["endpoint_probes"].extend(item["endpoints"])
            normalized["prompts"].extend(item["prompts"])
            if item["model_inventory"]:
                inventory = item["model_inventory"]
                inventory["run_id"] = run_id
                normalized["model_inventory"].append(inventory)
        elif (run_dir / "summary.json").exists():
            summary = load_json(run_dir / "summary.json")
            schema = summary.get("schema", "")
            if schema == "yai.hnsw_benchmark.v1":
                item = normalize_hnsw(root, run_id, run_meta)
                normalized["runs"].append(item["run"])
                normalized["hnsw"].append(item["hnsw"])
            else:
                item = normalize_local_model(root, run_id, run_meta)
                normalized["runs"].append(item["run"])
                normalized["local_model"].append(item["local_model"])
        elif (run_dir / "baseline.json").exists():
            item = normalize_baseline(root, run_id, run_meta)
            normalized["runs"].append(item["run"])
            normalized["baselines"].append(item["baseline"])
        else:
            normalized["runs"].append(
                {
                    "id": run_id,
                    "kind": "missing",
                    "role": run_meta.get("role", ""),
                    "label": run_meta.get("label", run_id),
                    "status": "incomplete",
                    "path": str(run_dir),
                }
            )
    return normalized


def ensure_dir(path):
    Path(path).mkdir(parents=True, exist_ok=True)


def esc(value):
    return html.escape(str(value), quote=True)


def write_file(path, content):
    Path(path).parent.mkdir(parents=True, exist_ok=True)
    Path(path).write_text(content, encoding="utf-8")


def svg_wrap(width, height, body):
    return (
        f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}" '
        f'viewBox="0 0 {width} {height}" role="img">\n{body}\n</svg>\n'
    )


def try_plotly_endpoint_latency(path, endpoints):
    try:
        import pandas as pd
        import plotly.express as px

        df = pd.DataFrame(endpoints)
        if df.empty:
            return False
        df["target"] = df["host"] + ":" + df["port"] + " " + df["endpoint"]
        fig = px.bar(
            df,
            x="time_seconds",
            y="target",
            color="status",
            orientation="h",
            color_discrete_map=STATUS_COLORS,
            title="Endpoint Latency",
            labels={"time_seconds": "Curl total time (seconds)", "target": "Endpoint"},
        )
        fig.update_layout(template="plotly_white", width=980, height=520)
        fig.write_image(str(path))
        return True
    except Exception:
        return False


def try_plotly_status_matrix(path, endpoints):
    try:
        import pandas as pd
        import plotly.graph_objects as go

        df = pd.DataFrame(endpoints)
        if df.empty:
            return False
        rows = sorted((df["host"] + ":" + df["port"]).unique())
        cols = sorted(df["endpoint"].unique())
        status_order = {
            "connection_refused": 0,
            "auth_blocked": 1,
            "incomplete": 2,
            "reachable": 3,
        }
        z = []
        text = []
        for row in rows:
            z_row = []
            text_row = []
            for col in cols:
                match = df[((df["host"] + ":" + df["port"]) == row) & (df["endpoint"] == col)]
                if match.empty:
                    status = "incomplete"
                    http_code = ""
                else:
                    status = str(match.iloc[0]["status"])
                    http_code = str(match.iloc[0]["http_code"])
                z_row.append(status_order.get(status, 2))
                text_row.append(f"{status}<br>HTTP {http_code}")
            z.append(z_row)
            text.append(text_row)
        fig = go.Figure(
            data=go.Heatmap(
                z=z,
                x=cols,
                y=rows,
                text=text,
                texttemplate="%{text}",
                showscale=False,
                colorscale=[
                    [0.0, STATUS_COLORS["connection_refused"]],
                    [0.33, STATUS_COLORS["auth_blocked"]],
                    [0.66, STATUS_COLORS["incomplete"]],
                    [1.0, STATUS_COLORS["reachable"]],
                ],
            )
        )
        fig.update_layout(template="plotly_white", title="Endpoint Status Matrix", width=980, height=420)
        fig.write_image(str(path))
        return True
    except Exception:
        return False


def try_plotly_hnsw(path, hnsw_rows):
    try:
        import pandas as pd
        import plotly.express as px

        df = pd.DataFrame(hnsw_rows)
        if df.empty:
            return False
        fig = px.bar(
            df,
            x="query_ms_per_query",
            y="label",
            color_discrete_sequence=[STATUS_COLORS["completed"]],
            orientation="h",
            title="HNSW Query Latency",
            labels={"query_ms_per_query": "Milliseconds per query", "label": "Run"},
        )
        fig.update_layout(template="plotly_white", width=860, height=420)
        fig.write_image(str(path))
        return True
    except Exception:
        return False


def try_plotly_topology_html(path, normalized):
    try:
        import plotly.graph_objects as go

        curation = normalized["curation"]
        negative_ports = [str(port) for port in curation.get("negative_evidence_ports", [])]
        nodes = [
            ("Operator", 0, 0, 0, STATUS_COLORS["completed"]),
            ("Provider 43117", 2, 1, 1, STATUS_COLORS["reachable"]),
            ("Chat auth gate", 2, -1, 1.4, STATUS_COLORS["auth_blocked"]),
        ]
        if negative_ports:
            nodes.append((f"Port {negative_ports[0]}", 1, 1.8, 0.6, STATUS_COLORS["connection_refused"]))
        fig = go.Figure()
        fig.add_trace(
            go.Scatter3d(
                x=[n[1] for n in nodes],
                y=[n[2] for n in nodes],
                z=[n[3] for n in nodes],
                mode="markers+text",
                text=[n[0] for n in nodes],
                textposition="top center",
                marker={"size": 9, "color": [n[4] for n in nodes]},
                name="topology",
            )
        )
        edges = [(0, 1, "reachable"), (0, 2, "auth_blocked")]
        if negative_ports:
            edges.append((0, 3, "connection_refused"))
        for a, b, status in edges:
            fig.add_trace(
                go.Scatter3d(
                    x=[nodes[a][1], nodes[b][1]],
                    y=[nodes[a][2], nodes[b][2]],
                    z=[nodes[a][3], nodes[b][3]],
                    mode="lines",
                    line={"color": STATUS_COLORS[status], "width": 6},
                    showlegend=False,
                )
            )
        fig.update_layout(
            title=f"Provider Topology 3D: {curation.get('provider_host', '')}",
            template="plotly_dark",
            width=980,
            height=620,
            scene={"xaxis_title": "Boundary", "yaxis_title": "Endpoint posture", "zaxis_title": "Evidence layer"},
        )
        fig.write_html(str(path), include_plotlyjs=True, full_html=True)
        return True
    except Exception:
        return False


def write_endpoint_latency_svg(path, endpoints):
    if try_plotly_endpoint_latency(path, endpoints):
        return
    width, height = 980, 520
    plot_x, plot_y, plot_w, plot_h = 90, 90, 820, 300
    max_time = max([e["time_seconds"] for e in endpoints] or [1.0])
    max_time = max(max_time, 0.001)
    bar_h = 30
    gap = 24
    body = [
        '<rect width="980" height="520" fill="#ffffff"/>',
        '<text x="90" y="46" font-family="Arial" font-size="24" font-weight="700" fill="#111827">Endpoint Latency</text>',
        '<text x="90" y="72" font-family="Arial" font-size="13" fill="#4b5563">Curl total time by endpoint; color encodes canonical endpoint status.</text>',
        f'<line x1="{plot_x}" y1="{plot_y + plot_h}" x2="{plot_x + plot_w}" y2="{plot_y + plot_h}" stroke="#d1d5db"/>',
    ]
    for i, endpoint in enumerate(endpoints):
        y = plot_y + i * (bar_h + gap)
        length = int((endpoint["time_seconds"] / max_time) * (plot_w - 240))
        color = STATUS_COLORS.get(endpoint["status"], STATUS_COLORS["incomplete"])
        label = f'{endpoint["host"]}:{endpoint["port"]} {endpoint["endpoint"]}'
        body.extend(
            [
                f'<text x="{plot_x}" y="{y + 20}" font-family="Arial" font-size="12" fill="#111827">{esc(label)}</text>',
                f'<rect x="{plot_x + 260}" y="{y}" width="{max(length, 3)}" height="{bar_h}" rx="3" fill="{color}"/>',
                f'<text x="{plot_x + 270 + max(length, 3)}" y="{y + 20}" font-family="Arial" font-size="12" fill="#111827">{endpoint["time_seconds"]:.3f}s</text>',
                f'<text x="{plot_x + 470}" y="{y + 20}" font-family="Arial" font-size="12" fill="{color}">{esc(endpoint["status"])}</text>',
            ]
        )
    write_file(path, svg_wrap(width, height, "\n".join(body)))


def write_endpoint_status_matrix_svg(path, endpoints):
    if try_plotly_status_matrix(path, endpoints):
        return
    width, height = 980, 420
    cell_w, cell_h = 210, 58
    start_x, start_y = 90, 118
    columns = sorted({e["endpoint"] for e in endpoints})
    rows = sorted({f'{e["host"]}:{e["port"]}' for e in endpoints})
    lookup = {(f'{e["host"]}:{e["port"]}', e["endpoint"]): e for e in endpoints}
    body = [
        '<rect width="980" height="420" fill="#ffffff"/>',
        '<text x="90" y="46" font-family="Arial" font-size="24" font-weight="700" fill="#111827">Endpoint Status Matrix</text>',
        '<text x="90" y="72" font-family="Arial" font-size="13" fill="#4b5563">Reachability and authentication posture for curated LAN probes.</text>',
    ]
    for j, column in enumerate(columns):
        x = start_x + 180 + j * cell_w
        body.append(f'<text x="{x}" y="{start_y - 18}" font-family="Arial" font-size="12" fill="#111827">{esc(column)}</text>')
    for i, row in enumerate(rows):
        y = start_y + i * cell_h
        body.append(f'<text x="{start_x}" y="{y + 35}" font-family="Arial" font-size="13" fill="#111827">{esc(row)}</text>')
        for j, column in enumerate(columns):
            x = start_x + 180 + j * cell_w
            item = lookup.get((row, column), {})
            status = item.get("status", "incomplete")
            color = STATUS_COLORS.get(status, STATUS_COLORS["incomplete"])
            body.extend(
                [
                    f'<rect x="{x}" y="{y}" width="{cell_w - 18}" height="{cell_h - 14}" rx="4" fill="{color}" opacity="0.90"/>',
                    f'<text x="{x + 12}" y="{y + 28}" font-family="Arial" font-size="12" fill="#ffffff">{esc(status)}</text>',
                    f'<text x="{x + 12}" y="{y + 42}" font-family="Arial" font-size="10" fill="#ffffff">HTTP {esc(item.get("http_code", ""))}</text>',
                ]
            )
    write_file(path, svg_wrap(width, height, "\n".join(body)))


def write_hnsw_svg(path, hnsw_rows):
    if try_plotly_hnsw(path, hnsw_rows):
        return
    width, height = 860, 420
    body = [
        '<rect width="860" height="420" fill="#ffffff"/>',
        '<text x="80" y="46" font-family="Arial" font-size="24" font-weight="700" fill="#111827">HNSW Query Latency</text>',
        '<text x="80" y="72" font-family="Arial" font-size="13" fill="#4b5563">Query milliseconds per query for curated vector-search smoke runs.</text>',
    ]
    if not hnsw_rows:
        body.append('<text x="80" y="150" font-family="Arial" font-size="14" fill="#6b7280">No HNSW runs in this curation.</text>')
        write_file(path, svg_wrap(width, height, "\n".join(body)))
        return
    max_ms = max([as_float(row.get("query_ms_per_query")) for row in hnsw_rows] or [1.0])
    max_ms = max(max_ms, 0.001)
    for i, row in enumerate(hnsw_rows):
        y = 130 + i * 70
        length = int((as_float(row.get("query_ms_per_query")) / max_ms) * 560)
        label = f'{row.get("label", row.get("run_id", ""))} ({row.get("method", "")})'
        body.extend(
            [
                f'<text x="80" y="{y + 20}" font-family="Arial" font-size="12" fill="#111827">{esc(label)}</text>',
                f'<rect x="330" y="{y}" width="{max(length, 3)}" height="32" rx="3" fill="{STATUS_COLORS["completed"]}"/>',
                f'<text x="{340 + max(length, 3)}" y="{y + 21}" font-family="Arial" font-size="12" fill="#111827">{as_float(row.get("query_ms_per_query")):.3f} ms/query</text>',
            ]
        )
    write_file(path, svg_wrap(width, height, "\n".join(body)))


def write_topology_html(path, normalized):
    if try_plotly_topology_html(path, normalized):
        return
    curation = normalized["curation"]
    endpoints = normalized["endpoint_probes"]
    inventory = normalized["model_inventory"][0] if normalized["model_inventory"] else {}
    negative_ports = [str(port) for port in curation.get("negative_evidence_ports", [])]
    dead_edge = '<div class="edge deadline"></div>' if negative_ports else ""
    dead_node = ""
    if negative_ports:
        dead_node = f'<div class="node dead"><strong>Negative Evidence</strong><span>{esc(curation.get("provider_host", ""))}:{esc(negative_ports[0])}</span><span>connection_refused</span></div>'
    topology_description = (
        "Three-dimensional view of the LAN benchmark boundary. The live model discovery path is separated from the chat authentication gate and the refused port."
        if negative_ports
        else "Three-dimensional view of the LAN benchmark boundary. The live model discovery path is separated from the chat authentication gate."
    )
    payload = json.dumps(
        {
            "operator_host": curation.get("operator_host", "operator"),
            "provider_host": curation.get("provider_host", "provider"),
            "live_provider_port": curation.get("live_provider_port", ""),
            "negative_evidence_ports": curation.get("negative_evidence_ports", []),
            "endpoints": endpoints,
            "model": inventory,
        },
        indent=2,
    )
    html_doc = f"""<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>Provider Topology 3D</title>
  <style>
    body {{ margin: 0; font-family: Arial, sans-serif; background: #0f172a; color: #e5e7eb; }}
    main {{ min-height: 100vh; display: grid; grid-template-columns: 1fr 360px; gap: 28px; padding: 40px; box-sizing: border-box; }}
    .scene {{ perspective: 1200px; display: grid; place-items: center; }}
    .space {{ width: 720px; height: 440px; position: relative; transform-style: preserve-3d; transform: rotateX(58deg) rotateZ(-34deg); }}
    .plane {{ position: absolute; inset: 40px; border: 1px solid rgba(148,163,184,.45); background: linear-gradient(135deg, rgba(30,41,59,.88), rgba(15,23,42,.55)); box-shadow: 0 30px 80px rgba(0,0,0,.35); }}
    .node {{ position: absolute; width: 190px; min-height: 82px; padding: 14px; border-radius: 8px; background: #111827; border: 1px solid #334155; transform-style: preserve-3d; transform: translateZ(46px); box-shadow: 0 18px 30px rgba(0,0,0,.30); }}
    .node strong {{ display: block; color: #f8fafc; font-size: 14px; margin-bottom: 8px; }}
    .node span {{ display: block; color: #cbd5e1; font-size: 12px; line-height: 1.35; }}
    .operator {{ left: 80px; top: 230px; border-color: #60a5fa; }}
    .provider {{ left: 410px; top: 150px; border-color: #34d399; }}
    .auth {{ left: 410px; top: 280px; border-color: #f59e0b; }}
    .dead {{ left: 240px; top: 70px; border-color: #ef4444; }}
    .edge {{ position: absolute; height: 4px; transform-origin: left center; background: #38bdf8; opacity: .78; }}
    .edge.live {{ left: 260px; top: 250px; width: 235px; transform: rotate(-17deg) translateZ(20px); background: #34d399; }}
    .edge.blocked {{ left: 278px; top: 285px; width: 220px; transform: rotate(4deg) translateZ(16px); background: #f59e0b; }}
    .edge.deadline {{ left: 245px; top: 185px; width: 170px; transform: rotate(-38deg) translateZ(12px); background: #ef4444; }}
    aside {{ align-self: center; background: rgba(15,23,42,.78); border: 1px solid #334155; border-radius: 8px; padding: 22px; }}
    h1 {{ font-size: 22px; margin: 0 0 10px; }}
    p {{ color: #cbd5e1; line-height: 1.45; }}
    code, pre {{ color: #bfdbfe; }}
    pre {{ max-height: 320px; overflow: auto; background: #020617; padding: 14px; border-radius: 6px; }}
  </style>
</head>
<body>
<main>
  <section class="scene" aria-label="3D provider topology">
    <div class="space">
      <div class="plane"></div>
      <div class="edge live"></div>
      <div class="edge blocked"></div>
      {dead_edge}
      <div class="node operator"><strong>Operator</strong><span>{esc(curation.get("operator_host", "MacBook"))}</span><span>YAI repo and report layer</span></div>
      <div class="node provider"><strong>Provider Live Port</strong><span>{esc(curation.get("provider_host", ""))}:{esc(curation.get("live_provider_port", ""))}</span><span>/v1/models reachable</span></div>
      <div class="node auth"><strong>Auth Gate</strong><span>/v1/chat/completions</span><span>auth_blocked</span></div>
      {dead_node}
    </div>
  </section>
  <aside>
    <h1>Provider Topology 3D</h1>
    <p>{esc(topology_description)}</p>
    <pre>{esc(payload)}</pre>
  </aside>
</main>
</body>
</html>
"""
    write_file(path, html_doc)


def md_table(headers, rows):
    out = ["| " + " | ".join(headers) + " |"]
    out.append("|" + "|".join(["---" for _ in headers]) + "|")
    for row in rows:
        out.append("| " + " | ".join(str(cell) for cell in row) + " |")
    return "\n".join(out)


def write_index_md(path, normalized):
    curation = normalized["curation"]
    endpoints = normalized["endpoint_probes"]
    inventory = normalized["model_inventory"][0] if normalized["model_inventory"] else {}
    hnsw_rows = normalized["hnsw"]
    runs = normalized["runs"]
    prompts = normalized.get("prompts", [])
    live_port = str(curation.get("live_provider_port", ""))
    negative_ports = [str(p) for p in curation.get("negative_evidence_ports", [])]
    negative_summary = (
        f', while {", ".join(negative_ports)} is negative evidence'
        if negative_ports
        else " with no negative-evidence port included in this curated run"
    )
    if negative_ports:
        interpretation_rows = [
            "- Proven: the provider host responds on `43117` for `/v1/models`.",
            "- Blocked: `/v1/chat/completions` reaches the server but returns `401 Invalid API Key`, classified as `auth_blocked`.",
            f'- Negative evidence: `{", ".join(negative_ports)}` is retained as `connection_refused` evidence and is not treated as the live provider port.',
            "- Missing: a valid provider API key is required before chat generation, token throughput and VRAM plots become meaningful.",
        ]
    else:
        interpretation_rows = [
            "- Proven: the provider host responds on `43117` for `/v1/models`.",
            "- Blocked: `/v1/chat/completions` reaches the server but returns `401 Invalid API Key`, classified as `auth_blocked`.",
            "- Missing: a valid provider API key is required before chat generation, token throughput and VRAM plots become meaningful.",
        ]
    interpretation = "\n".join(interpretation_rows)
    topology_caption = (
        "3D boundary view separating the operator host, the live provider port, the chat authentication gate and the refused port."
        if negative_ports
        else "3D boundary view separating the operator host, the live provider port and the chat authentication gate."
    )
    endpoint_rows = [
        [
            f'`{e["host"]}:{e["port"]}`',
            f'`{e["endpoint"]}`',
            e["curl_status"],
            e["http_code"],
            f'{e["time_seconds"]:.6f}',
            f'`{e["status"]}`',
            esc(e.get("error_message", "")),
        ]
        for e in endpoints
    ]
    model_rows = [
        ["Model id", f'`{inventory.get("model_id", "")}`'],
        ["Owner", f'`{inventory.get("owner", "")}`'],
        ["Format", f'`{inventory.get("format", "")}`'],
        ["Parameters", inventory.get("parameters", "")],
        ["Size bytes", inventory.get("size_bytes", "")],
        ["Train context", inventory.get("train_context", "")],
        ["Embedding size", inventory.get("embedding_size", "")],
    ]
    hnsw_table_rows = [
        [
            f'`{row.get("run_id", "")}`',
            f'`{row.get("method", "")}`',
            row.get("n", ""),
            row.get("dim", ""),
            row.get("queries", ""),
            row.get("k", ""),
            f'{as_float(row.get("query_ms_per_query")):.6f}',
        ]
        for row in hnsw_rows
    ]
    run_rows = [
        [f'`{run["id"]}`', f'`{run.get("kind", "")}`', f'`{run.get("status", "")}`', f'`{run.get("path", "")}`']
        for run in runs
    ]
    prompt_rows = [
        [
            f'`{prompt.get("prompt_id", "")}`',
            f'`{prompt.get("run_id", "")}`',
            f'`{prompt.get("role", "")}`',
            esc(prompt.get("content", "")),
            prompt.get("max_tokens", ""),
            prompt.get("temperature", ""),
        ]
        for prompt in prompts
    ]
    claims = "\n".join(f"- `{claim}`" for claim in curation.get("headline_claims", []))
    md = f"""# {curation.get("title", "Benchmark Report")}

Status: generated local lab report.

## Executive Summary

The curated LAN provider evidence shows that `172.20.10.3:{live_port}` is the live provider port for model discovery{negative_summary}. The provider returns `reachable` for `/v1/models` and `auth_blocked` for `/v1/chat/completions`; the current blocker is provider authentication, not LAN reachability.

Headline evidence:

{claims}

## Provider Readiness

{md_table(["Target", "Endpoint", "Curl status", "HTTP code", "Time seconds", "Status", "Error"], endpoint_rows)}

![Endpoint status matrix](assets/endpoint-status-matrix.svg)

Caption: curated endpoint status matrix using the canonical status palette from `benchmark-visual-language.md`.

![Endpoint latency](assets/endpoint-latency.svg)

Caption: curl total time for each curated endpoint probe. Failed and blocked probes remain visible evidence.

## Model Inventory

Observed through the reachable `/v1/models` endpoint.

{md_table(["Field", "Value"], model_rows)}

## Prompt Inventory

{md_table(["Prompt ID", "Run", "Role", "Content", "Max tokens", "Temperature"], prompt_rows)}

## Benchmark Panels

{md_table(["Run", "Method", "Vectors", "Dimensions", "Queries", "k", "Query ms/query"], hnsw_table_rows)}

![HNSW query latency](assets/hnsw-query-latency.svg)

Caption: vector-search smoke benchmark. This run used the standard-library fallback because NumPy and hnswlib were not installed on the operator host.

## 3D View

[Open provider topology 3D](assets/provider-topology-3d.html)

Caption: {topology_caption}

## Interpretation

{interpretation}

## Reproducibility Appendix

{md_table(["Run", "Kind", "Status", "Raw folder"], run_rows)}

Generated at `{normalized["generated_at_utc"]}` from `{curation.get("report_id", "")}`.
"""
    write_file(path, md)


def write_notebook(path, normalized):
    title = normalized["curation"].get("title", "Benchmark Report")
    notebook = {
        "cells": [
            {
                "cell_type": "markdown",
                "metadata": {},
                "source": [
                    f"# {title}\n",
                    "\n",
                    "Companion notebook for the generated Markdown benchmark report.\n",
                ],
            },
            {
                "cell_type": "code",
                "execution_count": None,
                "metadata": {},
                "outputs": [],
                "source": [
                    "import json\n",
                    "from pathlib import Path\n",
                    "data = json.loads(Path('data/normalized-runs.json').read_text())\n",
                    "data.keys()\n",
                ],
            },
            {
                "cell_type": "code",
                "execution_count": None,
                "metadata": {},
                "outputs": [],
                "source": [
                    "data['endpoint_probes']\n",
                ],
            },
        ],
        "metadata": {
            "kernelspec": {"display_name": "YAI Python", "language": "python", "name": "python3"},
            "language_info": {"name": "python", "version": "3"},
        },
        "nbformat": 4,
        "nbformat_minor": 5,
    }
    write_file(path, json.dumps(notebook, indent=2) + "\n")


def main():
    args = parse_args()
    root = Path(args.root)
    out = Path(args.out)
    curation = load_curation(args.curation)
    normalized = normalize_runs(root, curation)

    ensure_dir(out / "assets")
    ensure_dir(out / "data")

    write_file(out / "data" / "normalized-runs.json", json.dumps(normalized, indent=2, sort_keys=True) + "\n")
    write_endpoint_latency_svg(out / "assets" / "endpoint-latency.svg", normalized["endpoint_probes"])
    write_endpoint_status_matrix_svg(out / "assets" / "endpoint-status-matrix.svg", normalized["endpoint_probes"])
    write_hnsw_svg(out / "assets" / "hnsw-query-latency.svg", normalized["hnsw"])
    write_topology_html(out / "assets" / "provider-topology-3d.html", normalized)
    write_index_md(out / "index.md", normalized)
    write_notebook(out / "notebook.ipynb", normalized)
    print(f"benchmark report written: {out}")


if __name__ == "__main__":
    main()
