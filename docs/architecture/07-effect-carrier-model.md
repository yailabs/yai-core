# Effect And Carrier Model

An effect is the operational consequence attempted, executed, observed or imported.

A carrier is the controlled mechanism that can execute, observe or import evidence for an effect.

## Carrier Responsibilities

```text
carrier registry
carrier request
execution or observation
result normalization
receipt production
external receipt import
failure classification
```

## Initial Carriers

```text
null
filesystem
process
repository
model
network
database
event
external_receipt
```

## Receipt Rule

No effect path is complete until it has a receipt, a missing-receipt divergence, or an explicit observe-only record.

## Boundary

Carriers do not own policy. Control decides. Carriers execute or observe. Store persists receipts. Graph reconstructs relationships.
