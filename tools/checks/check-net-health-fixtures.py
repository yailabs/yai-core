#!/usr/bin/env python3
"""Validate NET health report fixture discipline."""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURE_DIR = ROOT / "proto/fixtures/net/health"
NODE_DIR = ROOT / "proto/fixtures/net/node"
ENDPOINT_DIR = ROOT / "proto/fixtures/net/endpoint"
CAPABILITY_DIR = ROOT / "proto/fixtures/net/capability"

EXPECTED_FILES = {
    "node-health.json",
    "endpoint-liveness.json",
    "endpoint-readiness.json",
    "capability-readiness.json",
    "degraded.json",
    "unavailable.json",
}

REQUIRED_FIELDS = {
    "schema_version",
    "health_report_id",
    "subject_id",
    "subject_kind",
    "check_kind",
    "health_state",
    "observed_by_node_id",
    "reason",
    "metadata",
    "observed_at_unix_ms",
}

ALLOWED_SUBJECT_KINDS = {
    "node",
    "endpoint",
    "capability",
    "transport",
    "external",
}

ALLOWED_CHECK_KINDS = {
    "health",
    "liveness",
    "readiness",
    "capability_readiness",
    "endpoint_reachability",
}

ALLOWED_HEALTH_STATES = {
    "unknown",
    "alive",
    "ready",
    "degraded",
    "unavailable",
    "failed",
}

FORBIDDEN_TEXT = (
    "hostname",
    "username",
    "ip_address",
    "mac_address",
    "serial",
    "/users/",
    "/home/",
    "secret",
    "password",
    "token",
    "api_key",
    "private_key",
)

FORBIDDEN_CLAIMS = (
    "production-ready",
    "benchmark result",
    "supports cuda",
    "supports metal",
    "supports mlx",
    "supports llama.cpp",
    "fastest",
)

FORBIDDEN_IP_TOKEN_RE = re.compile(r"(^|[^A-Za-z0-9_])ip([^A-Za-z0-9_]|$)", re.IGNORECASE)
FORBIDDEN_MAC_TOKEN_RE = re.compile(r"(^|[^A-Za-z0-9_])mac([^A-Za-z0-9_]|$)", re.IGNORECASE)
FORBIDDEN_HOME_TOKEN_RE = re.compile(r"(^|[^A-Za-z0-9_])home([^A-Za-z0-9_]|$)", re.IGNORECASE)


def fail(message: str) -> None:
    print(f"check-net-health-fixtures: {message}", file=sys.stderr)
    raise SystemExit(1)


def load_ids(directory: Path, field: str) -> set[str]:
    if not directory.is_dir():
        fail(f"missing reference fixture directory: {directory.relative_to(ROOT)}")

    ids: set[str] = set()
    for path in sorted(directory.glob("*.json")):
        rel = path.relative_to(ROOT)
        try:
            fixture = json.loads(path.read_text(encoding="utf-8"))
        except json.JSONDecodeError as exc:
            fail(f"{rel} is invalid JSON: {exc}")

        if not isinstance(fixture, dict):
            fail(f"{rel} must contain a JSON object")

        value = fixture.get(field)
        if not isinstance(value, str) or not value:
            fail(f"{rel} missing string field: {field}")
        ids.add(value)

    return ids


def main() -> None:
    if not FIXTURE_DIR.is_dir():
        fail(f"missing fixture directory: {FIXTURE_DIR.relative_to(ROOT)}")

    node_ids = load_ids(NODE_DIR, "node_id")
    endpoint_ids = load_ids(ENDPOINT_DIR, "endpoint_id")
    capability_ids = load_ids(CAPABILITY_DIR, "capability_id")

    paths = sorted(FIXTURE_DIR.glob("*.json"))
    found = {path.name for path in paths}
    if found != EXPECTED_FILES:
        fail(
            "fixture filenames mismatch; found "
            f"{sorted(found)}, expected {sorted(EXPECTED_FILES)}"
        )

    for path in paths:
        rel = path.relative_to(ROOT)
        raw = path.read_text(encoding="utf-8")
        lowered = raw.lower()

        for forbidden in FORBIDDEN_TEXT:
            if forbidden in lowered:
                fail(f"{rel} contains forbidden text: {forbidden}")

        for claim in FORBIDDEN_CLAIMS:
            if claim in lowered:
                fail(f"{rel} contains forbidden implementation claim: {claim}")

        if FORBIDDEN_IP_TOKEN_RE.search(raw):
            fail(f"{rel} contains forbidden text: ip")
        if FORBIDDEN_MAC_TOKEN_RE.search(raw):
            fail(f"{rel} contains forbidden text: mac")
        if FORBIDDEN_HOME_TOKEN_RE.search(raw):
            fail(f"{rel} contains forbidden text: home")

        try:
            fixture = json.loads(raw)
        except json.JSONDecodeError as exc:
            fail(f"{rel} is invalid JSON: {exc}")

        if not isinstance(fixture, dict):
            fail(f"{rel} must contain a JSON object")

        missing = sorted(REQUIRED_FIELDS - set(fixture))
        if missing:
            fail(f"{rel} missing fields: {', '.join(missing)}")

        if fixture["subject_kind"] not in ALLOWED_SUBJECT_KINDS:
            fail(f"{rel} invalid subject_kind: {fixture['subject_kind']}")

        if fixture["check_kind"] not in ALLOWED_CHECK_KINDS:
            fail(f"{rel} invalid check_kind: {fixture['check_kind']}")

        if fixture["health_state"] not in ALLOWED_HEALTH_STATES:
            fail(f"{rel} invalid health_state: {fixture['health_state']}")

        observed_at = fixture["observed_at_unix_ms"]
        if not isinstance(observed_at, int) or isinstance(observed_at, bool) or observed_at < 0:
            fail(f"{rel}: observed_at_unix_ms must be integer >= 0")

        if not isinstance(fixture["metadata"], dict):
            fail(f"{rel}: metadata must be an object")

        observed_by = fixture["observed_by_node_id"]
        if observed_by not in node_ids:
            fail(f"{rel}: observed_by_node_id does not resolve to node fixture id: {observed_by}")

        subject_id = fixture["subject_id"]
        subject_kind = fixture["subject_kind"]
        if subject_kind == "node" and subject_id not in node_ids:
            fail(f"{rel}: subject_id does not resolve to node fixture id: {subject_id}")
        if subject_kind == "endpoint" and subject_id not in endpoint_ids:
            fail(f"{rel}: subject_id does not resolve to endpoint fixture id: {subject_id}")
        if subject_kind == "capability" and subject_id not in capability_ids:
            fail(f"{rel}: subject_id does not resolve to capability fixture id: {subject_id}")

    print("check-net-health-fixtures: ok")


if __name__ == "__main__":
    main()
