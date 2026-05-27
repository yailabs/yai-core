# NET Protocol Vocabulary

Canonical internal vocabulary for NET.SPINE.1.

YAI controls authority.
NET moves streams.
External nodes execute capabilities.

CLORI extension:
YAI controls authority.
NET moves streams.
CLORI executes neural computation.

## Terms

- net module: Integrated YAI runtime communication module. It owns stream movement surfaces; it does not own YAI authority, policy or execution eligibility.
- node: A local or external execution participant addressable by NET vocabulary. A node advertises capability; it does not grant YAI authority.
- local node: Node projected inside the local YAI process boundary. It does not imply external service lifecycle ownership.
- localhost service: Service reachable through loopback posture. It is not a production transport contract or approval path.
- LAN node: Node projected on a local network. It is not discovered until a later discovery wave.
- remote node: Node projected beyond localhost/LAN posture. It is not trusted by placement alone.
- external node: Node outside YAI that may execute advertised capabilities. It does not become part of YAI authority.
- endpoint: Addressable contact surface for a node. An endpoint does not approve operations or imply liveness.
- capability: Description of what an external node can do. A capability does not say the operation is allowed.
- stream: Ordered movement channel for invocation events. A stream does not decide policy.
- stream event: Named item in a stream such as request, chunk, metric or receipt. It is transport vocabulary, not authority.
- invocation: Correlated request lifecycle through NET surfaces. It is not a YAI decision record by itself.
- route: Selected communication path candidate. A route selects a communication path; it does not approve the operation.
- health: Observed service or node condition. Health does not authorize use.
- readiness: Condition that a node or endpoint may accept work. Readiness is not policy approval.
- liveness: Condition that a node or endpoint appears alive. Liveness is not readiness and not authority.
- trace: Correlation vocabulary for stream movement and boundaries. It is not a tracing backend.
- receipt transport: Movement of receipt material through NET. A receipt can be transported by NET; NET does not make it authoritative.
- metric: Measurement vocabulary for NET movement and waiting. A metric is not a control decision.
- transport adapter: Future adapter shape for a communication path. It does not implement sockets, servers or discovery in this wave.
- NET-compatible node: External node target that can speak the NET contract surface. Compatibility does not grant trust.
- CLORI-compatible node target: External neural computation target aligned with CLORI compatibility. CLORI remains external and is not vendored into YAI.
