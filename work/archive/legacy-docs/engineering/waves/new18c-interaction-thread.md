Historical/superseded engineering record. Not an active source of truth.

# NEW.18C Interaction Thread / Participant View Boundary

Status: current implementation wave.

NEW.18C separates audit transcript, active interaction thread, participant view
frame, case projection and journal residue.

## Implemented Boundary

Record kinds:

```text
interaction_thread
interaction_turn
participant_view_frame
```

Runtime objects:

```text
include/yai/case/interaction_thread.h
include/yai/case/participant_view.h
system/case/interaction_thread.c
system/case/participant_view.c
```

CLI prompt behavior now treats the model context as:

```text
current case projection
active interaction thread selected turns
authority and redaction posture
memory/projection summary
```

It does not implicitly replay every journal `model_interpretation` as chat
history.

## Commands

```text
/thread status
/thread new [label]
/thread list
/thread use <thread_id>
/thread archive <thread_id>
/refresh
/transcript status
/transcript on
/transcript off
/memory propose ...
/exit
```

`/thread new` creates a new selected lane without deleting journal/audit
history. `/refresh` rebuilds the participant view from active thread plus case
context. Transcript mode affects audit completeness, not authority.

## Observability / Freshness Impact

```text
thread switch invalidates participant_view_frame
new turn creates participant view delta
transcript mode affects audit completeness, not authority
projection freshness must be checked before model prompt
```

## Non-goals

```text
do not delete journal
do not treat transcript as source of truth
do not create a chat root module
do not implement LMDB/shared memory
do not implement a model carrier
do not implement an agent harness
```
