# Integration Modes

YAI supports six integration/control modes. The same primitives can exist
in every mode, but the guarantee is different. Product, projection and audit
language must preserve the mode label.

## Mode Matrix

| Mode | What YAI can claim | What YAI cannot claim | Required protocol | Receipt expectation | Failure behavior |
|---|---|---|---|---|---|
| `observed` | observed, recorded, reconstructed | prevented, blocked, enforced | ingest observation or external receipt import | observed receipt, imported receipt or missing-receipt divergence | mark untrusted observation, incomplete binding or claim without receipt |
| `advisory` | recommended, warned, explained | blocked, enforced, executed | operation attempt plus advisory control decision | optional receipt or observation | record advisory-only decision and avoid enforcement claim |
| `interposed` | decided before forwarding effect request | real effect unless receipt returns | operation attempt and control decision before external executor | external receipt returned to YAI | mark bypass, missing receipt or observed divergence |
| `enforced` | host/carrier is expected to honor deny/constraint | bypass impossible outside declared boundary | control decision plus host/carrier enforcement contract | enforcement receipt or host acknowledgement | fail closed or record enforcement breach |
| `carrier_owned` | executed or blocked through YAI carrier | external bypass prevention outside carrier | operation attempt, control decision, carrier request, receipt | YAI carrier receipt | record partial carrier failure or subject-state mismatch |
| `embedded` | host-integrated decision and receipt path | control outside declared embed boundary | embedded host calls YAI decision/receipt protocol | host-integrated receipt | mark host boundary breach, missing receipt or stale embed posture |

## Rule

Observed and advisory modes are commercially useful but cannot claim prevention.
Carrier-owned and embedded modes produce the strongest guarantees, but only
inside their declared boundary.

## Flow Sketches

Observed:

```text
external effect
-> observation / external receipt import
-> subject binding when possible
-> record / graph / memory candidate
-> projection / divergence report
```

Interposed:

```text
attempt
-> control gate
-> allow / deny / defer / review
-> external executor
-> receipt import
```

Carrier-owned:

```text
attempt
-> control decision
-> YAI carrier executes or blocks
-> receipt
-> store / graph / memory / projection
```

Embedded:

```text
host request
-> YAI control decision inside host boundary
-> host action or block
-> embedded receipt
-> store / graph / reconcile
```
