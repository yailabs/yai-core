# ADR 0022: Carrier Contract v1

Status: accepted
Owner: control/carrier substrate
Purpose: Decide the common contract carriers must expose before trusted effects.
Not source of truth for: process carrier v0 or signal control

## Decision

YAI carriers must implement a common contract before they can produce trusted
effects. Filesystem is the first carrier.v1 adapter.

## Context

SPINE.33A defined carrier vocabulary. SPINE.33B added dispatch lanes and route
plans. SPINE.33C defines the carrier contract so a routed family cannot become
an ad hoc free tool invocation.

## Consequences

The filesystem carrier declares `carrier.v1`, exposes receipt-required posture
and maps existing read/write behavior to carrier outcomes. Future process,
network, database, repository and model carriers must use the same contract
before they execute effects.
