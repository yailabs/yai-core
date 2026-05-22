use std::fs::{self, OpenOptions};
use std::io::{Read, Write};
#[cfg(unix)]
use std::os::unix::net::UnixStream;
use std::path::PathBuf;

use yai_core_engine::graph::GraphSummary;
use yai_core_engine::journal::Journal;
use yai_core_engine::memory::MemorySummary;
use yai_core_engine::projection::ProjectionSummary;
use yai_core_engine::query::{QueryFilter, QueryResult};
use yai_core_engine::reconcile::ReconcileSummary;
use yai_core_engine::record::{Record, RecordKind};
use yai_core_engine::store::Store;

const VERSION: &str = env!("CARGO_PKG_VERSION");

fn print_info() {
    println!("yai: technical YAI Core control command");
    println!("status: NEW.15");
    println!("ownership: Rust client over C-defined core primitives");
    println!("daemon_ipc: local Unix socket with daemon-backed loop v0");
    println!("canonical_daemon: yaid");
    println!("journal_inspection: file-based JSONL v0");
    println!("control_inspection: journal-derived summary");
}

fn print_doctor() {
    let yai_home = yai_home();
    let run_dir = yai_home.join("run");
    let store_dir = yai_home.join("store");
    let log_dir = yai_home.join("log");
    let tmp_dir = yai_home.join("tmp");
    let socket = run_dir.join("yaid.sock");
    let yai_path = std::env::current_exe()
        .map(|path| path.display().to_string())
        .unwrap_or_else(|_| "unknown".to_string());
    let yaid_path = find_on_path("yaid").unwrap_or_else(|| "not found on PATH".to_string());
    let path_status = path_contains_current_bin().unwrap_or(false);

    println!("yai doctor: ok");
    println!("public_semantics: C ABI + core docs");
    println!("rust_role: yai client and operational data engine skeleton");
    println!("journal_mode: file debug only");
    println!("binary_path: {yai_path}");
    println!("yaid_path: {yaid_path}");
    println!("yai_version: {VERSION}");
    println!("YAI_HOME: {}", yai_home.display());
    println!("run_dir: {}", run_dir.display());
    println!("store_dir: {}", store_dir.display());
    println!("log_dir: {}", log_dir.display());
    println!("tmp_dir: {}", tmp_dir.display());
    println!(
        "PATH_status: {}",
        if path_status {
            "current binary dir present"
        } else {
            "warning current binary dir not on PATH"
        }
    );
    println!("daemon_socket_default: {}", socket.display());
}

fn print_usage() {
    println!("usage: yai [--version|info|doctor]");
    println!("       yai store tail --journal <path>");
    println!("       yai projection summary --journal <path>");
    println!("       yai projection inspect --journal <path>");
    println!("       yai projection request --journal <path> --consumer <consumer> --kind <kind>");
    println!("       yai projection model-context --journal <path> [--case <case_ref>]");
    println!("       yai case enter --journal <path> --case <case_ref> --subject <subject_ref> [--consumer model] [--kind model_context] [--shell zsh]");
    println!("       yai control summary --journal <path>");
    println!("       yai decision inspect --journal <path>");
    println!("       yai receipt summary --journal <path>");
    println!("       yai graph summary --journal <path>");
    println!("       yai memory summary --journal <path>");
    println!("       yai reconcile summary --journal <path>");
    println!("       yai query summary --journal <path>");
    println!("       yai query records --journal <path> [--kind <record_kind>] [--case <case_ref>] [--limit <N>]");
    println!("       yai engine summary --journal <path>");
    println!("       yai daemon status --socket <path>");
    println!("       yai daemon info --socket <path>");
    println!("       yai daemon shutdown --socket <path>");
    println!("       yai daemon run-minimum-loop --socket <path>");
    println!("       yai daemon run-filesystem-loop --socket <path>");
    println!("       yai daemon journal-summary --socket <path> --journal <path>");
    println!("       yai daemon projection-summary --socket <path> --journal <path>");
    println!("       yai carrier fs-read --sandbox <sandbox> --path <path>");
    println!("       yai carrier fs-write --sandbox <sandbox> --path <path> --content <text>");
}

