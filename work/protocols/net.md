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
