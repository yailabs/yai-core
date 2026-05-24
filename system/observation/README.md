# Observation

Status: active implementation surface
Owner: system/observation
Purpose: Host/external observation posture helpers.
Not source of truth for: enforcement, carrier execution, graph facts or durable record truth

Observation detects host/external state and possible mismatch.
Observation is not enforcement and not carrier execution.

SPINE.33A adds only low-level posture vocabulary. It does not add runtime host
probing, process monitoring, network observation or a dispatch queue.
