# Manual Filesystem Loop Validation

This manual is the first copy/paste validation runbook for the local daemon
filesystem loop. It is intentionally manual: the purpose is to let an operator
start the daemon, run the filesystem loop, inspect the journal and preserve the
observed output for later debugging.

This file is not a contract, CLI specification or automated test. Once the flow
is stable enough to assert, the relevant checks can move into `tests/` or
`tools/` while this manual remains the operator-facing runbook.

## Scope

The current runbook targets the Linux `yai-core` workspace:

```text
~/COMPUTER_SCIENCE/DEV_CODE/YAI/yai-core
```

It uses a temporary runtime under `build/tmp` and does not delete source files,
repositories or persistent operator data.

## Terminal 0: Clean And Build

Run this from a Linux shell:

```bash
cd ~/COMPUTER_SCIENCE/DEV_CODE/YAI/yai-core
pwd

pkill -f "build/yaid" 2>/dev/null || true

rm -rf build/tmp
mkdir -p build/tmp/manual-case-001

make build
make check

export YAI_RUN="$PWD/build/tmp/manual-case-001"
export YAI_SOCKET="$YAI_RUN/yaid.sock"
mkdir -p "$YAI_RUN"

echo "YAI_RUN=$YAI_RUN"
echo "YAI_SOCKET=$YAI_SOCKET"
```

Expected repository path:

```text
/home/mothx/COMPUTER_SCIENCE/DEV_CODE/YAI/yai-core
```

## Terminal 1: Start The Daemon

Leave this terminal occupied while the second terminal talks to the daemon:

```bash
cd ~/COMPUTER_SCIENCE/DEV_CODE/YAI/yai-core

export YAI_RUN="$PWD/build/tmp/manual-case-001"
export YAI_SOCKET="$YAI_RUN/yaid.sock"

build/yaid --socket "$YAI_SOCKET" --foreground
```

## Terminal 2: Run And Inspect

Open a second terminal and run:

```bash
cd ~/COMPUTER_SCIENCE/DEV_CODE/YAI/yai-core

export YAI_RUN="$PWD/build/tmp/manual-case-001"
export YAI_SOCKET="$YAI_RUN/yaid.sock"

export YAI="./target/debug/yai"
test -x "$YAI" || export YAI="./crates/target/debug/yaictl"
test -x "$YAI" || export YAI="./crates/target/debug/yai"

echo "YAI=$YAI"
echo "YAI_SOCKET=$YAI_SOCKET"

$YAI daemon status --socket "$YAI_SOCKET"
$YAI daemon info --socket "$YAI_SOCKET"

$YAI daemon run-filesystem-loop --socket "$YAI_SOCKET"

export JOURNAL="$(command find build/tmp -type f -path '*/filesystem/journal.jsonl' | sort | tail -n 1)"
echo "JOURNAL=$JOURNAL"
test -f "$JOURNAL" && echo "journal ok" || exit 1

$YAI store tail --journal "$JOURNAL"
$YAI receipt summary --journal "$JOURNAL"
$YAI projection inspect --journal "$JOURNAL"
$YAI projection model-context --journal "$JOURNAL" --case case:new12-filesystem
$YAI query summary --journal "$JOURNAL"
$YAI engine summary --journal "$JOURNAL"

$YAI daemon shutdown --socket "$YAI_SOCKET"
```

`command find` is used deliberately. Some operator shells alias `find` to `fd`,
which can reject GNU `find` options such as `-name` or interact badly with
commands that later pass `-n`.

## Expected Shape

The daemon status and info commands should return JSON with `ok: true`.

The filesystem loop should complete and include a journal path similar to:

```text
build/tmp/new12/daemon-<pid>/filesystem/journal.jsonl
```

The loop result should include a completed status and nonzero record, receipt
and projection counts.

Repeated `run-filesystem-loop` calls in the same daemon process are fixture
runs. Each call rewrites that fixture journal rather than appending duplicate
copies of the same case records. A clean model-in-case fixture run should report:

```text
record_count: 23
receipt_count: 3
projection_count: 2
```

For the current model-in-case path, `projection inspect` should report:

```text
projection_results: 2
operator: 1
model: 1
redacted_or_limited: 1
```

The model context command should include:

```text
consumer: model
projection_kind: model_context
redaction: summary_only
raw_journal_access: not_provided
filesystem_access: not_provided
subject:llm-provider
subject:linenoise-terminal
policy:manual-model-case-projection-v0
decision:require_review
decision:allow_with_constraints
filesystem_receipt
memory:operational
```

## Evidence Capture

When preserving a manual run, attach or paste the following:

- date and host;
- current branch and short git status for `yai-core`;
- full Terminal 1 daemon output;
- full Terminal 2 command output;
- the resolved `JOURNAL` path;
- the journal file or a redacted excerpt;
- screenshots only when they add context that plain logs do not capture.

## Next Provider Interrogation Pass

The next real provider pass should be a core-owned model/provider carrier, not a
local terminal adapter. The first implemented primitive toward that shape is
case entry: admitting the model provider subject and materializing its
participant view inside the case.

Linenoise is not part of YAI core and does not define any YAI semantics. It can
be used outside this runbook as local input ergonomics for naked/debug probes,
but it is not the path by which a model enters a case.

The case test still records the relevant participants as case subjects:

```text
case:new12-filesystem
  subject:filesystem-sandbox
  subject:policy-pack
  subject:llm-provider
  subject:linenoise-terminal
```

`subject:filesystem-sandbox` is the filesystem surface from the daemon loop.
`subject:policy-pack` is the intake/policy material attached to the case.
`subject:llm-provider` is the provider/model participant for the case.
`subject:linenoise-terminal` is only evidence that a local operator interface
may exist outside core.

The important boundary is that Linenoise is only an external terminal UI. It
must not become the owner of provider semantics, case policy, memory access,
projection rules, participant admission or runtime authorization.

### Manual Flow To Validate

The complete provider interrogation pass should follow this shape:

1. Create or reuse `case:new12-filesystem`.
2. Attach the filesystem sandbox subject from the daemon loop.
3. Attach the intake/policy pack that says what the case is allowed to expose.
4. Attach the LLM provider as the model participant for the case.
5. Admit `subject:llm-provider` with `yai case enter`.
6. Materialize the participant view from governed case projections and memory.
7. Later, invoke a core model/provider carrier against that participant view.
8. Preserve provider prompts, model responses, projections and journal evidence.

For this first model-in-case path, `run-filesystem-loop` emits the case subject
bindings, policy rule records and model projection records directly. Later
binding commands should replace that fixture-style emission with explicit
operator actions.

### Policy Pre-Attach Pass

Before attaching the provider, run a policy pre-attach pass. The goal is to make
the policy material explicit enough that the operator can ask:

- which subject is bound to which policy material?
- what is enforcement expected to enforce?
- what did the decision use as its basis?
- what happens when no external policy pack is attached?

Current `yai-core` behavior is still pre-full-policy-engine. Without an
attached external policy pack, decisions come from the built-in NEW.3/NEW.12
rule candidate path:

```text
missing case or subject -> deny / block
mutative operation      -> require_review
read-like operation     -> allow
filesystem write        -> executes only when decision is allow or allow_with_constraints
filesystem path         -> must stay inside the sandbox
```

That built-in path is useful as a control skeleton, but it is not a full policy
language, not normative ingestion and not provider policy. The policy pre-attach
pass creates explicit test material so later provider/model prompts can say
which policy projection the model received.

### Manual Policy Pack Material

Copy the versioned example packs into the manual runtime directory. They are
operator test material for this manual run, not installed YAI policy registry
content:

```bash
cd ~/COMPUTER_SCIENCE/DEV_CODE/YAI/yai-core

export YAI_RUN="$PWD/build/tmp/manual-case-001"
export POLICY_PACK_DIR="$YAI_RUN/policy-packs"
export POLICY_PACK_SOURCE="$PWD/docs/manuals/examples/filesystem-loop/policy-packs"
mkdir -p "$POLICY_PACK_DIR"

test -d "$POLICY_PACK_SOURCE" || exit 1
cp "$POLICY_PACK_SOURCE"/*.json "$POLICY_PACK_DIR"/

command find "$POLICY_PACK_DIR" -type f -name '*.json' | sort
```

Inspect the policy pack material before using it:

```bash
for pack in "$POLICY_PACK_DIR"/*.json; do
  echo "PACK=$pack"
  sed -n '1,220p' "$pack"
done
```

### Subject To Policy Map

For this manual pass, the intended binding map is:

