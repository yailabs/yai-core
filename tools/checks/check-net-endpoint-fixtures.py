#!/usr/bin/env python3
"""Validate NET endpoint descriptor fixture discipline."""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURE_DIR = ROOT / "proto/fixtures/net/endpoint"
NODE_DIR = ROOT / "proto/fixtures/net/node"
CAPABILITY_DIR = ROOT / "proto/fixtures/net/capability"

EXPECTED_FILES = {
    "local-process.json",
    "localhost-http.json",
    "local-ipc.json",
    "lan-http.json",
    "remote-http.json",
    "future-transport.json",
}

REQUIRED_FIELDS = {
    "schema_version",
    "endpoint_id",
    "node_id",
    "endpoint_label",
    "endpoint_kind",
    "endpoint_scope",
    "endpoint_state",
    "endpoint_security",
    "address_ref",
    "metadata",
    "declared_at_unix_ms",
    "capability_refs",
}

ALLOWED_KINDS = {
    "local_process",
    "localhost_http",
    "local_ipc",
    "lan_http",
    "remote_http",
    "future_transport",
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


def fail(message: str) -> None:
    print(f"check-net-endpoint-fixtures: {message}", file=sys.stderr)
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

        if fixture["endpoint_kind"] not in ALLOWED_KINDS:
            fail(f"{rel} invalid endpoint_kind: {fixture['endpoint_kind']}")

        if fixture["endpoint_scope"] not in ALLOWED_SCOPES:
            fail(f"{rel} invalid endpoint_scope: {fixture['endpoint_scope']}")

        if fixture["endpoint_state"] not in ALLOWED_STATES:
            fail(f"{rel} invalid endpoint_state: {fixture['endpoint_state']}")

        if fixture["endpoint_security"] not in ALLOWED_SECURITY:
            fail(f"{rel} invalid endpoint_security: {fixture['endpoint_security']}")

        declared_at = fixture["declared_at_unix_ms"]
        if not isinstance(declared_at, int) or isinstance(declared_at, bool) or declared_at < 0:
            fail(f"{rel}: declared_at_unix_ms must be integer >= 0")

        if not isinstance(fixture["metadata"], dict):
            fail(f"{rel}: metadata must be an object")

        capability_refs = fixture["capability_refs"]
        if not isinstance(capability_refs, list):
            fail(f"{rel}: capability_refs must be a list")
        if not all(isinstance(item, str) for item in capability_refs):
            fail(f"{rel}: capability_refs must contain strings")

        node_id = fixture["node_id"]
        if node_id not in node_ids:
            fail(f"{rel}: node_id does not resolve to node fixture id: {node_id}")

        for capability_id in capability_refs:
            if capability_id not in capability_ids:
                fail(
                    f"{rel}: capability_refs entry does not resolve to "
                    f"capability fixture id: {capability_id}"
                )

    print("check-net-endpoint-fixtures: ok")


if __name__ == "__main__":
    main()
