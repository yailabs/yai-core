Historical/superseded engineering record. Not an active source of truth.

# NEW.18B Live Case Context / Ref Boundary

Status: implemented as live case context boundary.

NEW.18B introduces explicit case context/session semantics while preserving refs
for persistence, graph, IPC and audit.

## Implemented Surface

```text
include/yai/case/case_context.h
include/yai/case/case_session.h
system/case/case_context.c
system/case/case_session.c
tests/smoke/case-context/test_case_context.c
```

`case_context` is materialized from a journal. It counts case-world records,
subject bindings, projection rules, authority scopes, projections, receipts,
memory and graph edges for a case. `case_session` wraps the journal backing
path and active case context.

No persistent record kinds were added. Sessions remain ephemeral for now.

## Runtime Meaning

```text
case_ref = persistent identity
journal_path = case_session backing store
case_world = materialized domains/attachments/bindings/subjects/authority/projection posture
case_context = live runtime operating surface
case_session = daemon/CLI active case instance
```

## CLI Output

`yai case enter`, `yai case attach-provider`, prompt dry-run and interactive
prompt output expose:

```text
case_session: active
case_world: materialized
case_context: active
raw_journal_access: not_provided
filesystem_access: not_provided
authority_scope: model interpretation_only
```

This does not remove refs. It stops presenting refs as the whole active case.

## Validation

`smoke-new18b` proves:

```text
case_session:active
case_world:loaded
case_context:active
domain:filesystem_sandbox
attachment:filesystem_sandbox bound
authority:model interpretation_only
authority:terminal display_only
filesystem loop completed
records persisted with case_ref
```

NEW.18B is intentionally not NEW.19. It does not realign global guards beyond
adding the new smoke target and source files.