fn yai_home() -> PathBuf {
    std::env::var_os("YAI_HOME")
        .map(PathBuf::from)
        .unwrap_or_else(|| {
            std::env::var_os("HOME")
                .map(PathBuf::from)
                .unwrap_or_else(|| PathBuf::from("."))
                .join(".yai")
        })
}

fn find_on_path(name: &str) -> Option<String> {
    let path = std::env::var_os("PATH")?;
    for dir in std::env::split_paths(&path) {
        let candidate = dir.join(name);
        if candidate.is_file() {
            return Some(candidate.display().to_string());
        }
    }
    None
}

fn path_contains_current_bin() -> Result<bool, String> {
    let current = std::env::current_exe()
        .map_err(|error| format!("failed to resolve current executable: {error}"))?;
    let Some(parent) = current.parent() else {
        return Ok(false);
    };
    let Some(path) = std::env::var_os("PATH") else {
        return Ok(false);
    };
    Ok(std::env::split_paths(&path).any(|entry| entry == parent))
}

fn journal_arg(args: &[String]) -> Result<PathBuf, String> {
    let mut index = 0;
    while index < args.len() {
        if args[index] == "--journal" {
            return args
                .get(index + 1)
                .map(PathBuf::from)
                .ok_or_else(|| "--journal requires a path".to_string());
        }
        index += 1;
    }
    Err("--journal <path> is required".to_string())
}

fn named_arg(args: &[String], name: &str) -> Result<String, String> {
    let mut index = 0;
    while index < args.len() {
        if args[index] == name {
            return args
                .get(index + 1)
                .cloned()
                .ok_or_else(|| format!("{name} requires a value"));
        }
        index += 1;
    }
    Err(format!("{name} is required"))
}

fn optional_arg(args: &[String], name: &str) -> Option<String> {
    let mut index = 0;
    while index < args.len() {
        if args[index] == name {
            return args.get(index + 1).cloned();
        }
        index += 1;
    }
    None
}

fn path_inside_sandbox(sandbox: &str, path: &str) -> bool {
    if sandbox.is_empty()
        || path.is_empty()
        || sandbox.split('/').any(|part| part == "..")
        || path.split('/').any(|part| part == "..")
    {
        return false;
    }
    path == sandbox || path.starts_with(&format!("{sandbox}/"))
}

fn fnv_hash(bytes: &[u8]) -> String {
    let mut value = 0xcbf29ce484222325u64;
    for byte in bytes {
        value ^= u64::from(*byte);
        value = value.wrapping_mul(0x100000001b3);
    }
    format!("{value:016x}")
}

fn store_tail(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let contents = fs::read_to_string(&path)
        .map_err(|error| format!("failed to read {}: {error}", path.display()))?;
    let mut count = 0usize;
    for line in contents.lines() {
        println!("{line}");
        count += 1;
    }
    println!("records: {count}");
    Ok(())
}

fn projection_summary(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let projection = ProjectionSummary::from_journal("operator", &journal);
    println!("records: {}", projection.source_record_count);
    if !projection.case_ref.is_empty() {
        println!("case: {}", projection.case_ref);
    }
    println!("receipts: {}", projection.receipt_count);
    println!("subjects: {}", projection.subject_count);
    Ok(())
}

fn projection_inspect(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let projection = ProjectionSummary::from_journal("projection", &journal);
    println!("records: {}", projection.source_record_count);
    println!(
        "projection_requests: {}",
        projection.projection_request_count
    );
    println!("projection_results: {}", projection.projection_result_count);
    println!("operator: {}", projection.operator_projection_count);
    println!("model: {}", projection.model_projection_count);
    println!("audit: {}", projection.audit_projection_count);
    println!(
        "redacted_or_limited: {}",
        projection.limited_projection_count
    );
    Ok(())
}

fn default_redaction_for_consumer(consumer: &str) -> &'static str {
    match consumer {
        "model" | "agent" => "summary_only",
        "debug" => "refs_only",
        _ => "none",
    }
}

