# Wedges

Initial wedges must be close enough to test and broad enough to prove the model.
All wedges start from `yai-core` as the control core. `interfaces` packages the
projection/API/SDK boundary around the wedge, and `console` provides operator
inspection. The old `yai` repo is source material for environment scenarios, not
the product root.

## Agentic Coding / Local AI Workstation

Subjects:

```text
repository
file
directory
process
model
```

Value: control file writes, process execution, model calls and memory use in a case-bound local environment.

## AI Workflow Governance

Subjects:

```text
workflow
job
tool
HTTP service
artifact
```

Value: observe or interpose existing workflows without replacing the workflow engine.

## Infrastructure / Kubernetes

Subjects:

```text
namespace
deployment
pod
configmap
sensitive config object
service
```

Value: connect rollout, admission, observed state and incident memory through receipts and reconciliation.

## Commercial Order

```text
Core Developer / Local
Integration Kit
Operational Memory / Audit Pack
Professional / Team
Enterprise / Embedded
Platform / Cloud later
```
