# ADR 0026: Non-Process Carrier Skeletons

Status: accepted
Date: 2026-05-25

## Context

SPINE.33F made carrier families visible in the coverage matrix, but planned
families still needed a concrete inspect surface before implementation.

## Decision

Carrier families must become visible skeletons before implementation. A
skeleton carrier is inspectable and receipt-aware, but does not execute effects.

## Consequences

Future operations can bind to an explicit carrier surface before execution is
available. Coverage, inspect output and receipt requirements can be validated
without network, database, git, service, socket, listener, model-provider or
review effects.