fn projection_request(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let consumer = named_arg(args, "--consumer")?;
    let kind = named_arg(args, "--kind")?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let projection = ProjectionSummary::from_journal(&consumer, &journal);
    println!("projection_request: preview");
    println!("consumer: {consumer}");
    println!("kind: {kind}");
    println!("redaction: {}", default_redaction_for_consumer(&consumer));
    println!("freshness: fresh");
    println!("source_records: {}", projection.source_record_count);
    println!(
        "source_receipts: {}",
        projection.receipt_count + projection.filesystem_receipt_count
    );
    println!("source_memory: {}", projection.memory_candidate_count);
    println!("source_divergences: {}", projection.divergence_count);
    Ok(())
}

fn control_summary(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let projection = ProjectionSummary::from_journal("control", &journal);
    println!("records: {}", projection.source_record_count);
    println!("decisions: {}", projection.decision_count);
    println!("rules: {}", projection.policy_rule_count);
    println!("gates: {}", projection.gate_count);
    println!("obligations: {}", projection.obligation_count);
    println!(
        "receipt_requirements: {}",
        projection.receipt_requirement_count
    );
    Ok(())
}

fn record_in_case(record: &Record, case_ref: Option<&str>) -> bool {
    case_ref
        .map(|expected| record.case_ref == expected)
        .unwrap_or(true)
}

fn display_field<'a>(value: &'a str, fallback: &'static str) -> &'a str {
    if value.is_empty() {
        fallback
    } else {
        value
    }
}

fn print_model_context_records(
    title: &str,
    journal: &Journal,
    case_ref: Option<&str>,
    kinds: &[RecordKind],
) {
    println!("## {title}");
    let mut count = 0usize;
    for record in journal
        .records()
        .iter()
        .filter(|record| record_in_case(record, case_ref) && kinds.contains(&record.kind))
    {
        println!(
            "- {} kind:{} subject_ref:{} attempt_id:{} decision_id:{} receipt_id:{} summary:{}",
            record.id,
            record.kind.as_str(),
            display_field(&record.subject_ref, "subject:none"),
            display_field(&record.attempt_id, "none"),
            display_field(&record.decision_id, "none"),
            display_field(&record.receipt_id, "none"),
            record.summary
        );
        count += 1;
    }
    if count == 0 {
        println!("- none");
    }
    println!();
}

fn print_model_context_view(journal: &Journal, case_ref: Option<&str>) {
    let projection = ProjectionSummary::from_journal("model", &journal);
    let case_ref = case_ref
        .or_else(|| (!projection.case_ref.is_empty()).then_some(projection.case_ref.as_str()));

    println!("case_ref: {}", case_ref.unwrap_or("unknown"));
    println!("consumer: model");
    println!("projection_kind: model_context");
    println!("redaction: summary_only");
    println!("source: case_projection_graph_memory");
    println!("raw_journal_access: not_provided");
    println!("filesystem_access: not_provided");
    println!("records: {}", projection.source_record_count);
    println!(
        "model_projection_records: {}",
        projection.model_projection_count
    );
    println!(
        "operator_projection_records: {}",
        projection.operator_projection_count
    );
    println!(
        "filesystem_receipts: {}",
        projection.filesystem_receipt_count
    );
    println!("memory_candidates: {}", projection.memory_candidate_count);
    println!("graph_edges: {}", projection.graph_edge_count);
    println!();

    print_model_context_records(
        "Subjects",
        &journal,
        case_ref,
        &[RecordKind::SubjectBinding],
    );
    print_model_context_records("Policy", &journal, case_ref, &[RecordKind::PolicyRule]);
    print_model_context_records("Decisions", &journal, case_ref, &[RecordKind::Decision]);
    print_model_context_records(
        "Filesystem Receipts",
        &journal,
        case_ref,
        &[RecordKind::FilesystemReceipt],
    );
    print_model_context_records("Memory", &journal, case_ref, &[RecordKind::MemoryCandidate]);
    print_model_context_records("Graph", &journal, case_ref, &[RecordKind::GraphEdge]);
    print_model_context_records(
        "Projection Records",
        &journal,
        case_ref,
        &[RecordKind::ProjectionRequest, RecordKind::ProjectionResult],
    );
}

fn projection_model_context(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let requested_case = optional_arg(args, "--case");
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    print_model_context_view(&journal, requested_case.as_deref());
    Ok(())
}

