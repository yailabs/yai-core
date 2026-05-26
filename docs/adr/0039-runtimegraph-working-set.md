# ADR 0039: RuntimeGraph Working Set

Status: Accepted

## Context

SPINE.41 made durable typed graph relations inspectable. The next step is a
runtime computational shape over those relations for active-case work.

## Decision

RuntimeGraph is introduced as an in-memory working set loaded from graph
relations for active-case computation. It does not replace graph persistence
and does not own durable truth.

The first RuntimeGraph is per-command ephemeral. A resident RuntimeGraph
service remains planned.

## Consequences

YAI can inspect the computational shape of a case graph without claiming
Ladybug integration, HNSW, Context Compiler or durable graph truth ownership.
