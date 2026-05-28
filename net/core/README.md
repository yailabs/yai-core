# core

Owns future shared NET primitives used across module areas.

Must not own YAI policy, case authority, RuntimeGraph mutation, journal writes
or memory writes.

Current status: C translation unit scaffold started.

`net/core/net.c` provides pure component metadata helpers only. It does not
implement runtime behavior.

Next expected wave: NET.SPINE.2 stream envelope types.