fn append_case_entry_record(
    path: &PathBuf,
    journal: &Journal,
    case_ref: &str,
    subject_ref: &str,
    consumer: &str,
    kind: &str,
) -> Result<(), String> {
    let record_id = format!(
        "case-entry:{}:{}",
        subject_ref.replace(':', "-"),
        journal.count() + 1
    );
    let record = Record::from_parts(
        record_id,
        case_ref,
        RecordKind::SubjectState,
        subject_ref,
        "",
        "",
        "",
        format!(
            "case_entry:admitted consumer:{consumer} kind:{kind} redaction:summary_only raw_journal_access:not_provided filesystem_access:not_provided"
        ),
    );
    let mut file = OpenOptions::new()
        .append(true)
        .open(path)
        .map_err(|error| format!("failed to append {}: {error}", path.display()))?;
    file.write_all(record.to_jsonl().as_bytes())
        .map_err(|error| format!("failed to write {}: {error}", path.display()))?;
    Ok(())
}

fn shell_quote(value: &str) -> String {
    let mut quoted = String::from("'");
    for ch in value.chars() {
        if ch == '\'' {
            quoted.push_str("'\\''");
        } else {
            quoted.push(ch);
        }
    }
    quoted.push('\'');
    quoted
}

fn print_case_enter_shell(path: &PathBuf, case_ref: &str, subject_ref: &str) {
    let prompt_flag = format!("[yai:{case_ref}]");
    println!("printf '%s\\n' {}", shell_quote("case_entry: accepted"));
    println!(
        "printf '%s\\n' {}",
        shell_quote("case_entry_status: shell_scoped")
    );
    println!(
        "printf '%s\\n' {}",
        shell_quote(&format!("case_ref: {case_ref}"))
    );
    println!(
        "printf '%s\\n' {}",
        shell_quote(&format!("subject_ref: {subject_ref}"))
    );
    println!(
        "printf '%s\\n' {}",
        shell_quote("participant_view: model_context")
    );
    println!("printf '%s\\n' {}", shell_quote("consumer: model"));
    println!("printf '%s\\n' {}", shell_quote("redaction: summary_only"));
    println!(
        "printf '%s\\n' {}",
        shell_quote("raw_journal_access: not_provided")
    );
    println!(
        "printf '%s\\n' {}",
        shell_quote("filesystem_access: not_provided")
    );
    println!(
        "export YAI_JOURNAL={}",
        shell_quote(&path.display().to_string())
    );
    println!("export YAI_CASE_REF={}", shell_quote(case_ref));
    println!("export YAI_SUBJECT_REF={}", shell_quote(subject_ref));
    println!("export YAI_CASE_PROMPT_FLAG={}", shell_quote(&prompt_flag));
    println!("if [ -z \"${{YAI_PROMPT_BASE+x}}\" ]; then export YAI_PROMPT_BASE=\"${{PROMPT:-${{PS1:-}}}}\"; fi");
    println!("if [ -z \"${{YAI_RPROMPT_BASE+x}}\" ]; then export YAI_RPROMPT_BASE=\"${{RPROMPT:-}}\"; fi");
    println!("export PROMPT=\"$YAI_CASE_PROMPT_FLAG $YAI_PROMPT_BASE\"");
    println!("export PS1=\"$PROMPT\"");
    println!("export RPROMPT=\"$YAI_RPROMPT_BASE\"");
}

