# ADR 0023: Process Carrier Signal Control

Status: accepted
Owner: control/carrier substrate
Purpose: Decide how process signals enter YAI carrier control.
Not source of truth for: host observation probes or process workflow APIs

## Decision

YAI process effects must be mediated through process carrier v0.
SIGTERM/SIGKILL are controlled effects, not shell commands. Real signal tests
are limited to test-owned processes.

## Consequences

The process carrier declares `carrier.v1`, observes pre/post state, produces
receipt posture and refuses arbitrary host process killing. Future process
control can build on this path instead of bypassing dispatch/gate/receipt.