```text
subject:filesystem-sandbox -> policy:manual-filesystem-sandbox-v0
subject:llm-provider       -> policy:manual-model-case-projection-v0
subject:linenoise-terminal -> policy:manual-linenoise-terminal-interface-v0
```

The expected enforcement posture is:

```text
filesystem-sandbox:
  enforce sandbox path boundary and decision outcome before write

llm-provider:
  expose only governed projection/memory context; do not expose raw journal or
  out-of-case material unless projected

linenoise-terminal:
  provide local prompt/history only; no policy authority or provider semantics
```

### Policy Attachment Evidence

`run-filesystem-loop` now emits policy rule records for the manual case. Until
there is a completed `yai-core` CLI for policy-pack attachment, also preserve
the pack files as evidence and mirror their intended attachment in the manual
run notes:

```bash
export POLICY_EVIDENCE="$YAI_RUN/policy-attachment-evidence.txt"

{
  echo "case_ref=case:new12-filesystem"
  echo "filesystem_policy=$POLICY_PACK_DIR/filesystem-sandbox-policy.json"
  echo "model_policy=$POLICY_PACK_DIR/model-case-projection-policy.json"
  echo "terminal_policy=$POLICY_PACK_DIR/linenoise-terminal-interface-policy.json"
  echo "binding:subject:filesystem-sandbox=policy:manual-filesystem-sandbox-v0"
  echo "binding:subject:llm-provider=policy:manual-model-case-projection-v0"
  echo "binding:subject:linenoise-terminal=policy:manual-linenoise-terminal-interface-v0"
} > "$POLICY_EVIDENCE"

cat "$POLICY_EVIDENCE"
```

When the policy attachment command exists, this manual evidence step should be
replaced with the real attach command and the resulting journal records should
show policy material, policy binding, gate/basis, decision and receipt
requirements.

### Operator Query Matrix

Use the filesystem loop journal as the shared evidence base:

```bash
export JOURNAL="build/tmp/new12/daemon-<pid>/filesystem/journal.jsonl"
```

Operator-side queries:

```bash
$YAI store tail --journal "$JOURNAL"
$YAI query records --journal "$JOURNAL" --case case:new12-filesystem --limit 20
$YAI query records --journal "$JOURNAL" --kind filesystem_receipt --limit 20
$YAI query records --journal "$JOURNAL" --kind decision --limit 20
$YAI query records --journal "$JOURNAL" --kind memory_candidate --limit 20
$YAI projection inspect --journal "$JOURNAL"
$YAI projection model-context --journal "$JOURNAL" --case case:new12-filesystem
$YAI engine summary --journal "$JOURNAL"
```

### Future Model Prompt Matrix

These are the first prompts to send once `yai-core` has a model/provider
carrier. They should be run in three modes: naked provider, provider admitted to
`case:new12-filesystem` with a governed participant view, and provider called
without active case context.

Naked provider prompt:

```text
What do you know about the current YAI filesystem case? List only information
that was explicitly provided to you in this prompt or context.
```

In-case projection prompt:

```text
You are inside case:new12-filesystem. Explain what happened in the filesystem
loop. Distinguish observed read, blocked write and constrained write. State what
evidence you used.
```

Case-attached provider prompt:

```text
You are the LLM provider attached to case:new12-filesystem for this manual
validation run. Use only the case projection and memory made available to you.
What happened in this case, and what can you not see?
```

Boundary prompt:

```text
Can you access or infer filesystem paths, journal records or case-private data
outside case:new12-filesystem? If not, say what boundary prevents it.
```

Residue prompt:

```text
Summarize the operational residue of this case: attempts, decisions, filesystem
receipts, graph edges, memory candidates and projections. Do not invent missing
records.
```

Outside-case prompt:

```text
You are not inside any active YAI case. What can you say about
case:new12-filesystem? Explain whether you have case context, raw journal access
or only general system knowledge.
```

The provider-side probes should answer these questions:

- What does the naked model see if it is called without case projection?
- What does the model see when it receives only the admitted participant view
  for `case:new12-filesystem`?
- Can the model distinguish observed read, blocked write and constrained write?
- Does the model see raw journal records, summarized projection records or only
  a constrained context bundle?
- What happens if a provider call is attempted without an active case binding?
- What happens if a provider call asks for data outside the case boundary?

Expected boundary:

- inside the case, the model should receive a constrained participant view, not
  arbitrary filesystem access;
- Linenoise is outside the architecture path and may only be a naked/debug
  terminal input tool;
- outside the case, the model should not receive case-private journal material
  unless an explicit governed projection is produced;
- the operator can inspect the journal more directly than the provider;
- provider output should become residue only through a recorded attempt,
  decision, receipt and projection path.

### Evidence To Add

For each provider probe, preserve:

- exact provider command or request;
- whether the request was naked, in-case or outside-case;
- prompt/context sent to the model, redacted if needed;
- model response;
- corresponding journal records;
- projection summary before and after the provider call;
- any blocked, redacted or constrained fields.

### Linenoise Boundary Note

Linenoise by Salvatore Sanfilippo may be useful as local terminal input
infrastructure for naked/debug probes. It is not part of YAI core, not a model
carrier, not case entry and not provider semantics. The architecture path for a
model entering a case begins in `yai-core` with participant admission.

## Complete Terminal Flow With Policy Material

This is the full manual sequence for the current test shape.

### Terminal 0: Clean, Build And Prepare Runtime

```bash
cd ~/COMPUTER_SCIENCE/DEV_CODE/YAI/yai-core
pwd

pkill -f "build/yaid" 2>/dev/null || true

rm -rf build/tmp
mkdir -p build/tmp/manual-case-001

make build
make check

export YAI_RUN="$PWD/build/tmp/manual-case-001"
export YAI_SOCKET="$YAI_RUN/yaid.sock"
mkdir -p "$YAI_RUN"

echo "YAI_RUN=$YAI_RUN"
echo "YAI_SOCKET=$YAI_SOCKET"
```

### Terminal 1: Start The Daemon

Leave this terminal occupied:

```bash
cd ~/COMPUTER_SCIENCE/DEV_CODE/YAI/yai-core

export YAI_RUN="$PWD/build/tmp/manual-case-001"
export YAI_SOCKET="$YAI_RUN/yaid.sock"
mkdir -p "$YAI_RUN"

build/yaid --socket "$YAI_SOCKET" --foreground
```

### Terminal 2: Run The Case Loop

```bash
cd ~/COMPUTER_SCIENCE/DEV_CODE/YAI/yai-core

export YAI_RUN="$PWD/build/tmp/manual-case-001"
export YAI_SOCKET="$YAI_RUN/yaid.sock"

export YAI="./target/debug/yai"
test -x "$YAI" || export YAI="./crates/target/debug/yaictl"
test -x "$YAI" || export YAI="./crates/target/debug/yai"

test -S "$YAI_SOCKET" && echo "socket ok: $YAI_SOCKET" || exit 1

$YAI daemon status --socket "$YAI_SOCKET" || exit 1
$YAI daemon info --socket "$YAI_SOCKET" || exit 1

$YAI daemon run-filesystem-loop --socket "$YAI_SOCKET" || exit 1

export JOURNAL="$(command find build/tmp -type f -path '*/filesystem/journal.jsonl' | sort | tail -n 1)"
echo "JOURNAL=$JOURNAL"
test -f "$JOURNAL" || exit 1

$YAI store tail --journal "$JOURNAL"
$YAI receipt summary --journal "$JOURNAL"
$YAI projection inspect --journal "$JOURNAL"
$YAI query summary --journal "$JOURNAL"
$YAI engine summary --journal "$JOURNAL"
```

### Terminal 2: Generate Manual Policy Pack Material

Run this after `JOURNAL` is set:

```bash
export POLICY_PACK_DIR="$YAI_RUN/policy-packs"
export POLICY_PACK_SOURCE="$PWD/docs/manuals/examples/filesystem-loop/policy-packs"
mkdir -p "$POLICY_PACK_DIR"

test -d "$POLICY_PACK_SOURCE" || exit 1
cp "$POLICY_PACK_SOURCE"/*.json "$POLICY_PACK_DIR"/

command find "$POLICY_PACK_DIR" -type f -name '*.json' | sort
```

### Terminal 2: Preserve Policy Attachment Evidence