fn case_enter(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let case_ref = named_arg(args, "--case")?;
    let subject_ref = named_arg(args, "--subject")?;
    let consumer = optional_arg(args, "--consumer").unwrap_or_else(|| "model".to_string());
    let kind = optional_arg(args, "--kind").unwrap_or_else(|| "model_context".to_string());
    let shell = optional_arg(args, "--shell");
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;

    let subject_bound = journal.records().iter().any(|record| {
        record.case_ref == case_ref
            && record.kind == RecordKind::SubjectBinding
            && record.subject_ref == subject_ref
    });
    if !subject_bound {
        return Err(format!(
            "subject {subject_ref} is not bound to case {case_ref}"
        ));
    }

    let projection_available = journal.records().iter().any(|record| {
        record.case_ref == case_ref
            && record.kind == RecordKind::ProjectionResult
            && record.summary.contains(&format!("consumer:{consumer}"))
            && record.summary.contains(&format!("kind:{kind}"))
            && record.summary.contains("redaction:summary_only")
    });
    if !projection_available {
        return Err(format!(
            "no governed {consumer}/{kind} projection is available for case {case_ref}"
        ));
    }

    let already_admitted = journal.records().iter().any(|record| {
        record.case_ref == case_ref
            && record.kind == RecordKind::SubjectState
            && record.subject_ref == subject_ref
            && record.summary.contains("case_entry:admitted")
            && record.summary.contains(&format!("consumer:{consumer}"))
            && record.summary.contains(&format!("kind:{kind}"))
    });

    if !already_admitted {
        append_case_entry_record(&path, &journal, &case_ref, &subject_ref, &consumer, &kind)?;
    }

    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to reload {}: {error}", path.display()))?;

    if let Some(shell) = shell.as_deref() {
        if shell != "zsh" && shell != "sh" {
            return Err(format!("unsupported shell: {shell}"));
        }
        print_case_enter_shell(&path, &case_ref, &subject_ref);
        return Ok(());
    }

    println!("case_entry: accepted");
    println!(
        "case_entry_status: {}",
        if already_admitted {
            "already_admitted"
        } else {
            "admitted"
        }
    );
    println!("case_ref: {case_ref}");
    println!("subject_ref: {subject_ref}");
    println!("participant_view: {kind}");
    println!("consumer: {consumer}");
    println!("redaction: summary_only");
    println!("raw_journal_access: not_provided");
    println!("filesystem_access: not_provided");
    println!();
    print_model_context_view(&journal, Some(&case_ref));
    Ok(())
}

fn decision_outcome(summary: &str) -> String {
    for part in summary.split_whitespace() {
        if let Some(value) = part.strip_prefix("decision:") {
            return value.trim_matches(',').to_string();
        }
    }
    "unknown".to_string()
}

fn decision_inspect(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let projection = ProjectionSummary::from_journal("control", &journal);
    let decision = journal
        .records()
        .iter()
        .find(|record| record.kind == RecordKind::Decision);
    let basis = journal
        .records()
        .iter()
        .find(|record| record.kind == RecordKind::DecisionBasis);

    if let Some(record) = decision {
        println!("decision: {}", decision_outcome(&record.summary));
    } else {
        println!("decision: none");
    }
    if let Some(record) = basis {
        println!("basis: {}", record.summary);
    } else {
        println!("basis: none");
    }
    println!("obligations: {}", projection.obligation_count);
    println!(
        "receipt_requirements: {}",
        projection.receipt_requirement_count
    );
    Ok(())
}

fn receipt_summary(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let projection = ProjectionSummary::from_journal("receipt", &journal);
    println!("records: {}", projection.source_record_count);
    println!("receipts: {}", projection.receipt_count);
    println!(
        "filesystem_receipts: {}",
        projection.filesystem_receipt_count
    );
    println!("subject_states: {}", projection.subject_state_count);
    println!("effects: {}", projection.effect_count);
    Ok(())
}

fn carrier_fs_read(args: &[String]) -> Result<(), String> {
    let sandbox = named_arg(args, "--sandbox")?;
    let path = named_arg(args, "--path")?;
    if !path_inside_sandbox(&sandbox, &path) {
        return Err("path is outside sandbox".to_string());
    }
    let bytes = fs::read(&path).map_err(|error| format!("failed to read {path}: {error}"))?;
    println!("carrier: filesystem");
    println!("effect: fs.read");
    println!("status: observed");
    println!("bytes: {}", bytes.len());
    println!("hash: {}", fnv_hash(&bytes));
    Ok(())
}

fn carrier_fs_write(args: &[String]) -> Result<(), String> {
    let sandbox = named_arg(args, "--sandbox")?;
    let path = named_arg(args, "--path")?;
    let content = named_arg(args, "--content")?;
    if !path_inside_sandbox(&sandbox, &path) {
        return Err("path is outside sandbox".to_string());
    }
    let mut file =
        fs::File::create(&path).map_err(|error| format!("failed to open {path}: {error}"))?;
    file.write_all(content.as_bytes())
        .map_err(|error| format!("failed to write {path}: {error}"))?;
    println!("carrier: filesystem");
    println!("effect: fs.write");
    println!("status: executed");
    println!("bytes: {}", content.len());
    println!("hash: {}", fnv_hash(content.as_bytes()));
    Ok(())
}

