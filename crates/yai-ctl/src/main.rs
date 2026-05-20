use std::fs;
use std::path::PathBuf;

use yai_core_engine::journal::Journal;
use yai_core_engine::projection::ProjectionSummary;
use yai_core_engine::record::RecordKind;

const VERSION: &str = env!("CARGO_PKG_VERSION");

fn print_info() {
    println!("yaictl: technical YAI Core control client");
    println!("status: NEW.2");
    println!("ownership: Rust client over C-defined core primitives");
    println!("daemon_ipc: not implemented");
    println!("journal_inspection: file-based JSONL v0");
    println!("control_inspection: journal-derived summary");
}

fn print_doctor() {
    println!("yaictl doctor: ok");
    println!("public_semantics: C ABI + core docs");
    println!("rust_role: ctl client and operational data engine skeleton");
    println!("journal_mode: file debug only");
}

fn print_usage() {
    println!("usage: yaictl [--version|info|doctor]");
    println!("       yaictl store tail --journal <path>");
    println!("       yaictl projection summary --journal <path>");
    println!("       yaictl control summary --journal <path>");
    println!("       yaictl decision inspect --journal <path>");
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

fn main() {
    let args = std::env::args().skip(1).collect::<Vec<_>>();
    let result = match args.first().map(String::as_str) {
        Some("--version") | Some("version") => println!("yaictl {}", VERSION),
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
        Some(_) => {
            print_usage();
            std::process::exit(2);
        }
        None => print_usage(),
    };
    result
}
