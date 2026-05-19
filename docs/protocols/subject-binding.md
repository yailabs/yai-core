# Subject Binding Protocol

Purpose: bind an observable, controllable, invokable, queryable or targetable
entity to a case.

Subject binding is the protocol surface for
[04-subject-model.md](../architecture/04-subject-model.md). It does not create a
generic object database and it does not claim complete external truth.

## Fields

```text
subject_ref
case_ref
subject_kind
locator
binding_mode
control_mode
trust_class
observability
allowed_effects
forbidden_effects
policy_refs
carrier_refs
receipt_obligations
projection_permissions
current_state_ref
last_observed_at
```

## Binding Modes

```text
attached
observed
interposed
carrier_owned
embedded
external
```

## Control Modes

```text
observed
advisory
interposed
enforced
carrier_owned
embedded
```

## Binding Vs Control Matrix

| Binding mode | Allowed control modes | What YAI can claim |
|---|---|---|
| `attached` | `observed`, `advisory` | subject is known to the case |
| `observed` | `observed`, `advisory` | subject activity/state can be recorded |
| `interposed` | `interposed`, `enforced` | YAI decides before forwarding effect request |
| `carrier_owned` | `carrier_owned`, `enforced` | YAI carrier executes or blocks effect |
| `embedded` | `embedded`, `enforced` | host embeds YAI decision/receipt protocol |
| `external` | `observed`, `advisory` | YAI imports external claims or receipts |

## Identity And State Rules

```text
subject identity is not locator
locator is an access/address hint
subject state is latest known case-scoped posture
subject state is not complete external truth
external claims remain claims until bound with provenance
```

## Rule

Case-bound subjects are the operational world of the case. Operations target
subjects and receipts update subject state.