fn graph_summary(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let projection = ProjectionSummary::from_journal("graph", &journal);
    let graph = GraphSummary::from_summaries(
        journal
            .records()
            .iter()
            .filter(|record| record.kind == RecordKind::GraphEdge)
            .map(|record| record.summary.as_str()),
    );

    println!("graph_edges: {}", projection.graph_edge_count);
    println!("case_binds_subject: {}", graph.case_binds_subject);
    println!("op_targets_subject: {}", graph.op_targets_subject);
    println!("decision_controls_op: {}", graph.decision_controls_op);
    println!("receipt_records_effect: {}", graph.receipt_records_effect);
    println!("receipt_updates_subject: {}", graph.receipt_updates_subject);
    Ok(())
}

fn memory_summary(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let summary = MemorySummary::from_journal(&journal);

    println!("records: {}", summary.records);
    println!("memory_candidates: {}", summary.memory_candidates);
    println!("operational: {}", summary.operational);
    println!("decision: {}", summary.decision);
    println!("subject: {}", summary.subject);
    println!("error: {}", summary.error);
    println!("recovery: {}", summary.recovery);
    Ok(())
}

fn reconcile_summary(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let summary = ReconcileSummary::from_journal(&journal);

    println!("records: {}", summary.records);
    println!("divergences: {}", summary.divergences);
    println!("reconciliations: {}", summary.reconciliations);
    println!("critical: {}", summary.critical);
    println!("warnings: {}", summary.warnings);
    Ok(())
}

fn query_filter_from_args(args: &[String]) -> Result<QueryFilter, String> {
    let record_kind = optional_arg(args, "--kind")
        .map(|kind| {
            RecordKind::from_str(&kind).ok_or_else(|| format!("unknown record kind: {kind}"))
        })
        .transpose()?;
    let limit = optional_arg(args, "--limit")
        .map(|value| {
            value
                .parse::<usize>()
                .map_err(|error| format!("invalid --limit value: {error}"))
        })
        .transpose()?;

    Ok(QueryFilter {
        case_ref: optional_arg(args, "--case"),
        record_kind,
        limit,
        include_summary: true,
        ..Default::default()
    })
}

fn query_summary(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let filter = QueryFilter::default();
    let result = QueryResult::scan(&journal, &filter);
    println!("records: {}", result.records);
    println!("matched: {}", result.matched);
    println!("returned: {}", result.returned);
    println!("truncated: {}", result.truncated);
    Ok(())
}

fn query_records(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let filter = query_filter_from_args(args)?;
    let include_summary = filter.include_summary;
    let result = QueryResult::scan(&journal, &filter);

    println!("records: {}", result.records);
    println!("matched: {}", result.matched);
    println!("returned: {}", result.returned);
    println!("truncated: {}", result.truncated);
    for record in result.matched_records {
        if include_summary {
            println!("{} {} {}", record.id, record.kind.as_str(), record.summary);
        } else {
            println!("{} {}", record.id, record.kind.as_str());
        }
    }
    Ok(())
}

fn engine_summary(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let store = Store::from_journal(journal);
    let summary = store.engine_summary();
    println!("records: {}", summary.records);
    println!("receipts: {}", summary.receipts);
    println!("graph_edges: {}", summary.graph_edges);
    println!("memory_candidates: {}", summary.memory_candidates);
    println!("projections: {}", summary.projections);
    println!("divergences: {}", summary.divergences);
    Ok(())
}

#[cfg(unix)]
fn daemon_request(args: &[String], request: &str) -> Result<(), String> {
    let socket = named_arg(args, "--socket")?;
    let mut stream = UnixStream::connect(&socket)
        .map_err(|error| format!("failed to connect {socket}: {error}"))?;
    stream
        .write_all(format!("{request}\n").as_bytes())
        .map_err(|error| format!("failed to write request: {error}"))?;
    let mut response = String::new();
    stream
        .read_to_string(&mut response)
        .map_err(|error| format!("failed to read response: {error}"))?;
    print!("{response}");
    Ok(())
}

#[cfg(unix)]
fn daemon_request_with_journal(args: &[String], request: &str) -> Result<(), String> {
    let journal = journal_arg(args)?;
    let line = format!(
        "{request} request_id=yai-{request} payload={}",
        journal.display()
    );
    daemon_request(args, &line)
}

