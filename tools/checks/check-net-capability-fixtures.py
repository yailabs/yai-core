#!/usr/bin/env python3
"""Validate NET capability advertisement fixture discipline."""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURE_DIR = ROOT / "proto/fixtures/net/capability"

EXPECTED_FILES = {
    "neural-llm-decode.json",
    "neural-embedding-encode.json",
    "metrics-stream.json",
    "receipt-emit.json",
    "generic-external.json",
}

REQUIRED_FIELDS = {
    "schema_version",
    "capability_id",
    "node_id",
    "capability_label",
    "capability_kind",
    "capability_family",
    "capability_scope",
    "capability_state",
    "constraints",
    "metadata",
    "advertised_at_unix_ms",
}

ALLOWED_KINDS = {
    "neural_llm_decode",
    "neural_embedding_encode",
    "metrics_stream",
    "receipt_emit",
    "generic_external",
}

ALLOWED_FAMILIES = {
    "neural",
    "metrics",
    "receipt",
    "generic",
}

ALLOWED_SCOPES = {
    "process",
    "node",
    "service",
    "external",
}

ALLOWED_STATES = {
    "unknown",
    "declared",
    "available",
    "degraded",
    "unavailable",
    "retired",
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
    print(f"check-net-capability-fixtures: {message}", file=sys.stderr)
    raise SystemExit(1)


def main() -> None:
    if not FIXTURE_DIR.is_dir():
        fail(f"missing fixture directory: {FIXTURE_DIR.relative_to(ROOT)}")

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

        if fixture["capability_kind"] not in ALLOWED_KINDS:
            fail(f"{rel} invalid capability_kind: {fixture['capability_kind']}")

        if fixture["capability_family"] not in ALLOWED_FAMILIES:
            fail(f"{rel} invalid capability_family: {fixture['capability_family']}")

        if fixture["capability_scope"] not in ALLOWED_SCOPES:
            fail(f"{rel} invalid capability_scope: {fixture['capability_scope']}")

        if fixture["capability_state"] not in ALLOWED_STATES:
            fail(f"{rel} invalid capability_state: {fixture['capability_state']}")

        advertised_at = fixture["advertised_at_unix_ms"]
        if not isinstance(advertised_at, int) or isinstance(advertised_at, bool) or advertised_at < 0:
            fail(f"{rel}: advertised_at_unix_ms must be integer >= 0")

        if not isinstance(fixture["constraints"], dict):
            fail(f"{rel}: constraints must be an object")

        if not isinstance(fixture["metadata"], dict):
            fail(f"{rel}: metadata must be an object")

    print("check-net-capability-fixtures: ok")


if __name__ == "__main__":
    main()
