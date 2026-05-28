# Implementation Gap List

## P0 gaps before Live Projection Frame Schema

- CaseHandle not formalized.
- SubjectHandle not formalized.
- CapabilityLease not formalized.
- ResourceScope not formalized.
- VisibilityScope / AuthorityScope split incomplete.
- Case generation snapshot not formalized.
- Authority records exist, but resolved AuthorityScope does not.
- Subject and case bindings declare relations but do not grant capability.
- Provider/model refs and attachments do not create a provider capability lease.
- Raw journal/record access is not separated from model/operator visibility by a resolved scope.
- Review authority is local-dev/displayed and not resolved through AuthorityScope.

## P1 gaps before Context Compiler

- ContextFrame source binding.
- ContextDelta invalidation.
- CaseModelSession binding.
- Retrieval provider scope gate.
- Projection freshness is count/summary-oriented and not yet derived from case generation snapshots.
- Memory candidate inclusion is not governed by VisibilityScope.
- Query/read access is not scope-gated for model context.

## P2 gaps before Model Runtime

- Runner cache/KV boundary.
- Provider/runtime capability binding.
- Provider endpoint/LAN target ResourceScope.
- Model/provider selection admissibility.
- Provider runtime receipt/proof contract.
- Advanced memory/retrieval quality inputs.

## Current Enforcement Gaps

- Filesystem sandbox enforcement is concrete, but there is no reusable
  ResourceScope object.
- Gate/decision enforcement exists for attempts, but carriers do not consume a
  CapabilityLease.
- Review approve performs execution; deny/defer/quarantine block execution.
  The reviewer authority itself is not formally resolved.
- `authority_scope` records are visible in records/projections/facts, but not
  enforced as a scope object.
- Projection/model visibility is described through projection records and
  redaction summaries, but not compiled from VisibilityScope.
