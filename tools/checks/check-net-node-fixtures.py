#!/usr/bin/env python3
"""Validate NET node identity fixture discipline."""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURE_DIR = ROOT / "proto/fixtures/net/node"

EXPECTED_FILES = {
    "local-process.json",
    "localhost-service.json",
    "lan-node.json",
    "remote-node.json",
    "external-node.json",
}

REQUIRED_FIELDS = {
    "schema_version",
    "node_id",
    "node_label",
    "node_kind",
    "node_scope",
    "node_state",
    "identity_source",
    "safe_metadata",
    "observed_at_unix_ms",
    "endpoint_refs",
    "capability_refs",
}

ALLOWED_KINDS = {
    "local",
    "localhost_service",
    "lan",
    "remote",
    "external",
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
    "observed",
    "healthy",
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

FORBIDDEN_IP_TOKEN_RE = re.compile(r"(^|[^A-Za-z0-9_])ip([^A-Za-z0-9_]|$)", re.IGNORECASE)
FORBIDDEN_MAC_TOKEN_RE = re.compile(r"(^|[^A-Za-z0-9_])mac([^A-Za-z0-9_]|$)", re.IGNORECASE)
FORBIDDEN_HOME_TOKEN_RE = re.compile(r"(^|[^A-Za-z0-9_])home([^A-Za-z0-9_]|$)", re.IGNORECASE)


def fail(message: str) -> None:
    print(f"check-net-node-fixtures: {message}", file=sys.stderr)
    raise SystemExit(1)


def require_list(value: object, field: str, path: Path) -> None:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        fail(f"{path}: {field} must be a list of strings")


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

        if fixture["node_kind"] not in ALLOWED_KINDS:
            fail(f"{rel} invalid node_kind: {fixture['node_kind']}")

        if fixture["node_scope"] not in ALLOWED_SCOPES:
            fail(f"{rel} invalid node_scope: {fixture['node_scope']}")

        if fixture["node_state"] not in ALLOWED_STATES:
            fail(f"{rel} invalid node_state: {fixture['node_state']}")

        observed_at = fixture["observed_at_unix_ms"]
        if not isinstance(observed_at, int) or isinstance(observed_at, bool) or observed_at < 0:
            fail(f"{rel}: observed_at_unix_ms must be integer >= 0")

        if not isinstance(fixture["safe_metadata"], dict):
            fail(f"{rel}: safe_metadata must be an object")

        require_list(fixture["endpoint_refs"], "endpoint_refs", rel)
        require_list(fixture["capability_refs"], "capability_refs", rel)

    print("check-net-node-fixtures: ok")


if __name__ == "__main__":
    main()
