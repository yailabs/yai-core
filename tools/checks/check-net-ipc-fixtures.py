#!/usr/bin/env python3
"""Validate NET local IPC channel fixture discipline."""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURE_DIR = ROOT / "proto/fixtures/net/ipc"
ENDPOINT_DIR = ROOT / "proto/fixtures/net/endpoint"
TRANSPORT_DIR = ROOT / "proto/fixtures/net/transport"

EXPECTED_FILES = {
    "abstract-local.json",
    "unix-socket-future.json",
    "named-pipe-future.json",
    "platform-ipc-future.json",
    "memory-channel-future.json",
}

REQUIRED_FIELDS = {
    "schema_version",
    "ipc_channel_id",
    "transport_adapter_id",
    "endpoint_id",
    "channel_label",
    "channel_kind",
    "channel_scope",
    "channel_state",
    "channel_security",
    "channel_ref",
    "metadata",
    "declared_at_unix_ms",
}

ALLOWED_KINDS = {
    "abstract_local",
    "unix_socket_future",
    "named_pipe_future",
    "platform_ipc_future",
    "memory_channel_future",
}

ALLOWED_SCOPES = {
    "process",
    "machine",
    "session",
    "future",
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
    "same_user_future",
    "declared_local",
    "untrusted",
}

FORBIDDEN_TEXT = (
    "hostname",
    "username",
    "ip_address",
    "mac_address",
    "serial",
    "/users/",
    "/home/",
    "/tmp",
    "/var",
    "/run",
    "secret",
    "password",
    "token",
    "api_key",
    "private_key",
    "process_id",
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
FORBIDDEN_PID_TOKEN_RE = re.compile(r"(^|[^A-Za-z0-9_])pid([^A-Za-z0-9_]|$)", re.IGNORECASE)


def fail(message: str) -> None:
    print(f"check-net-ipc-fixtures: {message}", file=sys.stderr)
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


def validate_safety(path: Path, raw: str) -> None:
    rel = path.relative_to(ROOT)
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
    if FORBIDDEN_PID_TOKEN_RE.search(raw):
        fail(f"{rel} contains forbidden text: pid")


def main() -> None:
    if not FIXTURE_DIR.is_dir():
        fail(f"missing fixture directory: {FIXTURE_DIR.relative_to(ROOT)}")

    endpoint_ids = load_ids(ENDPOINT_DIR, "endpoint_id")
    transport_ids = load_ids(TRANSPORT_DIR, "transport_adapter_id")
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
        validate_safety(path, raw)

        try:
            fixture = json.loads(raw)
        except json.JSONDecodeError as exc:
            fail(f"{rel} is invalid JSON: {exc}")

        if not isinstance(fixture, dict):
            fail(f"{rel} must contain a JSON object")

        missing = sorted(REQUIRED_FIELDS - set(fixture))
        if missing:
            fail(f"{rel} missing fields: {', '.join(missing)}")

        if fixture["schema_version"] != "net.local_ipc.channel.v1":
            fail(f"{rel} invalid schema_version: {fixture['schema_version']}")

        if fixture["channel_kind"] not in ALLOWED_KINDS:
            fail(f"{rel} invalid channel_kind: {fixture['channel_kind']}")

        if fixture["channel_scope"] not in ALLOWED_SCOPES:
            fail(f"{rel} invalid channel_scope: {fixture['channel_scope']}")

        if fixture["channel_state"] not in ALLOWED_STATES:
            fail(f"{rel} invalid channel_state: {fixture['channel_state']}")

        if fixture["channel_security"] not in ALLOWED_SECURITY:
            fail(f"{rel} invalid channel_security: {fixture['channel_security']}")

        declared_at = fixture["declared_at_unix_ms"]
        if not isinstance(declared_at, int) or isinstance(declared_at, bool) or declared_at < 0:
            fail(f"{rel}: declared_at_unix_ms must be integer >= 0")

        if not isinstance(fixture["metadata"], dict):
            fail(f"{rel}: metadata must be an object")

        endpoint_id = fixture["endpoint_id"]
        if endpoint_id not in endpoint_ids:
            fail(f"{rel}: endpoint_id does not resolve to endpoint fixture id: {endpoint_id}")

        transport_adapter_id = fixture["transport_adapter_id"]
        if transport_adapter_id not in transport_ids:
            fail(
                f"{rel}: transport_adapter_id does not resolve to transport "
                f"fixture id: {transport_adapter_id}"
            )

    print("check-net-ipc-fixtures: ok")


if __name__ == "__main__":
    main()
