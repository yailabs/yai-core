#!/usr/bin/env python3
"""Validate NET stream envelope fixture discipline."""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURE_DIR = ROOT / "work/protocols/fixtures/net/stream"

EXPECTED_EVENTS = {
    "request",
    "response",
    "chunk",
    "metric",
    "receipt",
    "error",
    "complete",
}

REQUIRED_FIELDS = {
    "schema_version",
    "stream_id",
    "invocation_id",
    "trace_id",
    "sequence",
    "event_kind",
    "stream_state",
    "payload_kind",
    "origin_node_id",
    "target_node_id",
    "timestamp_unix_ms",
    "payload",
}

ALLOWED_STATES = {
    "created",
    "open",
    "half_closed",
    "completed",
    "failed",
    "cancelled",
}

ALLOWED_PAYLOADS = {
    "none",
    "bytes",
    "text",
    "json",
    "metric",
    "receipt",
    "error",
}

FORBIDDEN_TEXT = (
    "secret",
    "password",
    "token",
    "api_key",
    "private_key",
)

LOCALHOST_PORT_RE = re.compile(r"\b(localhost|127\.0\.0\.1):\d+\b", re.IGNORECASE)


def fail(message: str) -> None:
    print(f"check-net-stream-fixtures: {message}", file=sys.stderr)
    raise SystemExit(1)


def require_nonnegative_int(value: object, field: str, path: Path) -> None:
    if not isinstance(value, int) or isinstance(value, bool) or value < 0:
        fail(f"{path}: {field} must be integer >= 0")


def main() -> None:
    if not FIXTURE_DIR.is_dir():
        fail(f"missing fixture directory: {FIXTURE_DIR.relative_to(ROOT)}")

    fixture_paths = sorted(FIXTURE_DIR.glob("*.json"))
    expected_paths = {FIXTURE_DIR / f"{event}.json" for event in EXPECTED_EVENTS}
    if set(fixture_paths) != expected_paths:
        found = ", ".join(path.name for path in fixture_paths)
        expected = ", ".join(f"{event}.json" for event in sorted(EXPECTED_EVENTS))
        fail(f"fixture filenames mismatch; found [{found}], expected [{expected}]")

    for path in fixture_paths:
        raw = path.read_text(encoding="utf-8")
        lowered = raw.lower()

        for forbidden in FORBIDDEN_TEXT:
            if forbidden in lowered:
                fail(f"{path.relative_to(ROOT)} contains forbidden text: {forbidden}")

        if LOCALHOST_PORT_RE.search(raw) and "fake" not in lowered:
            fail(f"{path.relative_to(ROOT)} contains a real localhost port claim")

        try:
            fixture = json.loads(raw)
        except json.JSONDecodeError as exc:
            fail(f"{path.relative_to(ROOT)} is invalid JSON: {exc}")

        if not isinstance(fixture, dict):
            fail(f"{path.relative_to(ROOT)} must contain a JSON object")

        missing = sorted(REQUIRED_FIELDS - set(fixture))
        if missing:
            fail(f"{path.relative_to(ROOT)} missing fields: {', '.join(missing)}")

        event_from_name = path.stem
        event_kind = fixture["event_kind"]
        if event_kind not in EXPECTED_EVENTS:
            fail(f"{path.relative_to(ROOT)} invalid event_kind: {event_kind}")
        if event_kind != event_from_name:
            fail(f"{path.relative_to(ROOT)} filename does not match event_kind")

        stream_state = fixture["stream_state"]
        if stream_state not in ALLOWED_STATES:
            fail(f"{path.relative_to(ROOT)} invalid stream_state: {stream_state}")

        payload_kind = fixture["payload_kind"]
        if payload_kind not in ALLOWED_PAYLOADS:
            fail(f"{path.relative_to(ROOT)} invalid payload_kind: {payload_kind}")

        require_nonnegative_int(fixture["sequence"], "sequence", path.relative_to(ROOT))
        require_nonnegative_int(
            fixture["timestamp_unix_ms"],
            "timestamp_unix_ms",
            path.relative_to(ROOT),
        )

    print("check-net-stream-fixtures: ok")


if __name__ == "__main__":
    main()
