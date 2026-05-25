# Filesystem Loop Lab Outputs

Status: output/report contract.

The filesystem-loop lab produces local daemon logs, journal residue and optional
provider prompt output. These are evidence inputs, not publication reports.

## Local Output

```text
$YAI_HOME/run/yaid.log
$YAI_HOME/cases/manual-filesystem-loop/policy-packs/
<active journal path>
$YAI_HOME/store/lmdb/
```

Raw local output should not be committed.

## Journal Replay Output

SPINE.36 journal replay to LMDB produces command output, not a report file by
default.

Important fields:

```text
journal_replay: dry_run|completed|failed
records_written: N
records_duplicate: N
invalid_entries: N
record_store_status: ready
idempotent: yes|no
```

The LMDB directory is local runtime state and should not be committed.

## Curated Output

Curated model behavior examples live under:

```text
docs/labs/model-behavior/
```

Benchmark reports live under:

```text
docs/labs/nvidia/reports/
```

Do not mix filesystem-loop validation transcripts with benchmark reports unless
a report explicitly declares the transcript as supporting evidence.