#[cfg(not(unix))]
fn daemon_request(_args: &[String], _request: &str) -> Result<(), String> {
    Err("daemon IPC is only implemented on Unix in NEW.11".to_string())
}

#[cfg(not(unix))]
fn daemon_request_with_journal(_args: &[String], _request: &str) -> Result<(), String> {
    Err("daemon IPC is only implemented on Unix in NEW.13".to_string())
}

fn main() {
    let args = std::env::args().skip(1).collect::<Vec<_>>();
    let result = match args.first().map(String::as_str) {
        Some("--version") | Some("version") => println!("yai {}", VERSION),
        Some("info") => print_info(),
        Some("doctor") => print_doctor(),
        Some("store") if args.get(1).map(String::as_str) == Some("tail") => {
            if let Err(error) = store_tail(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("projection") if args.get(1).map(String::as_str) == Some("summary") => {
            if let Err(error) = projection_summary(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("projection") if args.get(1).map(String::as_str) == Some("inspect") => {
            if let Err(error) = projection_inspect(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("projection") if args.get(1).map(String::as_str) == Some("request") => {
            if let Err(error) = projection_request(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("projection") if args.get(1).map(String::as_str) == Some("model-context") => {
            if let Err(error) = projection_model_context(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("case") if args.get(1).map(String::as_str) == Some("enter") => {
            if let Err(error) = case_enter(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("control") if args.get(1).map(String::as_str) == Some("summary") => {
            if let Err(error) = control_summary(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("decision") if args.get(1).map(String::as_str) == Some("inspect") => {
            if let Err(error) = decision_inspect(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("receipt") if args.get(1).map(String::as_str) == Some("summary") => {
            if let Err(error) = receipt_summary(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("graph") if args.get(1).map(String::as_str) == Some("summary") => {
            if let Err(error) = graph_summary(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("memory") if args.get(1).map(String::as_str) == Some("summary") => {
            if let Err(error) = memory_summary(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("reconcile") if args.get(1).map(String::as_str) == Some("summary") => {
            if let Err(error) = reconcile_summary(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("query") if args.get(1).map(String::as_str) == Some("summary") => {
            if let Err(error) = query_summary(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("query") if args.get(1).map(String::as_str) == Some("records") => {
            if let Err(error) = query_records(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("engine") if args.get(1).map(String::as_str) == Some("summary") => {
            if let Err(error) = engine_summary(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("daemon") if args.get(1).map(String::as_str) == Some("status") => {
            if let Err(error) = daemon_request(&args[2..], "status") {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("daemon") if args.get(1).map(String::as_str) == Some("info") => {
            if let Err(error) = daemon_request(&args[2..], "info") {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("daemon") if args.get(1).map(String::as_str) == Some("shutdown") => {
            if let Err(error) = daemon_request(&args[2..], "shutdown") {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("daemon") if args.get(1).map(String::as_str) == Some("run-minimum-loop") => {
            if let Err(error) = daemon_request(
                &args[2..],
                "run_minimum_loop request_id=yai-minimum case_ref=case:new12-daemon subject_ref=subject:repo-test",
            ) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("daemon") if args.get(1).map(String::as_str) == Some("run-filesystem-loop") => {
            if let Err(error) = daemon_request(
                &args[2..],
                "run_filesystem_loop request_id=yai-filesystem case_ref=case:new12-filesystem subject_ref=subject:filesystem-sandbox",
            ) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("daemon") if args.get(1).map(String::as_str) == Some("journal-summary") => {
            if let Err(error) = daemon_request_with_journal(&args[2..], "journal_summary") {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("daemon") if args.get(1).map(String::as_str) == Some("projection-summary") => {
            if let Err(error) = daemon_request_with_journal(&args[2..], "projection_summary") {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("carrier") if args.get(1).map(String::as_str) == Some("fs-read") => {
            if let Err(error) = carrier_fs_read(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("carrier") if args.get(1).map(String::as_str) == Some("fs-write") => {
            if let Err(error) = carrier_fs_write(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some(_) => {
            print_usage();
            std::process::exit(2);
        }
        None => print_usage(),
    };
    result
}
