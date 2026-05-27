# NET Protocol Vocabulary

Canonical internal vocabulary for NET.

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

## Stream Envelope

A stream envelope carries event material through NET stream surfaces. A stream
envelope does not approve an operation. A receipt event carried by NET is not
made authoritative by NET. Ordering is represented by sequence number, not by
filesystem order. Transport adapters may carry stream envelopes later, but this
wave does not implement adapters.

- stream envelope: Versioned event container for stream movement. It is not a policy decision.
- stream id: Stable stream correlation id. It does not imply storage ownership.
- invocation id: Correlation id for an invocation lifecycle. It is not authorization.
- trace id: Correlation id for trace propagation. It is not a tracing backend.
- sequence number: Integer event ordering value within a stream. Filesystem order is not ordering truth.
- stream event kind: One of request, response, chunk, metric, receipt, error or complete. It names event shape, not permission.
- payload kind: One of none, bytes, text, json, metric, receipt or error. It does not define serialization law beyond this scaffold.
- timestamp: Millisecond Unix timestamp supplied as envelope metadata. It is not trust proof.
- origin node: Node id that emitted the envelope. It does not grant authority.
- target node: Node id intended to receive the envelope. It does not prove reachability.
- completion state: Stream lifecycle state such as completed or cancelled. It does not make receipts authoritative.
- error state: Stream lifecycle state or event describing failure. It does not decide recovery policy.

Required event kinds:
request, response, chunk, metric, receipt, error, complete.

Required stream lifecycle:
created, open, half_closed, completed, failed, cancelled.

Stream envelopes use origin_node_id and target_node_id as references to NET
node identities. NET.SPINE.3 does not require a live registry; it only makes
the reference vocabulary explicit.

## Node Identity

A node is a communication participant, not an authority. A node can advertise
capabilities, but capability does not grant permission. A node id is a stable
NET-facing identifier, not necessarily a hostname. A local machine projection is
a safe view, not raw host inventory. NET may represent node health later, but
NET does not approve operations. system/ must not perform LAN discovery
directly. engine/ must not store live endpoint truth as authority. Real
hostnames, IPs, usernames, MAC addresses and hardware serials must not appear in
canonical fixtures.

- node: Communication participant represented by NET. It does not own authority.
- node id: Stable NET-facing identifier for a node. It is not necessarily a hostname.
- node label: Human-readable label for display or fixture clarity. It is not identity proof.
- node kind: One of local, localhost_service, lan, remote or external. It does not imply trust.
- node scope: One of process, machine, lan, remote or external. It does not imply discovery.
- node status: One of unknown, declared, observed, healthy, degraded, unavailable or retired. It does not approve operations.
- node projection: Safe NET-facing view of node identity material. It is not raw machine inventory.
- local machine projection: Safe local machine view with fake or scrubbed metadata. It must not expose hostname, username, IP, MAC or hardware serial data.
- endpoint reference: Identifier reference to future endpoint material. It is not live endpoint truth.
- capability reference: Identifier reference to advertised capability material. It does not grant permission.
- identity source: Description of where the node identity was declared or observed. It is not proof of trust.
- safe metadata: Small generic metadata safe for canonical fixtures. It must not include private machine metadata.
- private machine metadata: Hostnames, IPs, usernames, MAC addresses, hardware serials, real filesystem paths or secrets. It must not appear in canonical NET fixtures.

Required node kinds:
local, localhost_service, lan, remote, external.

Required node scopes:
process, machine, lan, remote, external.

Required node states:
unknown, declared, observed, healthy, degraded, unavailable, retired.

## Capability Advertisement

A capability says what a node can do. A capability does not approve an
operation, grant case authority or resolve policy. Routing may later use
capabilities as input, but routing is not implemented here. YAI remains
responsible for authority and policy decisions. NET may move invocation material
to a capable node later; this wave only creates the advertised vocabulary.

- capability: Describes a node's advertised work surface. It does not approve an operation.
- capability id: Stable NET-facing identifier for a capability advertisement. It is not authority.
- capability kind: One of neural_llm_decode, neural_embedding_encode, metrics_stream, receipt_emit or generic_external.
- capability family: Broad grouping such as neural, metrics, receipt or generic. It does not imply routing.
- capability scope: Where the advertised capability applies: process, node, service or external.
- capability state: One of unknown, declared, available, degraded, unavailable or retired. It is not policy approval.
- advertised capability: Capability material associated with a node id. It does not grant permission.
- capability constraints: Safe limits or requirements advertised with the capability. They do not replace YAI policy.
- capability metadata: Safe descriptive metadata for compatibility and notes. It must not claim production implementation.
- node capability reference: Node fixture reference to a capability id. It is not live registry resolution.
- registry seed: Static protocol fixture material for future registry work. It is not a runtime registry.
- compatibility tag: Descriptive tag for compatibility posture. It does not vendor or certify an external implementation.

Required capability kinds:
neural_llm_decode, neural_embedding_encode, metrics_stream, receipt_emit, generic_external.

Required capability families:
neural, metrics, receipt, generic.

Required capability scopes:
process, node, service, external.

Required capability states:
unknown, declared, available, degraded, unavailable, retired.

## Endpoint Descriptor

An endpoint is a communication address surface, not an authority. An endpoint
descriptor may be declared before it is observed. NET may later route through
endpoints, but this wave does not route. NET may later probe endpoints, but
this wave does not probe. Endpoint availability does not approve any YAI
operation. Endpoint metadata must not expose secrets or real private machine
identity in canonical fixtures.

- endpoint: Communication address surface associated with a node. It does not approve operations.
- endpoint id: Stable NET-facing identifier for an endpoint descriptor. It is not proof of reachability.
- endpoint kind: One of local_process, localhost_http, local_ipc, lan_http, remote_http or future_transport.
- endpoint scope: One of process, machine, lan, remote or external. It does not imply discovery.
- endpoint state: One of unknown, declared, available, degraded, unavailable or retired. It is not policy approval.
- endpoint security: One of unknown, local_only, trusted_local, declared_remote or external_untrusted. It is not trust authority.
- address reference: Abstract address surface such as local-process://demo. It must not expose real host, port, path or private machine data.
- local endpoint: Process-local endpoint declaration. It is not service lifecycle ownership.
- localhost endpoint: Machine-local loopback posture declaration. It does not implement HTTP in this wave.
- LAN endpoint: LAN posture declaration. It is not discovered in this wave.
- remote endpoint: Remote posture declaration. It is not trusted by placement alone.
- external endpoint: External node endpoint declaration. It does not become part of YAI authority.
- declared endpoint: Endpoint material recorded as a static seed before observation or probing.
- endpoint registry seed: Static fixture material for future registry work. It is not a live runtime registry.

Required endpoint kinds:
local_process, localhost_http, local_ipc, lan_http, remote_http, future_transport.

Required endpoint scopes:
process, machine, lan, remote, external.

Required endpoint states:
unknown, declared, available, degraded, unavailable, retired.

Required endpoint security values:
unknown, local_only, trusted_local, declared_remote, external_untrusted.
