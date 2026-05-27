#!/usr/bin/env python3
"""Validate NET transport adapter fixture discipline."""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURE_DIR = ROOT / "proto/fixtures/net/transport"
ENDPOINT_DIR = ROOT / "proto/fixtures/net/endpoint"

EXPECTED_FILES = {
    "localhost-http.json",
    "local-ipc.json",
    "lan-http.json",
    "remote-http.json",
    "future-transport.json",
}

REQUIRED_FIELDS = {
    "schema_version",
    "transport_adapter_id",
    "endpoint_id",
    "transport_label",
    "transport_kind",
    "transport_scope",
    "transport_state",
    "transport_security",
    "stream_profile",
    "metadata",
    "declared_at_unix_ms",
}

ALLOWED_KINDS = {
    "localhost_http",
    "local_ipc",
    "lan_http",
    "remote_http",
    "future",
}

ALLOWED_SCOPES = {
    "process",
    "machine",
    "lan",
    "remote",
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

ALLOWED_SECURITY = {
    "unknown",
    "local_only",
    "trusted_local",
    "declared_remote",
    "external_untrusted",
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
FORBIDDEN_PORT_TOKEN_RE = re.compile(r"(^|[^A-Za-z0-9_])port([^A-Za-z0-9_]|$)", re.IGNORECASE)


def fail(message: str) -> None:
    print(f"check-net-transport-fixtures: {message}", file=sys.stderr)
    raise SystemExit(1)


def load_endpoint_ids() -> set[str]:
    if not ENDPOINT_DIR.is_dir():
        fail(f"missing reference fixture directory: {ENDPOINT_DIR.relative_to(ROOT)}")

    endpoint_ids: set[str] = set()
    for path in sorted(ENDPOINT_DIR.glob("*.json")):
        rel = path.relative_to(ROOT)
        try:
            fixture = json.loads(path.read_text(encoding="utf-8"))
        except json.JSONDecodeError as exc:
            fail(f"{rel} is invalid JSON: {exc}")

        if not isinstance(fixture, dict):
            fail(f"{rel} must contain a JSON object")

        endpoint_id = fixture.get("endpoint_id")
        if not isinstance(endpoint_id, str) or not endpoint_id:
            fail(f"{rel} missing string field: endpoint_id")
        endpoint_ids.add(endpoint_id)

    return endpoint_ids


def main() -> None:
    if not FIXTURE_DIR.is_dir():
        fail(f"missing fixture directory: {FIXTURE_DIR.relative_to(ROOT)}")

    endpoint_ids = load_endpoint_ids()
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
        if FORBIDDEN_PORT_TOKEN_RE.search(raw):
            fail(f"{rel} contains forbidden text: port")

        try:
            fixture = json.loads(raw)
        except json.JSONDecodeError as exc:
            fail(f"{rel} is invalid JSON: {exc}")

        if not isinstance(fixture, dict):
            fail(f"{rel} must contain a JSON object")

        missing = sorted(REQUIRED_FIELDS - set(fixture))
        if missing:
            fail(f"{rel} missing fields: {', '.join(missing)}")

        if fixture["schema_version"] != "net.transport.adapter.v1":
            fail(f"{rel} invalid schema_version: {fixture['schema_version']}")

        if fixture["transport_kind"] not in ALLOWED_KINDS:
            fail(f"{rel} invalid transport_kind: {fixture['transport_kind']}")

        if fixture["transport_scope"] not in ALLOWED_SCOPES:
            fail(f"{rel} invalid transport_scope: {fixture['transport_scope']}")

        if fixture["transport_state"] not in ALLOWED_STATES:
            fail(f"{rel} invalid transport_state: {fixture['transport_state']}")

        if fixture["transport_security"] not in ALLOWED_SECURITY:
            fail(f"{rel} invalid transport_security: {fixture['transport_security']}")

        declared_at = fixture["declared_at_unix_ms"]
        if not isinstance(declared_at, int) or isinstance(declared_at, bool) or declared_at < 0:
            fail(f"{rel}: declared_at_unix_ms must be integer >= 0")

        if not isinstance(fixture["metadata"], dict):
            fail(f"{rel}: metadata must be an object")

        endpoint_id = fixture["endpoint_id"]
        if endpoint_id not in endpoint_ids:
            fail(f"{rel}: endpoint_id does not resolve to endpoint fixture id: {endpoint_id}")

    print("check-net-transport-fixtures: ok")


if __name__ == "__main__":
    main()