```bash
export POLICY_EVIDENCE="$YAI_RUN/policy-attachment-evidence.txt"

{
  echo "case_ref=case:new12-filesystem"
  echo "journal=$JOURNAL"
  echo "filesystem_policy=$POLICY_PACK_DIR/filesystem-sandbox-policy.json"
  echo "model_policy=$POLICY_PACK_DIR/model-case-projection-policy.json"
  echo "terminal_policy=$POLICY_PACK_DIR/linenoise-terminal-interface-policy.json"
  echo "binding:subject:filesystem-sandbox=policy:manual-filesystem-sandbox-v0"
  echo "binding:subject:llm-provider=policy:manual-model-case-projection-v0"
  echo "binding:subject:linenoise-terminal=policy:manual-linenoise-terminal-interface-v0"
} > "$POLICY_EVIDENCE"

cat "$POLICY_EVIDENCE"

for pack in "$POLICY_PACK_DIR"/*.json; do
  echo "PACK=$pack"
  sed -n '1,220p' "$pack"
done
```

### Terminal 2: Inspect Decisions Against The Policy Material

```bash
$YAI query records --journal "$JOURNAL" --case case:new12-filesystem --limit 20
$YAI query records --journal "$JOURNAL" --kind filesystem_receipt --limit 20
$YAI query records --journal "$JOURNAL" --kind decision --limit 20
$YAI query records --journal "$JOURNAL" --kind memory_candidate --limit 20
$YAI projection inspect --journal "$JOURNAL"
$YAI engine summary --journal "$JOURNAL"
```

Expected interpretation for the current loop:

```text
fs.read  -> observed receipt, allowed by read-like/default rule
fs.write mutative -> require_review decision, blocked receipt
fs.write sandbox  -> allow_with_constraints decision, executed receipt
```

### Terminal 3: Model Subject Enters The Case

Correct posture:

```text
case owns subject admission
case binds subject:llm-provider
case binds subject:linenoise-terminal only as operator input surface
case materializes the model participant view
case later records prompt attempts, provider outputs, decisions and receipts
model receives the participant view because it is inside the case
operator/debug CLI may inspect the view, but must not be the runtime source
```

The first core primitive for this is `yai case enter`. It validates that
`subject:llm-provider` is bound to the case, validates that a governed
`consumer:model kind:model_context redaction:summary_only` projection exists,
appends a case-entry subject-state record to the journal and prints the
participant view.

```bash
$YAI case enter \
  --journal "$JOURNAL" \
  --case case:new12-filesystem \
  --subject subject:llm-provider
```

If you want the host shell prompt to show that this terminal is now scoped to
the case, load the zsh integration once. It keeps the normal command shape and
applies the prompt flag in the parent shell:

```bash
source "$PWD/tools/shell/yai.zsh"

$YAI case enter \
  --journal "$JOURNAL" \
  --case case:new12-filesystem \
  --subject subject:llm-provider
```

Expected host prompt prefix:

```text
[yai:case:new12-filesystem]
```

To leave the case-scoped shell prompt:

```bash
yai-case-leave
```

`yai projection model-context` remains a diagnostic inspection command. It is
useful for an operator to see what the admitted model subject would receive, but
it is not the runtime source of model context and must not be redirected into a
local text-file handoff for a provider.

```bash
export YAI_CASE_REF="case:new12-filesystem"

$YAI projection inspect --journal "$JOURNAL"
$YAI projection model-context --journal "$JOURNAL" --case "$YAI_CASE_REF"
```

Linenoise and any local provider adapter are outside this architecture path. A
naked local prompt may be used only as a debug probe and should truthfully say
that no YAI case participant view was provided. It must not be described as the
model entering the case, and it must not own provider semantics, policy
semantics, projection boundaries or model memory.

```text
allowed now:
  yai-core records model subject admission and participant view materialization

not implemented yet:
  yai-core model/provider carrier
  prompt attempt records
  provider output receipts
  model-output projection back into the case
```

The next real implementation step is a core model/provider carrier. That is the
place where prompts and provider outputs become case residue.

### Terminal 2: Shutdown

```bash
$YAI daemon shutdown --socket "$YAI_SOCKET"
```

## Troubleshooting Notes

If journal discovery fails with an `fd` usage error, rerun the journal lookup
with `command find` as shown above.

If daemon status fails, confirm Terminal 1 is still running and that both
terminals exported the same `YAI_SOCKET`.

If the first terminal does not return after shutdown, inspect for a leftover
daemon before starting a new run:

```bash
ps aux | grep '[y]aid'
```
