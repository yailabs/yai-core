# Interaction Thread Model

Status: NEW.18C interaction thread doctrine.

An interaction thread is a case-bound conversation or interaction lane used by
a participant such as a terminal, model, operator or harness.

Core rule:

```text
journal = replay/audit
interaction_thread = selected conversational lane
participant_view_frame = model-visible assembled context for a turn
projection = case cognitive view
transcript = audit material, not automatic model context
```

The active model context is not the whole journal. It is assembled from:

```text
current case projection
active interaction thread selected turns
relevant memory/projection delta
authority scope
```

## Terms

`interaction_thread`
: Case-bound lane for conversational or interaction turns.

`interaction_turn`
: One user/model/system exchange or event inside a thread.

`participant_view_frame`
: The model, operator or terminal-visible context assembled for a specific
turn.

`transcript`
: Audit persistence of prompt/output material, redacted or full depending
mode.

`model_interpretation`
: Model output interpreted as non-authoritative case material.

## Lifecycle

```text
case_session
-> active_interaction_thread
-> participant_view_frame
-> model prompt
-> model_interpretation
-> turn / receipt / audit record
```

Starting a new thread resets the conversational lane sent to the model. It does
not delete case history, audit residue, receipts, graph, memory or prior
threads.

## Journal Posture

NEW.18C remains journal-backed for persistence. That is a bootstrap posture.
The mature target is:

```text
hot state      = live active session/thread/frame
record store   = durable lookup
journal        = append-only replay/audit/debug/rebuild
projection     = current cognitive case view
memory         = operational experience derived from residue
```

The journal must not be treated as raw chat memory.
