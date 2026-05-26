//! YAI - control CLI
//!
//! Purpose:
//!   Provide the local technical control command for runtime inspection, daemon
//!   requests and record-plane operator views.
//!
//! Ownership:
//!   Command parsing and user-facing text output for `yai`.
//!
//! Boundary:
//!   Does not own core data-plane truth, daemon internals or public SDK shape.
//!
//! Status:
//!   active

use std::ffi::{CStr, CString};
use std::fmt::Write as FmtWrite;
use std::fs::{self, OpenOptions};
use std::io::{IsTerminal, Read, Write};
use std::net::TcpStream;
use std::os::raw::{c_char, c_int, c_void};
#[cfg(unix)]
use std::os::unix::net::UnixStream;
use std::path::{Path, PathBuf};

use yai_core_engine::graph::GraphSummary;
use yai_core_engine::journal::{Journal, JournalInspection, JOURNAL_RECORD_SCHEMA};
use yai_core_engine::memory::MemorySummary;
use yai_core_engine::projection::ProjectionSummary;
use yai_core_engine::query::{QueryFilter, QueryResult};
use yai_core_engine::reconcile::ReconcileSummary;
use yai_core_engine::record::{Record, RecordKind};
use yai_core_engine::store::lmdb::{
    LmdbRecordStore, RecordStoreStatusKind, ReplayMetadata, GRAPH_RELATION_SCHEMA,
    GRAPH_RELATION_STORE_NAME, RECORD_SCHEMA,
};
use yai_core_engine::store::Store;

const VERSION: &str = env!("CARGO_PKG_VERSION");
const ANSI_RESET: &str = "\x1b[0m";
const ANSI_BOLD: &str = "\x1b[1m";
const ANSI_DIM: &str = "\x1b[2m";
const ANSI_CYAN: &str = "\x1b[36m";
const ANSI_BLUE: &str = "\x1b[34m";
const ANSI_GREEN: &str = "\x1b[32m";
const ANSI_YELLOW: &str = "\x1b[33m";
const ANSI_MAGENTA: &str = "\x1b[35m";

unsafe extern "C" {
    fn linenoise(prompt: *const c_char) -> *mut c_char;
    fn linenoiseFree(ptr: *mut c_void);
    fn linenoiseHistoryAdd(line: *const c_char) -> c_int;
    fn linenoiseHistorySetMaxLen(len: c_int) -> c_int;
    #[cfg(unix)]
    fn kill(pid: c_int, sig: c_int) -> c_int;
}

fn print_info() {
    println!("yai: technical YAI control command");
    println!("status: SPINE.42 RuntimeGraph In-Memory Working Set");
    println!("ownership: Rust client over C-defined core primitives");
    println!("daemon_ipc: local Unix socket with daemon-backed loop v0");
    println!("canonical_daemon: yaid");
    println!("runtime_layout: YAI_HOME local runtime v0");
    println!("foundation_freeze: filesystem_runtime_layout");
    println!("hot_state: YAI_HOME/run/hot-state.json live cache v0");
    println!("record_store: YAI_HOME/store/lmdb LMDB lookup plane");
    println!(
        "carrier_substrate: carrier.v1 filesystem/process plus non-process skeletons; outcome and consistency harnesses; host probe v0"
    );
    println!("provider-runtime: planned surface active");
    println!("device-registry: active");
    println!("journal_inspection: file-based JSONL v0");
    println!("journal_replay: LMDB materialization with schema/cursor/report metadata v0");
    println!("graph_relation_write_path: active_minimal");
    println!("runtime_graph: active_minimal per_command_ephemeral");
    println!("control_inspection: journal-derived summary");
}

fn print_doctor() {
    let yai_home = yai_home();
    let run_dir = yai_home.join("run");
    let store_dir = yai_home.join("store");
    let log_dir = yai_home.join("log");
    let tmp_dir = yai_home.join("tmp");
    let cases_dir = yai_home.join("cases");
    let sockets_dir = yai_home.join("sockets");
    let config_dir = yai_home.join("config");
    let socket = run_dir.join("yaid.sock");
    let hot_state_path = run_dir.join("hot-state.json");
    let record_status = record_store_status();
    let yai_path = std::env::current_exe()
        .map(|path| path.display().to_string())
        .unwrap_or_else(|_| "unknown".to_string());
    let yaid_path = find_on_path("yaid").unwrap_or_else(|| "not found on PATH".to_string());
    let yaid_found = if find_on_path("yaid").is_some() {
        "found"
    } else {
        "missing"
    };
    let path_status = path_contains_current_bin().unwrap_or(false);
    let runtime_layout_ok = [
        &run_dir,
        &store_dir,
        &log_dir,
        &tmp_dir,
        &cases_dir,
        &sockets_dir,
        &config_dir,
    ]
    .iter()
    .all(|path| path.is_dir());
    let hot_status = hot_snapshot_status(&hot_state_path);

    println!("yai doctor: ok");
    println!("public_semantics: C ABI + core docs");
    println!("rust_role: yai client and operational data engine skeleton");
    println!("journal_mode: file debug only");
    println!("binary_path: {yai_path}");
    println!("yaid_path: {yaid_path}");
    println!("yaid_found: {yaid_found}");
    println!("yai_version: {VERSION}");
    println!("YAI_HOME: {}", yai_home.display());
    println!("YAI_HOME_status: {}", path_state(&yai_home));
    println!("run_dir: {}", path_state_with_path(&run_dir));
    println!("store_dir: {}", path_state_with_path(&store_dir));
    println!("log_dir: {}", path_state_with_path(&log_dir));
    println!("tmp_dir: {}", path_state_with_path(&tmp_dir));
    println!("cases_dir: {}", path_state_with_path(&cases_dir));
    println!("sockets_dir: {}", path_state_with_path(&sockets_dir));
    println!("config_dir: {}", path_state_with_path(&config_dir));
    println!(
        "env_file: {}",
        yai_env_file()
            .map(|path| path.display().to_string())
            .unwrap_or_else(|| "not found".to_string())
    );
    println!(
        "PATH_status: {}",
        if path_status {
            "current binary dir present"
        } else {
            "warning current binary dir not on PATH"
        }
    );
    println!("daemon_socket_default: {}", socket.display());
    println!(
        "socket_default_status: {}",
        if socket.exists() {
            "present"
        } else {
            "not_present"
        }
    );
    println!("hot_state_path: {}", hot_state_path.display());
    println!("hot_state_status: {}", hot_status.status);
    println!("hot_state_schema_status: {}", hot_status.schema_status);
    println!("hot_state_readable: {}", hot_status.readable);
    println!("record_store_path: {}", record_status.path.display());
    println!("record_store_status: {}", record_status.status);
    println!("record_store_backend: {}", record_status.backend);
    if let Some(content) = hot_status.content.as_deref() {
        println!(
            "case_session_status: {}",
            json_string_or(content, "case_session_status", "unknown")
        );
        println!(
            "case_context_status: {}",
            json_string_or(content, "case_context_status", "unknown")
        );
        let projection_freshness = json_string_or(content, "projection_freshness", "unknown");
        let stale_reason = json_string_or(content, "projection_stale_reason", "unknown");
        println!("projection_freshness: {projection_freshness}");
        println!("stale_reason: {stale_reason}");
        println!(
            "freshness_policy: {}",
            projection_policy_for("model", &projection_freshness, &stale_reason)
        );
    }
    println!(
        "runtime_layout_status: {}",
        if runtime_layout_ok {
            "ok"
        } else {
            "incomplete"
        }
    );
}

fn print_usage() {
    println!("usage: yai [--version|info|doctor]");
    println!("       yai store status");
    println!("       yai store summary");
    println!("       yai store record get <record_id>");
    println!("       yai store record list --case <case_ref> [--limit <N>]");
    println!("       yai store record list --kind <record_kind> [--limit <N>]");
    println!("       yai store record list --subject <subject_ref> [--limit <N>]");
    println!("       yai store record list --receipt <receipt_ref> [--limit <N>]");
    println!("       yai store tail --journal <path>");
    println!("       yai journal inspect --path <journal.jsonl> [--show-errors]");
    println!("       yai journal replay --path <journal.jsonl> [--dry-run]");
    println!("       yai journal replay-status --path <journal.jsonl>");
    println!("       yai journal replay-report --path <journal.jsonl>");
    println!("       yai projection summary --journal <path>");
    println!("       yai projection inspect --journal <path> [--consumer model|operator|audit|debug|agent]");
    println!("       yai projection request --journal <path> --consumer <consumer> --kind <kind>");
    println!("       yai case enter --journal <path> --case <case_ref> --subject <subject_ref> [--consumer model] [--kind model_context] [--shell zsh]");
    println!("       yai case attach-provider --journal <path> --case <case_ref> --subject <subject_ref> --base-url <url> --model <model> [--api-key-env <env>] [--shell zsh]");
    println!("       yai prompt [--once <text>] [--dry-run] [--language-mode auto|none] [--journal <path>] [--case <case_ref>] [--subject <subject_ref>]");
    println!("       yai prompt [--dry-run] [--language-mode auto|none] [--journal <path>] [--case <case_ref>] [--subject <subject_ref>] < prompt.txt");
    println!("       yai control summary --journal <path>");
    println!("       yai decision inspect --journal <path>");
    println!("       yai receipt summary --journal <path>");
    println!("       yai graph summary --journal <path>");
    println!("       yai graph schema");
    println!("       yai graph runtime-status");
    println!("       yai graph materialize --case <case_ref>");
    println!("       yai graph relations --case <case_ref> [--limit <N>]");
    println!("       yai graph runtime-load --case <case_ref>");
    println!("       yai graph runtime-summary --case <case_ref>");
    println!("       yai memory summary --journal <path>");
    println!("       yai reconcile summary --journal <path>");
    println!("       yai query summary --journal <path>");
    println!("       yai query records --journal <path> [--kind <record_kind>] [--case <case_ref>] [--limit <N>]");
    println!("       yai engine summary --journal <path>");
    println!("       yai hot status");
    println!("       yai daemon status --socket <path>");
    println!("       yai daemon info --socket <path>");
    println!("       yai daemon shutdown --socket <path>");
    println!("       yai daemon run-minimum-loop --socket <path>");
    println!("       yai daemon run-filesystem-loop --socket <path>");
    println!("       yai daemon journal-summary --socket <path> --journal <path>");
    println!("       yai daemon projection-summary --socket <path> --journal <path>");
    println!("       yai carrier families");
    println!("       yai carrier lanes");
    println!("       yai carrier route --family <carrier_family>");
    println!("       yai carrier coverage [--family <carrier_family>] [--mode controlled|observed|imported]");
    println!("       yai carrier inspect <carrier_family>");
    println!("       yai carrier outcome-test --family <carrier_family> [--mode controlled|observed|imported] --outcome <outcome>");
    println!("       yai carrier reconcile-outcome --scenario <scenario>");
    println!("       yai carrier reconcile-outcome --decision <decision> --dispatch <dispatch> --carrier-outcome <outcome> --receipt-present yes|no --carrier-attempted yes|no --execution-performed yes|no --observation <result>");
    println!("       yai device add --id <id> --name <name> --host <host> --port <port> --target local|lan|external");
    println!("       yai device list");
    println!("       yai device inspect <device_id>");
    println!("       yai device trust <device_id>");
    println!("       yai device untrust <device_id>");
    println!("       yai provider runtime status");
    println!("       yai provider targets");
    println!("       yai provider start --dry-run --target local --kind <kind> --model <model>");
    println!("       yai provider start --dry-run --target lan --device <device_id> --kind <kind> --model <model>");
    println!(
        "       yai provider start --dry-run --target external --endpoint <url> --kind <kind>"
    );
    println!("       yai provider logs-path");
    println!("       yai model catalog status");
    println!("       yai model runtime status");
    println!("       yai process observe --pid <pid>");
    println!("       yai process signal --pid <pid> --signal TERM|KILL [--dry-run]");
    println!("       yai observe process --pid <pid>");
    println!("       yai observe compare-process --pid <pid> --expected running|stopped");
    println!("       yai carrier fs-read --sandbox <sandbox> --path <path>");
    println!("       yai carrier fs-write --sandbox <sandbox> --path <path> --content <text>");
}

fn print_carrier_families() {
    println!("carrier_families:");
    println!("- filesystem");
    println!("- process");
    println!("- network_http");
    println!("- database");
    println!("- repository_git");
    println!("- service");
    println!("- endpoint");
    println!("- socket");
    println!("- listener");
    println!("- model_provider");
    println!("- observation");
    println!("- review");
    println!();
    println!("current_status:");
    println!("  filesystem: implemented_minimal carrier.v1");
    println!("  process: implemented_minimal");
    println!("  network_http: skeleton carrier.v1");
    println!("  database: skeleton carrier.v1");
    println!("  repository_git: skeleton carrier.v1");
    println!("  service: skeleton carrier.v1");
    println!("  endpoint: skeleton carrier.v1");
    println!("  socket: skeleton carrier.v1");
    println!("  listener: skeleton carrier.v1");
    println!("  model_provider: skeleton carrier.v1");
    println!("  observation: planned");
    println!("  review: skeleton carrier.v1");
    println!();
    println!("gate_outcomes:");
    println!("- allow");
    println!("- deny");
    println!("- defer");
    println!("- observe_only");
    println!("- require_review");
    println!("- require_evidence");
    println!("- require_redaction");
    println!("- allow_with_constraints");
    println!("- quarantine");
    println!();
    println!("dispatch_statuses:");
    println!("- pending");
    println!("- routable");
    println!("- dispatched");
    println!("- blocked");
    println!("- deferred");
    println!("- failed");
    println!("- not_supported");
    println!();
    println!("receipt_guarantee_modes:");
    println!("- observed");
    println!("- interposed");
    println!("- carrier_owned");
    println!("- embedded");
    println!("- external_import");
    println!();
    println!("next:");
    println!("  inspect_lanes: yai carrier lanes");
    println!("  inspect_route: yai carrier route --family <family>");
    println!("  inspect_coverage: yai carrier coverage");
    println!("  inspect_contract: yai carrier inspect filesystem");
    println!("  inspect_skeleton: yai carrier inspect database");
}

#[derive(Clone, Copy)]
struct CarrierCoverage {
    family: &'static str,
    controlled: &'static str,
    observed: &'static str,
    imported: &'static str,
    execution_available: &'static str,
    execution_scope: &'static str,
    receipt_required: &'static str,
    carrier_contract: &'static str,
    outcomes: &'static [(&'static str, &'static str)],
    note: &'static str,
}

const CARRIER_COVERAGE: &[CarrierCoverage] = &[
    CarrierCoverage {
        family: "filesystem",
        controlled: "active_minimal",
        observed: "skeleton",
        imported: "planned",
        execution_available: "true",
        execution_scope: "filesystem_sandbox_only",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("executed", "supported"),
            ("blocked", "supported"),
            ("failed", "planned"),
            ("mismatch", "planned"),
            ("observed", "supported"),
        ],
        note: "filesystem carrier.v1 active-minimal",
    },
    CarrierCoverage {
        family: "process",
        controlled: "active_minimal",
        observed: "active_minimal",
        imported: "planned",
        execution_available: "true_limited",
        execution_scope: "test_owned_or_safe_policy_only",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("executed", "supported"),
            ("blocked", "supported"),
            ("failed", "supported"),
            ("mismatch", "supported"),
            ("observed", "supported"),
        ],
        note: "process carrier controls only safe/test-owned signal paths",
    },
    CarrierCoverage {
        family: "network_http",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        execution_available: "false",
        execution_scope: "none",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("executed", "planned"),
            ("blocked", "planned"),
            ("failed", "planned"),
            ("mismatch", "planned"),
        ],
        note: "visible skeleton; no network execution",
    },
    CarrierCoverage {
        family: "database",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        execution_available: "false",
        execution_scope: "none",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("executed", "planned"),
            ("blocked", "planned"),
            ("failed", "planned"),
            ("mismatch", "planned"),
        ],
        note: "visible skeleton; no database execution",
    },
    CarrierCoverage {
        family: "repository_git",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        execution_available: "false",
        execution_scope: "none",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("executed", "planned"),
            ("blocked", "planned"),
            ("failed", "planned"),
            ("mismatch", "planned"),
        ],
        note: "visible skeleton; no git execution",
    },
    CarrierCoverage {
        family: "service",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        execution_available: "false",
        execution_scope: "none",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("executed", "planned"),
            ("blocked", "planned"),
            ("failed", "planned"),
            ("mismatch", "planned"),
        ],
        note: "visible skeleton; no service lifecycle control",
    },
    CarrierCoverage {
        family: "endpoint",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        execution_available: "false",
        execution_scope: "none",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("executed", "planned"),
            ("blocked", "planned"),
            ("failed", "planned"),
            ("mismatch", "planned"),
        ],
        note: "visible skeleton; no endpoint execution",
    },
    CarrierCoverage {
        family: "socket",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        execution_available: "false",
        execution_scope: "none",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("executed", "planned"),
            ("blocked", "planned"),
            ("failed", "planned"),
            ("mismatch", "planned"),
        ],
        note: "visible skeleton; no socket control",
    },
    CarrierCoverage {
        family: "listener",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        execution_available: "false",
        execution_scope: "none",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("executed", "planned"),
            ("blocked", "planned"),
            ("failed", "planned"),
            ("mismatch", "planned"),
        ],
        note: "visible skeleton; no listener control",
    },
    CarrierCoverage {
        family: "model_provider",
        controlled: "planned",
        observed: "planned",
        imported: "skeleton",
        execution_available: "false",
        execution_scope: "none",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("observed", "planned"),
            ("failed", "planned"),
            ("mismatch", "planned"),
            ("not_attempted", "supported"),
        ],
        note: "visible skeleton; no model provider carrier execution",
    },
    CarrierCoverage {
        family: "observation",
        controlled: "unsupported",
        observed: "active_minimal",
        imported: "planned",
        execution_available: "false",
        execution_scope: "observation_only",
        receipt_required: "yes",
        carrier_contract: "host_probe.v0",
        outcomes: &[
            ("observed", "supported"),
            ("mismatch", "supported"),
            ("failed", "planned"),
            ("not_attempted", "supported"),
        ],
        note: "observation is not effect execution",
    },
    CarrierCoverage {
        family: "review",
        controlled: "unsupported",
        observed: "unsupported",
        imported: "skeleton",
        execution_available: "false",
        execution_scope: "review_lane_only",
        receipt_required: "yes",
        carrier_contract: "carrier.v1",
        outcomes: &[
            ("deferred", "planned"),
            ("waiting_operator", "planned"),
            ("waiting_agent", "planned"),
            ("not_attempted", "supported"),
        ],
        note: "review is not machine effect execution",
    },
    CarrierCoverage {
        family: "unknown",
        controlled: "unsupported",
        observed: "unsupported",
        imported: "unsupported",
        execution_available: "false",
        execution_scope: "none",
        receipt_required: "no",
        carrier_contract: "none",
        outcomes: &[("not_attempted", "supported")],
        note: "unsupported/unclassified family",
    },
];

fn print_coverage_entry(entry: &CarrierCoverage, list_item: bool, mode_filter: Option<&str>) {
    if list_item {
        println!("- family: {}", entry.family);
        println!("  controlled: {}", entry.controlled);
        println!("  observed: {}", entry.observed);
        println!("  imported: {}", entry.imported);
        println!("  execution_available: {}", entry.execution_available);
        println!("  receipt_required: {}", entry.receipt_required);
        println!("  outcomes:");
        for (outcome, support) in entry.outcomes {
            println!("    {outcome}: {support}");
        }
        println!("  note: {}", entry.note);
        println!();
        return;
    }
    println!("family: {}", entry.family);
    match mode_filter {
        Some("controlled") => println!("controlled: {}", entry.controlled),
        Some("observed") => println!("observed: {}", entry.observed),
        Some("imported") => println!("imported: {}", entry.imported),
        _ => {
            println!("controlled: {}", entry.controlled);
            println!("observed: {}", entry.observed);
            println!("imported: {}", entry.imported);
        }
    }
    println!("execution_available: {}", entry.execution_available);
    println!("execution_scope: {}", entry.execution_scope);
    println!("receipt_required: {}", entry.receipt_required);
    println!("carrier_contract: {}", entry.carrier_contract);
    println!("supported_outcomes:");
    for (outcome, support) in entry.outcomes {
        if *support == "supported" {
            println!("- {outcome}");
        }
    }
    println!("note: {}", entry.note);
}

fn carrier_coverage(args: &[String]) -> Result<(), String> {
    let family_filter = optional_arg(args, "--family");
    let mode_filter = optional_arg(args, "--mode");
    if let Some(mode) = mode_filter.as_deref() {
        if mode != "controlled" && mode != "observed" && mode != "imported" {
            return Err("--mode must be controlled, observed or imported".to_string());
        }
    }
    if let Some(family) = family_filter {
        let entry = CARRIER_COVERAGE
            .iter()
            .find(|entry| entry.family == family)
            .unwrap_or_else(|| {
                CARRIER_COVERAGE
                    .last()
                    .expect("coverage table is non-empty")
            });
        print_coverage_entry(entry, false, mode_filter.as_deref());
        return Ok(());
    }
    println!("carrier_coverage:");
    for entry in CARRIER_COVERAGE {
        if let Some(mode) = mode_filter.as_deref() {
            let status = match mode {
                "controlled" => entry.controlled,
                "observed" => entry.observed,
                "imported" => entry.imported,
                _ => "unknown",
            };
            println!("- family: {}", entry.family);
            println!("  {mode}: {status}");
            println!("  execution_available: {}", entry.execution_available);
            println!("  receipt_required: {}", entry.receipt_required);
            println!();
        } else {
            print_coverage_entry(entry, true, None);
        }
    }
    Ok(())
}

const CARRIER_OUTCOME_TEST_OUTCOMES: &[&str] = &[
    "executed",
    "blocked",
    "deferred",
    "failed",
    "mismatch",
    "observed",
    "quarantined",
    "waiting_operator",
    "waiting_agent",
    "not_attempted",
];

fn normalized_carrier_family(family: &str) -> &'static str {
    CARRIER_COVERAGE
        .iter()
        .find(|entry| entry.family == family)
        .map(|entry| entry.family)
        .unwrap_or("unknown")
}

fn outcome_test_carrier_status(family: &str) -> &'static str {
    match family {
        "filesystem" | "process" | "observation" => "active_minimal",
        "network_http" | "database" | "repository_git" | "service" | "endpoint" | "socket"
        | "listener" | "model_provider" | "review" => "skeleton",
        _ => "unsupported",
    }
}

fn outcome_test_execution_available(family: &str) -> &'static str {
    match family {
        "filesystem" => "true",
        "process" => "true_limited",
        _ => "false",
    }
}

fn is_skeleton_outcome_family(family: &str) -> bool {
    matches!(
        family,
        "network_http"
            | "database"
            | "repository_git"
            | "service"
            | "endpoint"
            | "socket"
            | "listener"
            | "model_provider"
            | "review"
    )
}

fn outcome_test_reason(family: &str, effective_outcome: &str) -> &'static str {
    if family == "unknown" {
        return "unsupported_carrier_family";
    }
    if effective_outcome == "mismatch" {
        return "simulated_mismatch_posture";
    }
    if is_skeleton_outcome_family(family) {
        return match effective_outcome {
            "failed" => "simulated_failure_posture",
            "deferred" => "simulated_deferred_posture",
            "observed" => "simulated_observed_posture",
            "quarantined" => "simulated_quarantine_posture",
            "waiting_operator" | "waiting_agent" => "simulated_waiting_posture",
            "not_attempted" => "not_attempted",
            _ => "skeleton_carrier_no_execution",
        };
    }
    if family == "process" && effective_outcome == "blocked" {
        return "unsafe_or_policy_blocked";
    }
    match effective_outcome {
        "executed" => "active_carrier_harness_dry_run",
        "blocked" => "policy_blocked_harness",
        "deferred" => "simulated_deferred_posture",
        "failed" => "simulated_failure_posture",
        "observed" => "simulated_observed_posture",
        "quarantined" => "simulated_quarantine_posture",
        "waiting_operator" | "waiting_agent" => "simulated_waiting_posture",
        "not_attempted" => "not_attempted",
        _ => "unsupported_outcome",
    }
}

fn carrier_outcome_test(args: &[String]) -> Result<(), String> {
    let requested_family =
        optional_arg(args, "--family").ok_or_else(|| "--family is required".to_string())?;
    let mode = optional_arg(args, "--mode").unwrap_or_else(|| "controlled".to_string());
    let requested_outcome =
        optional_arg(args, "--outcome").ok_or_else(|| "--outcome is required".to_string())?;

    if !matches!(mode.as_str(), "controlled" | "observed" | "imported") {
        return Err("unsupported_mode".to_string());
    }
    if !CARRIER_OUTCOME_TEST_OUTCOMES.contains(&requested_outcome.as_str()) {
        return Err("unsupported_outcome".to_string());
    }

    let family = normalized_carrier_family(&requested_family);
    let effective_outcome = if family == "unknown" {
        "not_attempted"
    } else {
        requested_outcome.as_str()
    };
    let divergence_candidate = if effective_outcome == "mismatch" {
        "generated"
    } else {
        "none"
    };

    println!("family: {family}");
    println!("mode: {mode}");
    println!("requested_outcome: {requested_outcome}");
    println!("effective_outcome: {effective_outcome}");
    println!("carrier_status: {}", outcome_test_carrier_status(family));
    println!(
        "execution_available: {}",
        outcome_test_execution_available(family)
    );
    println!("execution_performed: false");
    println!("carrier_attempted: false");
    println!(
        "receipt_required: {}",
        if family == "unknown" { "no" } else { "yes" }
    );
    println!(
        "receipt_posture: {}",
        if family == "unknown" {
            "none"
        } else {
            "simulated"
        }
    );
    println!("divergence_candidate: {divergence_candidate}");
    println!("reason: {}", outcome_test_reason(family, effective_outcome));
    Ok(())
}

const CARRIER_RECONCILE_SCENARIOS: &[&str] = &[
    "denied_but_attempted",
    "executed_without_receipt",
    "blocked_but_effect_observed",
    "receipt_claimed_executed_but_not_observed",
    "failed_with_partial_effect",
    "skeleton_executed_unexpectedly",
    "clean_executed",
    "clean_blocked",
    "clean_observed",
];

const CARRIER_RECONCILE_DECISIONS: &[&str] = &[
    "allow",
    "deny",
    "defer",
    "observe_only",
    "require_review",
    "require_evidence",
    "require_redaction",
    "allow_with_constraints",
    "quarantine",
    "unknown",
];

const CARRIER_RECONCILE_DISPATCH: &[&str] = &[
    "pending",
    "routable",
    "dispatched",
    "blocked",
    "deferred",
    "failed",
    "not_supported",
    "unknown",
];

const CARRIER_RECONCILE_OBSERVATIONS: &[&str] = &[
    "matched",
    "mismatch",
    "not_observed",
    "not_found",
    "permission_denied",
    "unknown",
];

#[derive(Clone)]
struct CarrierConsistencyInput {
    scenario: String,
    decision: String,
    dispatch: String,
    carrier_outcome: String,
    receipt_present: bool,
    carrier_attempted: bool,
    execution_performed: bool,
    observation: String,
    receipt_required: bool,
    expected_effect: bool,
    observed_effect: bool,
    skeleton_carrier: bool,
}

fn bool_arg(value: &str) -> Result<bool, String> {
    match value {
        "yes" | "true" => Ok(true),
        "no" | "false" => Ok(false),
        _ => Err("expected yes|no".to_string()),
    }
}

fn bool_word(value: bool) -> &'static str {
    if value {
        "yes"
    } else {
        "no"
    }
}

fn decision_is_deny_like(decision: &str) -> bool {
    matches!(
        decision,
        "deny" | "quarantine" | "require_review" | "require_evidence" | "require_redaction"
    )
}

fn default_expected_effect(decision: &str, carrier_outcome: &str) -> bool {
    !decision_is_deny_like(decision)
        && matches!(carrier_outcome, "executed" | "failed" | "mismatch")
}

fn default_observed_effect(
    carrier_outcome: &str,
    execution_performed: bool,
    observation: &str,
) -> bool {
    match (carrier_outcome, observation) {
        ("blocked", "mismatch") | ("failed", "mismatch") => true,
        ("executed", "mismatch") | ("executed", "not_found") | ("executed", "not_observed") => {
            false
        }
        _ => execution_performed && observation == "matched",
    }
}

fn carrier_reconcile_scenario_input(scenario: &str) -> Result<CarrierConsistencyInput, String> {
    if !CARRIER_RECONCILE_SCENARIOS.contains(&scenario) {
        return Err("unsupported_scenario".to_string());
    }

    let mut input = CarrierConsistencyInput {
        scenario: scenario.to_string(),
        decision: "allow".to_string(),
        dispatch: "dispatched".to_string(),
        carrier_outcome: "executed".to_string(),
        receipt_present: true,
        carrier_attempted: true,
        execution_performed: true,
        observation: "matched".to_string(),
        receipt_required: true,
        expected_effect: true,
        observed_effect: true,
        skeleton_carrier: false,
    };

    match scenario {
        "clean_executed" => {}
        "clean_blocked" => {
            input.decision = "deny".to_string();
            input.dispatch = "blocked".to_string();
            input.carrier_outcome = "blocked".to_string();
            input.carrier_attempted = false;
            input.execution_performed = false;
            input.observation = "not_observed".to_string();
            input.expected_effect = false;
            input.observed_effect = false;
        }
        "clean_observed" => {
            input.decision = "observe_only".to_string();
            input.carrier_outcome = "observed".to_string();
            input.carrier_attempted = false;
            input.execution_performed = false;
            input.expected_effect = false;
            input.observed_effect = false;
        }
        "denied_but_attempted" => {
            input.decision = "deny".to_string();
            input.dispatch = "blocked".to_string();
        }
        "executed_without_receipt" => {
            input.receipt_present = false;
        }
        "blocked_but_effect_observed" => {
            input.decision = "deny".to_string();
            input.dispatch = "blocked".to_string();
            input.carrier_outcome = "blocked".to_string();
            input.carrier_attempted = false;
            input.execution_performed = false;
            input.observation = "mismatch".to_string();
            input.expected_effect = false;
            input.observed_effect = true;
        }
        "receipt_claimed_executed_but_not_observed" => {
            input.observation = "mismatch".to_string();
            input.observed_effect = false;
        }
        "failed_with_partial_effect" => {
            input.carrier_outcome = "failed".to_string();
            input.observation = "mismatch".to_string();
            input.observed_effect = true;
        }
        "skeleton_executed_unexpectedly" => {
            input.skeleton_carrier = true;
        }
        _ => {}
    }
    Ok(input)
}

fn carrier_reconcile_explicit_input(args: &[String]) -> Result<CarrierConsistencyInput, String> {
    let decision =
        optional_arg(args, "--decision").ok_or_else(|| "--decision is required".to_string())?;
    let dispatch =
        optional_arg(args, "--dispatch").ok_or_else(|| "--dispatch is required".to_string())?;
    let carrier_outcome = optional_arg(args, "--carrier-outcome")
        .ok_or_else(|| "--carrier-outcome is required".to_string())?;
    let receipt_present = optional_arg(args, "--receipt-present")
        .ok_or_else(|| "--receipt-present is required".to_string())?;
    let carrier_attempted = optional_arg(args, "--carrier-attempted")
        .ok_or_else(|| "--carrier-attempted is required".to_string())?;
    let execution_performed = optional_arg(args, "--execution-performed")
        .ok_or_else(|| "--execution-performed is required".to_string())?;
    let observation = optional_arg(args, "--observation")
        .ok_or_else(|| "--observation is required".to_string())?;

    if !CARRIER_RECONCILE_DECISIONS.contains(&decision.as_str()) {
        return Err("unsupported_decision".to_string());
    }
    if !CARRIER_RECONCILE_DISPATCH.contains(&dispatch.as_str()) {
        return Err("unsupported_dispatch".to_string());
    }
    if !CARRIER_OUTCOME_TEST_OUTCOMES.contains(&carrier_outcome.as_str()) {
        return Err("unsupported_carrier_outcome".to_string());
    }
    if !CARRIER_RECONCILE_OBSERVATIONS.contains(&observation.as_str()) {
        return Err("unsupported_observation".to_string());
    }

    let receipt_present = bool_arg(&receipt_present)?;
    let carrier_attempted = bool_arg(&carrier_attempted)?;
    let execution_performed = bool_arg(&execution_performed)?;
    let receipt_required = optional_arg(args, "--receipt-required")
        .map(|value| bool_arg(&value))
        .transpose()?
        .unwrap_or(true);
    let expected_effect = optional_arg(args, "--expected-effect")
        .map(|value| bool_arg(&value))
        .transpose()?
        .unwrap_or_else(|| default_expected_effect(&decision, &carrier_outcome));
    let observed_effect = optional_arg(args, "--observed-effect")
        .map(|value| bool_arg(&value))
        .transpose()?
        .unwrap_or_else(|| {
            default_observed_effect(&carrier_outcome, execution_performed, &observation)
        });
    let skeleton_carrier = optional_arg(args, "--skeleton-carrier")
        .map(|value| bool_arg(&value))
        .transpose()?
        .unwrap_or(false);

    Ok(CarrierConsistencyInput {
        scenario: "explicit".to_string(),
        decision,
        dispatch,
        carrier_outcome,
        receipt_present,
        carrier_attempted,
        execution_performed,
        observation,
        receipt_required,
        expected_effect,
        observed_effect,
        skeleton_carrier,
    })
}

fn carrier_consistency_divergence(input: &CarrierConsistencyInput) -> (&'static str, &'static str) {
    if input.skeleton_carrier && (input.execution_performed || input.carrier_attempted) {
        return ("skeleton_executed_unexpectedly", "critical");
    }
    if decision_is_deny_like(&input.decision) && input.carrier_attempted {
        return ("denied_but_attempted", "critical");
    }
    if input.carrier_outcome == "executed" && input.receipt_required && !input.receipt_present {
        return ("executed_without_receipt", "error");
    }
    if input.carrier_outcome == "blocked" && input.observed_effect {
        return ("blocked_but_effect_observed", "critical");
    }
    if input.carrier_outcome == "executed"
        && !input.observed_effect
        && matches!(
            input.observation.as_str(),
            "mismatch" | "not_found" | "not_observed"
        )
    {
        return ("receipt_claimed_executed_but_not_observed", "error");
    }
    if input.carrier_outcome == "failed" && input.observed_effect {
        return ("failed_with_partial_effect", "error");
    }
    if input.carrier_outcome == "mismatch" {
        return ("generated", "warning");
    }
    if input.receipt_required && !input.receipt_present && input.carrier_outcome != "not_attempted"
    {
        return ("missing_receipt", "error");
    }
    if input.decision == "unknown" && input.receipt_present {
        return ("receipt_without_decision", "warning");
    }
    ("none", "info")
}

fn receipt_posture(input: &CarrierConsistencyInput) -> &'static str {
    if !input.receipt_required {
        "none"
    } else if input.receipt_present {
        "present"
    } else {
        "missing"
    }
}

fn carrier_reconcile_outcome(args: &[String]) -> Result<(), String> {
    let input = if let Some(scenario) = optional_arg(args, "--scenario") {
        carrier_reconcile_scenario_input(&scenario)?
    } else {
        carrier_reconcile_explicit_input(args)?
    };
    let (divergence_candidate, severity) = carrier_consistency_divergence(&input);
    let result = if divergence_candidate == "none" {
        "consistent"
    } else {
        "inconsistent"
    };

    println!("scenario: {}", input.scenario);
    println!("decision: {}", input.decision);
    println!("dispatch: {}", input.dispatch);
    println!("carrier_outcome: {}", input.carrier_outcome);
    println!("receipt_required: {}", bool_word(input.receipt_required));
    println!("receipt_present: {}", bool_word(input.receipt_present));
    println!("receipt_posture: {}", receipt_posture(&input));
    println!("observation: {}", input.observation);
    println!("carrier_attempted: {}", input.carrier_attempted);
    println!("execution_performed: {}", input.execution_performed);
    println!("expected_effect: {}", bool_word(input.expected_effect));
    println!("observed_effect: {}", bool_word(input.observed_effect));
    println!("divergence_candidate: {divergence_candidate}");
    println!("severity: {severity}");
    println!("result: {result}");
    println!("execution_side_effect: none");
    Ok(())
}

#[derive(Clone, Copy)]
struct CarrierLane {
    lane: &'static str,
    carrier_family: &'static str,
    status: &'static str,
    ordering_policy: &'static str,
    capacity_policy: &'static str,
    lock_policy: &'static str,
    timeout_policy: &'static str,
    retry_policy: &'static str,
    receipt_requirement: &'static str,
    dispatch_status: &'static str,
}

const CARRIER_LANES: &[CarrierLane] = &[
    CarrierLane {
        lane: "filesystem_lane",
        carrier_family: "filesystem",
        status: "active_minimal",
        ordering_policy: "serial_per_case",
        capacity_policy: "single_inflight",
        lock_policy: "case_lock",
        timeout_policy: "normal",
        retry_policy: "safe_retry",
        receipt_requirement: "required",
        dispatch_status: "routable",
    },
    CarrierLane {
        lane: "process_lane",
        carrier_family: "process",
        status: "active_minimal",
        ordering_policy: "serial_per_case",
        capacity_policy: "single_inflight",
        lock_policy: "target_lock",
        timeout_policy: "normal",
        retry_policy: "requires_review",
        receipt_requirement: "required",
        dispatch_status: "routable",
    },
    CarrierLane {
        lane: "network_http_lane",
        carrier_family: "network_http",
        status: "planned",
        ordering_policy: "serial_per_case",
        capacity_policy: "single_inflight",
        lock_policy: "carrier_lock",
        timeout_policy: "normal",
        retry_policy: "requires_review",
        receipt_requirement: "required",
        dispatch_status: "routable",
    },
    CarrierLane {
        lane: "database_lane",
        carrier_family: "database",
        status: "planned",
        ordering_policy: "serial_per_case",
        capacity_policy: "single_inflight",
        lock_policy: "target_lock",
        timeout_policy: "normal",
        retry_policy: "requires_review",
        receipt_requirement: "required",
        dispatch_status: "routable",
    },
    CarrierLane {
        lane: "repository_git_lane",
        carrier_family: "repository_git",
        status: "planned",
        ordering_policy: "serial_per_case",
        capacity_policy: "single_inflight",
        lock_policy: "target_lock",
        timeout_policy: "normal",
        retry_policy: "requires_review",
        receipt_requirement: "required",
        dispatch_status: "routable",
    },
    CarrierLane {
        lane: "model_provider_lane",
        carrier_family: "model_provider",
        status: "planned",
        ordering_policy: "serial_per_case",
        capacity_policy: "single_inflight",
        lock_policy: "carrier_lock",
        timeout_policy: "normal",
        retry_policy: "requires_review",
        receipt_requirement: "required",
        dispatch_status: "routable",
    },
    CarrierLane {
        lane: "observation_lane",
        carrier_family: "observation",
        status: "planned",
        ordering_policy: "parallel_by_subject",
        capacity_policy: "single_inflight",
        lock_policy: "none",
        timeout_policy: "normal",
        retry_policy: "none",
        receipt_requirement: "required",
        dispatch_status: "routable",
    },
    CarrierLane {
        lane: "review_lane",
        carrier_family: "review",
        status: "planned",
        ordering_policy: "serial_per_case",
        capacity_policy: "single_inflight",
        lock_policy: "case_lock",
        timeout_policy: "manual",
        retry_policy: "manual_retry",
        receipt_requirement: "required",
        dispatch_status: "routable",
    },
];

fn print_carrier_lanes() {
    println!("carrier_lanes:");
    for lane in CARRIER_LANES {
        println!("- lane: {}", lane.lane);
        println!("  carrier_family: {}", lane.carrier_family);
        println!("  status: {}", lane.status);
        println!("  ordering_policy: {}", lane.ordering_policy);
        println!("  capacity_policy: {}", lane.capacity_policy);
        println!("  lock_policy: {}", lane.lock_policy);
        println!("  timeout_policy: {}", lane.timeout_policy);
        println!("  retry_policy: {}", lane.retry_policy);
        println!("  receipt_requirement: {}", lane.receipt_requirement);
        println!("  execution_performed: false");
        println!();
    }
}

fn carrier_route(args: &[String]) -> Result<(), String> {
    let family =
        optional_arg(args, "--family").ok_or_else(|| "--family is required".to_string())?;
    let family = family.as_str();
    if let Some(lane) = CARRIER_LANES
        .iter()
        .find(|lane| lane.carrier_family == family && family != "unknown")
    {
        println!("carrier_family: {}", lane.carrier_family);
        println!("lane: {}", lane.lane);
        println!("dispatch_status: {}", lane.dispatch_status);
        println!("lane_status: {}", lane.status);
        println!("execution_performed: false");
        println!("receipt_requirement: {}", lane.receipt_requirement);
        println!("ordering_policy: {}", lane.ordering_policy);
        println!("capacity_policy: {}", lane.capacity_policy);
        println!("lock_policy: {}", lane.lock_policy);
        println!("timeout_policy: {}", lane.timeout_policy);
        println!("retry_policy: {}", lane.retry_policy);
        return Ok(());
    }
    println!("carrier_family: unknown");
    println!("lane: unknown_lane");
    println!("dispatch_status: not_supported");
    println!("lane_status: not_supported");
    println!("execution_performed: false");
    println!("receipt_requirement: not_available");
    Ok(())
}

#[derive(Clone, Copy)]
struct CarrierSkeleton {
    carrier: &'static str,
    carrier_family: &'static str,
    adapter_status: &'static str,
    controlled: &'static str,
    observed: &'static str,
    imported: &'static str,
    non_execution_reason: &'static str,
    future_activation_wave: &'static str,
}

const CARRIER_SKELETONS: &[CarrierSkeleton] = &[
    CarrierSkeleton {
        carrier: "network_http",
        carrier_family: "network_http",
        adapter_status: "skeleton",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        non_execution_reason: "adapter_not_implemented",
        future_activation_wave: "planned",
    },
    CarrierSkeleton {
        carrier: "database",
        carrier_family: "database",
        adapter_status: "skeleton",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        non_execution_reason: "adapter_not_implemented",
        future_activation_wave: "planned",
    },
    CarrierSkeleton {
        carrier: "repository_git",
        carrier_family: "repository_git",
        adapter_status: "skeleton",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        non_execution_reason: "adapter_not_implemented",
        future_activation_wave: "planned",
    },
    CarrierSkeleton {
        carrier: "service",
        carrier_family: "service",
        adapter_status: "skeleton",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        non_execution_reason: "adapter_not_implemented",
        future_activation_wave: "planned",
    },
    CarrierSkeleton {
        carrier: "endpoint",
        carrier_family: "endpoint",
        adapter_status: "skeleton",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        non_execution_reason: "adapter_not_implemented",
        future_activation_wave: "planned",
    },
    CarrierSkeleton {
        carrier: "socket",
        carrier_family: "socket",
        adapter_status: "skeleton",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        non_execution_reason: "adapter_not_implemented",
        future_activation_wave: "planned",
    },
    CarrierSkeleton {
        carrier: "listener",
        carrier_family: "listener",
        adapter_status: "skeleton",
        controlled: "skeleton",
        observed: "skeleton",
        imported: "skeleton",
        non_execution_reason: "adapter_not_implemented",
        future_activation_wave: "planned",
    },
    CarrierSkeleton {
        carrier: "model_provider",
        carrier_family: "model_provider",
        adapter_status: "skeleton",
        controlled: "planned",
        observed: "planned",
        imported: "skeleton",
        non_execution_reason: "model_provider_carrier_not_implemented",
        future_activation_wave: "SPINE.93A+",
    },
    CarrierSkeleton {
        carrier: "review",
        carrier_family: "review",
        adapter_status: "skeleton",
        controlled: "unsupported",
        observed: "unsupported",
        imported: "skeleton",
        non_execution_reason: "review_lane_not_implemented",
        future_activation_wave: "planned",
    },
];

fn print_carrier_skeleton(skeleton: &CarrierSkeleton) {
    println!("carrier: {}", skeleton.carrier);
    println!("carrier_family: {}", skeleton.carrier_family);
    println!("contract: carrier.v1");
    println!("status: {}", skeleton.adapter_status);
    println!("controlled: {}", skeleton.controlled);
    println!("observed: {}", skeleton.observed);
    println!("imported: {}", skeleton.imported);
    println!("execution_available: false");
    println!("receipt_required: yes");
    println!("supports_inspect: yes");
    println!("non_execution_reason: {}", skeleton.non_execution_reason);
    println!(
        "future_activation_wave: {}",
        skeleton.future_activation_wave
    );
    println!("carrier_attempted: false");
}

fn carrier_inspect(args: &[String]) -> Result<(), String> {
    let name = args
        .first()
        .filter(|value| !value.starts_with("--"))
        .ok_or_else(|| "carrier name is required".to_string())?;
    match name.as_str() {
        "filesystem" => {
            println!("carrier: filesystem");
            println!("carrier_family: filesystem");
            println!("contract: carrier.v1");
            println!("status: active_minimal");
            println!("lane: filesystem_lane");
            println!("dispatch_status: routable");
            println!("supports:");
            println!("  read: observed");
            println!("  write: decision_required");
            println!("receipt_required: yes");
            println!("pre_state_observation: supported");
            println!("post_state_observation: supported");
            println!("evidence_capture: supported");
            println!("receipt_assembly: supported");
            println!("receipt_validation: supported");
            println!("residue_recording: supported");
            println!("guarantee_mode: interposed");
            println!("process_execution: not_supported");
            println!("network_execution: not_supported");
            Ok(())
        }
        "process" => {
            println!("carrier: process");
            println!("carrier_family: process");
            println!("contract: carrier.v1");
            println!("status: active_minimal");
            println!("lane: process_lane");
            println!("dispatch_status: routable");
            println!("supports:");
            println!("  observe: observed");
            println!("  signal_TERM: decision_required");
            println!("  signal_KILL: test_owned_only");
            println!("receipt_required: yes");
            println!("pre_state_observation: supported");
            println!("post_state_observation: supported");
            println!("evidence_capture: supported");
            println!("receipt_validation: supported");
            println!("guarantee_mode: interposed");
            println!("platform: posix");
            Ok(())
        }
        _ => {
            if let Some(skeleton) = CARRIER_SKELETONS
                .iter()
                .find(|skeleton| skeleton.carrier == name.as_str())
            {
                print_carrier_skeleton(skeleton);
                return Ok(());
            }
            println!("carrier: unknown");
            println!("status: not_supported");
            println!("dispatch_status: not_supported");
            println!("execution_performed: false");
            Ok(())
        }
    }
}

fn parse_pid_arg(args: &[String]) -> Result<i32, String> {
    let value = optional_arg(args, "--pid").ok_or_else(|| "--pid is required".to_string())?;
    value
        .parse::<i32>()
        .map_err(|_| "--pid must be an integer".to_string())
}

#[cfg(unix)]
fn process_state_for_pid(pid: i32) -> &'static str {
    if pid <= 0 {
        return "not_found";
    }
    let result = unsafe { kill(pid as c_int, 0) };
    if result == 0 {
        return "running";
    }
    if let Some(errno) = std::io::Error::last_os_error().raw_os_error() {
        if errno == 3 {
            return "not_found";
        }
        if errno == 1 {
            return "permission_denied";
        }
    }
    let proc_path = PathBuf::from(format!("/proc/{pid}"));
    if PathBuf::from("/proc").is_dir() && !proc_path.exists() {
        return "not_found";
    }
    "unknown"
}

#[cfg(not(unix))]
fn process_state_for_pid(_pid: i32) -> &'static str {
    "unknown"
}

fn observed_result_for_state(state: &str) -> &'static str {
    match state {
        "running" => "matched",
        "not_found" => "not_found",
        "permission_denied" => "permission_denied",
        "unknown" => "unknown",
        _ => "unknown",
    }
}

fn expected_matches_observed(expected: &str, observed: &str) -> bool {
    expected == observed || (expected == "stopped" && observed != "running")
}

fn divergence_candidate_for(expected: &str, observed: &str) -> &'static str {
    match (expected, observed) {
        ("stopped", "running") => "expected_stopped_but_running",
        ("running", "not_found") => "expected_running_but_not_found",
        _ => "unknown",
    }
}

fn process_observe(args: &[String]) -> Result<(), String> {
    let pid = parse_pid_arg(args)?;
    let state = process_state_for_pid(pid);
    println!("process_ref: process:{pid}");
    println!("pid: {pid}");
    println!("state: {state}");
    println!("owner_scope: external_observed");
    println!("carrier_family: process");
    println!("outcome: observed");
    println!("receipt_required: yes");
    Ok(())
}

fn observe_process(args: &[String]) -> Result<(), String> {
    let pid = parse_pid_arg(args)?;
    let state = process_state_for_pid(pid);
    println!("observation_target: process");
    println!("pid: {pid}");
    println!("result: {}", observed_result_for_state(state));
    println!("observed_state: {state}");
    println!("enforcement: none");
    println!("observation_is_enforcement: false");
    Ok(())
}

fn observe_compare_process(args: &[String]) -> Result<(), String> {
    let pid = parse_pid_arg(args)?;
    let expected =
        optional_arg(args, "--expected").ok_or_else(|| "--expected is required".to_string())?;
    if expected != "running" && expected != "stopped" {
        return Err("--expected must be running or stopped".to_string());
    }
    let observed = process_state_for_pid(pid);
    let matched = expected_matches_observed(&expected, observed);
    println!("observation_target: process");
    println!("pid: {pid}");
    println!("expected_state: {expected}");
    println!("observed_state: {observed}");
    println!("result: {}", if matched { "matched" } else { "mismatch" });
    println!("enforcement: none");
    println!("observation_is_enforcement: false");
    if !matched {
        println!(
            "divergence_candidate: {}",
            divergence_candidate_for(&expected, observed)
        );
        println!("severity: warning");
        println!("silent_repair: false");
    }
    Ok(())
}

fn process_signal(args: &[String]) -> Result<(), String> {
    let pid = parse_pid_arg(args)?;
    let signal =
        optional_arg(args, "--signal").ok_or_else(|| "--signal is required".to_string())?;
    let signal = signal.to_uppercase();
    let dry_run = args.iter().any(|arg| arg == "--dry-run");
    println!("op: process.signal");
    println!("pid: {pid}");
    println!("signal: {signal}");
    if dry_run {
        println!("dry_run: true");
        println!("carrier_family: process");
        println!("lane: process_lane");
        println!("dispatch_status: routable");
        println!("decision_required: true");
        println!("carrier_attempted: false");
        println!("expected_receipt: process_signal_receipt");
        return Ok(());
    }
    println!("decision: deny");
    println!("carrier_attempted: false");
    println!("outcome: blocked");
    println!("reason: unsafe_process_target");
    Ok(())
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

fn yai_config_dir() -> PathBuf {
    yai_home().join("config")
}

fn provider_run_dir() -> PathBuf {
    yai_home().join("run").join("providers")
}

fn provider_log_dir() -> PathBuf {
    yai_home().join("log").join("providers")
}

fn provider_plan_dir() -> PathBuf {
    yai_home().join("tmp").join("provider-plans")
}

fn device_registry_path() -> PathBuf {
    yai_config_dir().join("devices.jsonl")
}

fn ensure_provider_runtime_dirs() -> Result<(), String> {
    for path in [
        yai_config_dir(),
        provider_run_dir(),
        provider_log_dir(),
        provider_plan_dir(),
    ] {
        fs::create_dir_all(&path)
            .map_err(|error| format!("failed to create {}: {error}", path.display()))?;
    }
    Ok(())
}

#[derive(Clone, Debug)]
struct DeviceEntry {
    device_id: String,
    name: String,
    target_mode: String,
    host: String,
    port: u16,
    pairing_status: String,
    trust_status: String,
    os: String,
    arch: String,
    gpu_backend: String,
    provider_supervisor_status: String,
    notes: String,
}

impl DeviceEntry {
    fn to_jsonl(&self) -> String {
        format!(
            "{{\"device_id\":\"{}\",\"name\":\"{}\",\"target_mode\":\"{}\",\"host\":\"{}\",\"port\":{},\"pairing_status\":\"{}\",\"trust_status\":\"{}\",\"os\":\"{}\",\"arch\":\"{}\",\"gpu_backend\":\"{}\",\"provider_supervisor_status\":\"{}\",\"notes\":\"{}\"}}\n",
            json_escape(&self.device_id),
            json_escape(&self.name),
            json_escape(&self.target_mode),
            json_escape(&self.host),
            self.port,
            json_escape(&self.pairing_status),
            json_escape(&self.trust_status),
            json_escape(&self.os),
            json_escape(&self.arch),
            json_escape(&self.gpu_backend),
            json_escape(&self.provider_supervisor_status),
            json_escape(&self.notes)
        )
    }
}

fn validate_runtime_target(value: &str) -> Result<(), String> {
    if matches!(value, "local" | "lan" | "external") {
        Ok(())
    } else {
        Err("target must be local, lan or external".to_string())
    }
}

fn parse_device_line(line: &str) -> Option<DeviceEntry> {
    let device_id = extract_json_string_field(line, "device_id")?;
    let port = json_number_field(line, "port")?.parse::<u16>().ok()?;
    Some(DeviceEntry {
        device_id,
        name: extract_json_string_field(line, "name").unwrap_or_else(|| "unknown".to_string()),
        target_mode: extract_json_string_field(line, "target_mode")
            .unwrap_or_else(|| "lan".to_string()),
        host: extract_json_string_field(line, "host").unwrap_or_else(|| "unknown".to_string()),
        port,
        pairing_status: extract_json_string_field(line, "pairing_status")
            .unwrap_or_else(|| "unknown".to_string()),
        trust_status: extract_json_string_field(line, "trust_status")
            .unwrap_or_else(|| "unknown".to_string()),
        os: extract_json_string_field(line, "os").unwrap_or_else(|| "unknown".to_string()),
        arch: extract_json_string_field(line, "arch").unwrap_or_else(|| "unknown".to_string()),
        gpu_backend: extract_json_string_field(line, "gpu_backend")
            .unwrap_or_else(|| "unknown".to_string()),
        provider_supervisor_status: extract_json_string_field(line, "provider_supervisor_status")
            .unwrap_or_else(|| "not_checked".to_string()),
        notes: extract_json_string_field(line, "notes").unwrap_or_default(),
    })
}

fn read_device_registry() -> Result<Vec<DeviceEntry>, String> {
    let path = device_registry_path();
    if !path.is_file() {
        return Ok(Vec::new());
    }
    let content = fs::read_to_string(&path)
        .map_err(|error| format!("failed to read {}: {error}", path.display()))?;
    Ok(content.lines().filter_map(parse_device_line).collect())
}

fn write_device_registry(devices: &[DeviceEntry]) -> Result<(), String> {
    ensure_provider_runtime_dirs()?;
    let path = device_registry_path();
    let mut file = fs::File::create(&path)
        .map_err(|error| format!("failed to open {}: {error}", path.display()))?;
    for device in devices {
        file.write_all(device.to_jsonl().as_bytes())
            .map_err(|error| format!("failed to write {}: {error}", path.display()))?;
    }
    Ok(())
}

fn print_device_entry(device: &DeviceEntry, list_item: bool) {
    let prefix = if list_item { "  " } else { "" };
    if list_item {
        println!("- device_id: {}", device.device_id);
    } else {
        println!("device_id: {}", device.device_id);
    }
    println!("{prefix}name: {}", device.name);
    println!("{prefix}target_mode: {}", device.target_mode);
    println!("{prefix}host: {}", device.host);
    println!("{prefix}port: {}", device.port);
    println!("{prefix}pairing_status: {}", device.pairing_status);
    println!("{prefix}trust_status: {}", device.trust_status);
    println!(
        "{prefix}provider_supervisor_status: {}",
        device.provider_supervisor_status
    );
    if !list_item {
        println!("os: {}", device.os);
        println!("arch: {}", device.arch);
        println!("gpu_backend: {}", device.gpu_backend);
        println!("model_catalog: not_scanned");
        println!(
            "notes: {}",
            if device.notes.is_empty() {
                "none"
            } else {
                device.notes.as_str()
            }
        );
    }
}

fn device_add(args: &[String]) -> Result<(), String> {
    let device_id = named_arg(args, "--id")?;
    let name = named_arg(args, "--name")?;
    let host = named_arg(args, "--host")?;
    let port_text = named_arg(args, "--port")?;
    let target_mode = named_arg(args, "--target")?;
    validate_runtime_target(&target_mode)?;
    let port = port_text
        .parse::<u16>()
        .map_err(|_| "--port must be an integer from 0 to 65535".to_string())?;
    let entry = DeviceEntry {
        device_id: device_id.clone(),
        name,
        target_mode: target_mode.clone(),
        host,
        port,
        pairing_status: if target_mode == "lan" {
            "paired".to_string()
        } else {
            "unknown".to_string()
        },
        trust_status: "untrusted".to_string(),
        os: optional_arg(args, "--os").unwrap_or_else(|| "unknown".to_string()),
        arch: optional_arg(args, "--arch").unwrap_or_else(|| "unknown".to_string()),
        gpu_backend: optional_arg(args, "--gpu-backend").unwrap_or_else(|| "unknown".to_string()),
        provider_supervisor_status: "not_checked".to_string(),
        notes: optional_arg(args, "--notes").unwrap_or_default(),
    };
    let mut devices = read_device_registry()?;
    devices.retain(|device| device.device_id != device_id);
    devices.push(entry.clone());
    write_device_registry(&devices)?;
    println!("device:add ok");
    println!("device_registry: {}", device_registry_path().display());
    print_device_entry(&entry, false);
    Ok(())
}

fn device_list() -> Result<(), String> {
    let devices = read_device_registry()?;
    if devices.is_empty() {
        println!("devices: none");
        println!("device_registry: {}", device_registry_path().display());
        return Ok(());
    }
    println!("devices:");
    for device in &devices {
        print_device_entry(device, true);
    }
    println!("device_registry: {}", device_registry_path().display());
    Ok(())
}

fn device_inspect(args: &[String]) -> Result<(), String> {
    let device_id = args
        .first()
        .filter(|value| !value.starts_with("--"))
        .ok_or_else(|| "device_id is required".to_string())?;
    let devices = read_device_registry()?;
    let Some(device) = devices.iter().find(|device| &device.device_id == device_id) else {
        println!("device: not_found");
        println!("device_id: {device_id}");
        println!("device_registry: {}", device_registry_path().display());
        return Ok(());
    };
    print_device_entry(device, false);
    Ok(())
}

fn set_device_trust(args: &[String], trust_status: &str) -> Result<(), String> {
    let device_id = args
        .first()
        .filter(|value| !value.starts_with("--"))
        .ok_or_else(|| "device_id is required".to_string())?;
    let mut devices = read_device_registry()?;
    let Some(device) = devices
        .iter_mut()
        .find(|device| device.device_id == *device_id)
    else {
        println!("device: not_found");
        println!("device_id: {device_id}");
        println!("device_registry: {}", device_registry_path().display());
        return Ok(());
    };
    device.trust_status = trust_status.to_string();
    let updated = device.clone();
    write_device_registry(&devices)?;
    println!("device_id: {}", updated.device_id);
    println!("trust_status: {}", updated.trust_status);
    println!("device_registry: {}", device_registry_path().display());
    Ok(())
}

fn print_provider_runtime_status() {
    println!("provider_runtime:");
    println!("  status: surface_v0");
    println!("  execution: dry_run_only");
    println!("  provider_supervision: planned");
    println!("  device_registry: active");
    println!("  runtime_targets:");
    println!("  - local");
    println!("  - lan");
    println!("  - external");
    println!("local:");
    println!("  supervisor: planned");
    println!("lan:");
    println!("  requires_paired_device: yes");
    println!("  requires_trusted_device: yes");
    println!("  supervisor: planned");
    println!("external:");
    println!("  attach_only: yes");
}

fn print_provider_targets() {
    println!("runtime_targets:");
    println!("- local");
    println!("- lan");
    println!("- external");
    println!("local:");
    println!("  provider_process: same_machine");
    println!("  supervisor: planned");
    println!("lan:");
    println!("  device_is_runtime_target: yes");
    println!("  requires_paired_device: yes");
    println!("  requires_trusted_device: yes");
    println!("  supervisor: planned");
    println!("external:");
    println!("  attach_only: yes");
    println!("  supervisor: not_owned");
}

fn print_provider_logs_path() -> Result<(), String> {
    ensure_provider_runtime_dirs()?;
    println!("provider_log_dir: {}", provider_log_dir().display());
    println!("provider_run_dir: {}", provider_run_dir().display());
    println!("provider_plan_dir: {}", provider_plan_dir().display());
    Ok(())
}

fn print_model_catalog_status() {
    println!("model_catalog:");
    println!("  local_scan: planned");
    println!("  lan_scan: planned");
    println!("  manual_path_registration: planned");
    println!("  current_models: not_scanned");
}

fn print_model_runtime_status() {
    println!("model_runtime:");
    println!("  provider_supervision: planned");
    println!("  model_routing: planned");
    println!("  context_compiler: planned");
    println!("  retrieval_hnsw: planned");
    println!("  decoding_acceleration: planned");
    println!("  normal_decoding: baseline");
    println!("  speculative_draft: planned");
    println!("  native_mtp: opportunistic");
    println!("  fallback: normal_decoding");
}

fn provider_start_plan(args: &[String]) -> Result<(), String> {
    if !args.iter().any(|arg| arg == "--dry-run") {
        return Err("--dry-run is required in SPINE.33L".to_string());
    }
    let target = named_arg(args, "--target")?;
    validate_runtime_target(&target)?;
    let provider_kind = named_arg(args, "--kind")?;
    ensure_provider_runtime_dirs()?;

    println!("provider_start_plan:");
    println!("  target: {target}");
    match target.as_str() {
        "local" => {
            let model = named_arg(args, "--model")?;
            println!("  provider_kind: {provider_kind}");
            println!("  model: {model}");
            println!("  status: planned");
            println!("  action: dry_run_only");
            println!("  would_contact_device: false");
            println!("  would_start_provider: true");
            println!("  execution_performed: false");
            println!("  logs_path: {}", provider_log_dir().display());
            println!("  receipt_required: yes");
        }
        "lan" => {
            let device_id = named_arg(args, "--device")?;
            println!("  device_id: {device_id}");
            let devices = read_device_registry()?;
            let Some(device) = devices.iter().find(|device| device.device_id == device_id) else {
                println!("  status: blocked");
                println!("  reason: device_not_found");
                println!("  execution_performed: false");
                return Ok(());
            };
            if device.pairing_status != "paired" {
                println!("  status: blocked");
                println!("  reason: device_not_paired");
                println!("  execution_performed: false");
                return Ok(());
            }
            if device.trust_status != "trusted" {
                println!("  status: blocked");
                println!("  reason: device_not_trusted");
                println!("  execution_performed: false");
                return Ok(());
            }
            let model = named_arg(args, "--model")?;
            println!("  provider_kind: {provider_kind}");
            println!("  model: {model}");
            println!("  status: planned");
            println!("  action: dry_run_only");
            println!("  would_contact_device: true");
            println!("  would_start_provider: true");
            println!("  execution_performed: false");
            println!("  logs_path: {}", provider_log_dir().display());
            println!("  receipt_required: yes");
        }
        "external" => {
            let endpoint = named_arg(args, "--endpoint")?;
            println!("  provider_kind: {provider_kind}");
            println!("  endpoint: {endpoint}");
            println!("  status: planned");
            println!("  action: attach_only");
            println!("  would_contact_device: false");
            println!("  would_start_provider: false");
            println!("  execution_performed: false");
            println!("  logs_path: {}", provider_log_dir().display());
            println!("  receipt_required: yes");
        }
        _ => unreachable!("validated target"),
    }
    Ok(())
}

fn hot_state_path() -> PathBuf {
    yai_home().join("run").join("hot-state.json")
}

fn record_store_path() -> PathBuf {
    yai_home().join("store").join("lmdb")
}

fn replay_report_dir() -> PathBuf {
    yai_home().join("store").join("replay").join("reports")
}

fn replay_report_path(journal_identity: &str) -> PathBuf {
    replay_report_dir().join(format!("{journal_identity}.replay-report.json"))
}

struct RecordStoreStatus {
    path: PathBuf,
    backend: &'static str,
    status: &'static str,
}

fn record_store_status() -> RecordStoreStatus {
    let path = record_store_path();
    let status = LmdbRecordStore::status(&path);
    RecordStoreStatus {
        path,
        backend: status.backend,
        status: status.status.as_str(),
    }
}

fn print_store_status() {
    let status = record_store_status();
    println!("record_store_backend: {}", status.backend);
    println!("record_store_status: {}", status.status);
    println!("record_store_path: {}", status.path.display());
    println!("record_env: record_env");
    println!("schema: yai.record.v1");
    if status.status == "ready" {
        println!(
            "indexes: records_by_id,records_by_case,records_by_kind,records_by_subject,records_by_receipt"
        );
    }
}

fn print_store_summary() -> Result<(), String> {
    let status = LmdbRecordStore::status(record_store_path());
    println!("record_store_backend: {}", status.backend);
    println!("record_store_status: {}", status.status.as_str());
    println!("record_store_path: {}", status.path.display());
    if status.status != RecordStoreStatusKind::Ready {
        println!("records_total: 0");
        println!("records_by_case: 0");
        println!("records_by_kind: 0");
        println!("records_by_subject: 0");
        println!("records_by_receipt: 0");
        return Ok(());
    }
    let store = LmdbRecordStore::open(&status.path)?;
    let summary = store.summary()?;
    println!("records_total: {}", summary.records_total);
    println!("records_by_case: {}", summary.records_by_case);
    println!("records_by_kind: {}", summary.records_by_kind);
    println!("records_by_subject: {}", summary.records_by_subject);
    println!("records_by_receipt: {}", summary.records_by_receipt);
    Ok(())
}

fn print_non_ready_record_store(status: &yai_core_engine::store::lmdb::RecordStoreStatus) {
    println!("record_store_backend: {}", status.backend);
    println!("record_store_status: {}", status.status.as_str());
    println!("record_store_path: {}", status.path.display());
}

fn store_record_get(args: &[String]) -> Result<(), String> {
    let record_id = args
        .first()
        .filter(|value| !value.starts_with("--"))
        .ok_or_else(|| "record id is required".to_string())?;
    let status = LmdbRecordStore::status(record_store_path());
    if status.status != RecordStoreStatusKind::Ready {
        print_non_ready_record_store(&status);
        return Ok(());
    }
    let store = LmdbRecordStore::open(&status.path)?;
    let Some(record) = store.get_record_by_id(record_id)? else {
        println!("record: not_found");
        return Ok(());
    };
    println!("schema: {}", record.schema);
    println!("record_id: {}", record.record_id);
    println!("record_kind: {}", record.record_kind);
    println!("case_ref: {}", record.case_ref);
    println!("source:");
    println!(
        "  plane: {}",
        json_string_or(&record.raw_json, "plane", "unknown")
    );
    println!(
        "  ref: {}",
        json_string_or(&record.raw_json, "ref", "unknown")
    );
    println!("payload:");
    println!(
        "  summary: {}",
        json_string_or(&record.raw_json, "summary", "unknown")
    );
    println!("envelope: {}", record.raw_json);
    Ok(())
}

fn store_record_list(args: &[String]) -> Result<(), String> {
    let case_ref = optional_arg(args, "--case");
    let record_kind = optional_arg(args, "--kind");
    let subject_ref = optional_arg(args, "--subject");
    let receipt_ref = optional_arg(args, "--receipt");
    let filter_count = [
        case_ref.is_some(),
        record_kind.is_some(),
        subject_ref.is_some(),
        receipt_ref.is_some(),
    ]
    .into_iter()
    .filter(|present| *present)
    .count();
    if filter_count != 1 {
        return Err("provide exactly one of --case, --kind, --subject or --receipt".to_string());
    }
    let limit = parse_limit(args)?;
    let status = LmdbRecordStore::status(record_store_path());
    if status.status != RecordStoreStatusKind::Ready {
        print_non_ready_record_store(&status);
        return Ok(());
    }
    let store = LmdbRecordStore::open(&status.path)?;
    let result = if let Some(case_ref) = case_ref.as_deref() {
        let result = store.list_records_by_case(case_ref, limit)?;
        println!("filter: case");
        println!("filter_value: {case_ref}");
        result
    } else if let Some(record_kind) = record_kind.as_deref() {
        if RecordKind::from_str(record_kind).is_none() {
            return Err(format!("unknown record kind: {record_kind}"));
        }
        let result = store.list_records_by_kind(record_kind, limit)?;
        println!("filter: kind");
        println!("filter_value: {record_kind}");
        result
    } else if let Some(subject_ref) = subject_ref.as_deref() {
        let result = store.list_records_by_subject(subject_ref, limit)?;
        println!("filter: subject");
        println!("filter_value: {subject_ref}");
        result
    } else {
        let receipt_ref = receipt_ref.as_deref().unwrap_or_default();
        let result = store.list_records_by_receipt(receipt_ref, limit)?;
        println!("filter: receipt");
        println!("filter_value: {receipt_ref}");
        result
    };
    println!("records_total: {}", result.records_total);
    println!("limit: {limit}");
    if result.records.is_empty() {
        println!("records: none");
    } else {
        println!("records:");
        for record in result.records {
            println!("- record_id: {}", record.record_id);
            println!("  record_kind: {}", record.record_kind);
            println!("  case_ref: {}", record.case_ref);
        }
    }
    Ok(())
}

fn parse_limit(args: &[String]) -> Result<usize, String> {
    let limit = optional_arg(args, "--limit").unwrap_or_else(|| "20".to_string());
    let parsed = limit
        .parse::<usize>()
        .map_err(|_| format!("invalid --limit value: {limit}"))?;
    if parsed == 0 {
        return Err("--limit must be greater than zero".to_string());
    }
    Ok(parsed)
}

fn json_string_field(content: &str, key: &str) -> Option<String> {
    let marker = format!("\"{key}\":\"");
    let start = content.find(&marker)? + marker.len();
    let rest = &content[start..];
    let end = rest.find('"')?;
    Some(rest[..end].to_string())
}

fn json_number_field(content: &str, key: &str) -> Option<String> {
    let marker = format!("\"{key}\":");
    let start = content.find(&marker)? + marker.len();
    let rest = &content[start..];
    let end = rest
        .find(|ch: char| !(ch.is_ascii_digit()))
        .unwrap_or(rest.len());
    (end > 0).then(|| rest[..end].to_string())
}

fn json_string_or(content: &str, key: &str, fallback: &str) -> String {
    json_string_field(content, key)
        .filter(|value| !value.is_empty())
        .unwrap_or_else(|| fallback.to_string())
}

struct HotSnapshotStatus {
    status: &'static str,
    reason: &'static str,
    schema_status: &'static str,
    readable: &'static str,
    content: Option<String>,
}

fn validate_hot_snapshot(content: &str) -> bool {
    let trimmed = content.trim();
    if !trimmed.starts_with('{') || !trimmed.ends_with('}') {
        return false;
    }
    if json_string_field(content, "schema").as_deref() != Some("yai.hot_state.v1") {
        return false;
    }
    let required_strings = [
        "hot_state_id",
        "case_ref",
        "case_session_id",
        "case_context_id",
        "case_session_status",
        "case_world_status",
        "case_context_status",
        "projection_freshness",
        "projection_stale_reason",
    ];
    if required_strings
        .iter()
        .any(|key| json_string_field(content, key).is_none())
    {
        return false;
    }
    let required_numbers = ["case_version", "updated_at_unix_ms"];
    !required_numbers
        .iter()
        .any(|key| json_number_field(content, key).is_none())
}

fn hot_snapshot_status(path: &std::path::Path) -> HotSnapshotStatus {
    if !path.is_file() {
        return HotSnapshotStatus {
            status: "unavailable",
            reason: "missing_snapshot",
            schema_status: "missing",
            readable: "no",
            content: None,
        };
    }
    match fs::read_to_string(path) {
        Ok(content) if validate_hot_snapshot(&content) => HotSnapshotStatus {
            status: "active",
            reason: "none",
            schema_status: "valid",
            readable: "yes",
            content: Some(content),
        },
        Ok(content) => HotSnapshotStatus {
            status: "unavailable",
            reason: "invalid_snapshot",
            schema_status: "invalid",
            readable: "yes",
            content: Some(content),
        },
        Err(_) => HotSnapshotStatus {
            status: "unavailable",
            reason: "unreadable_snapshot",
            schema_status: "unknown",
            readable: "no",
            content: None,
        },
    }
}

#[derive(Clone, Debug)]
struct ProjectionFreshnessView {
    freshness: String,
    stale_reason: String,
    policy: String,
    consumer: String,
    source: String,
}

fn projection_policy_for(consumer: &str, freshness: &str, stale_reason: &str) -> &'static str {
    if freshness == "fresh" && stale_reason == "none" {
        return "usable";
    }
    if matches!(consumer, "operator" | "audit" | "debug") {
        return "refresh_recommended";
    }
    if !matches!(consumer, "model" | "agent") {
        return "unknown";
    }
    if freshness == "rebuilding" {
        return "refresh_required";
    }
    match stale_reason {
        "new_receipt_after_projection"
        | "new_decision_after_projection"
        | "new_memory_after_projection" => "refresh_required",
        "new_authority_scope_after_projection"
        | "new_divergence_after_projection"
        | "thread_changed"
        | "manual_refresh_required"
        | "unknown" => "blocked_for_model",
        _ => "blocked_for_model",
    }
}

fn projection_freshness_view(case_ref: &str, consumer: &str) -> ProjectionFreshnessView {
    let status = hot_snapshot_status(&hot_state_path());
    if status.status == "active" {
        if let Some(content) = status.content.as_deref() {
            let hot_case = json_string_or(content, "case_ref", "");
            if !case_ref.is_empty() && hot_case == case_ref {
                let freshness = json_string_or(content, "projection_freshness", "unknown");
                let stale_reason = json_string_or(content, "projection_stale_reason", "unknown");
                return ProjectionFreshnessView {
                    policy: projection_policy_for(consumer, &freshness, &stale_reason).to_string(),
                    freshness,
                    stale_reason,
                    consumer: consumer.to_string(),
                    source: "hot_state".to_string(),
                };
            }
        }
    }
    ProjectionFreshnessView {
        freshness: "fresh".to_string(),
        stale_reason: "none".to_string(),
        policy: projection_policy_for(consumer, "fresh", "none").to_string(),
        consumer: consumer.to_string(),
        source: "projection_record".to_string(),
    }
}

fn print_hot_status() -> Result<(), String> {
    let path = hot_state_path();
    let status = hot_snapshot_status(&path);
    if status.status != "active" {
        println!("hot_state: unavailable");
        println!("reason: {}", status.reason);
        println!("snapshot: {}", path.display());
        println!("snapshot_path: {}", path.display());
        println!("snapshot_status: {}", status.status);
        println!("schema: unknown");
        println!("case_session: unknown");
        println!("case_world: unknown");
        println!("case_context: unknown");
        println!("active_thread: unknown");
        println!("participant_view: unknown");
        println!("projection: unknown");
        println!("projection_policy: unknown");
        println!("freshness_policy: unknown");
        println!("stale_reason: unknown");
        return Ok(());
    }

    let content = status
        .content
        .as_deref()
        .ok_or_else(|| "valid hot snapshot was not loaded".to_string())?;
    println!("hot_state: active");
    println!("snapshot: {}", path.display());
    println!("snapshot_path: {}", path.display());
    println!("snapshot_status: active");
    println!("schema: {}", json_string_or(content, "schema", "unknown"));
    println!("case: {}", json_string_or(&content, "case_ref", "unknown"));
    let case_session_status = json_string_or(content, "case_session_status", "unknown");
    let case_world_status = json_string_or(content, "case_world_status", "unknown");
    let case_context_status = json_string_or(content, "case_context_status", "unknown");
    println!("session: {case_session_status}");
    println!("case_session: {case_session_status}");
    println!("case_world: {case_world_status}");
    println!("context: {case_context_status}");
    println!("case_context: {case_context_status}");
    println!(
        "case_session_id: {}",
        json_string_or(content, "case_session_id", "unknown")
    );
    println!(
        "case_context_id: {}",
        json_string_or(content, "case_context_id", "unknown")
    );
    println!(
        "active_thread: {}",
        json_string_or(content, "active_thread_id", "none")
    );
    println!(
        "participant_view: {}",
        json_string_or(content, "participant_view_frame_id", "none")
    );
    println!(
        "case_version: {}",
        json_number_field(content, "case_version").unwrap_or_else(|| "0".to_string())
    );
    println!(
        "projection: {}",
        json_string_or(content, "projection_freshness", "unknown")
    );
    let projection_freshness = json_string_or(content, "projection_freshness", "unknown");
    let stale_reason = json_string_or(content, "projection_stale_reason", "unknown");
    let freshness_policy = projection_policy_for("model", &projection_freshness, &stale_reason);
    println!("projection_policy: {}", freshness_policy);
    println!("freshness_policy: {freshness_policy}");
    println!("projection_freshness: {}", projection_freshness);
    println!("stale_reason: {stale_reason}");
    println!("projection_stale_reason: {stale_reason}");
    println!(
        "last_record: {}",
        json_string_or(content, "last_record_id", "none")
    );
    println!(
        "last_decision: {}",
        json_string_or(content, "last_decision_id", "none")
    );
    println!(
        "last_receipt: {}",
        json_string_or(content, "last_receipt_id", "none")
    );
    println!(
        "pending_ops: {}",
        json_number_field(content, "pending_op_count").unwrap_or_else(|| "0".to_string())
    );
    println!(
        "pending_obligations: {}",
        json_number_field(content, "pending_obligation_count").unwrap_or_else(|| "0".to_string())
    );
    println!(
        "carrier_locks: {}",
        json_number_field(content, "carrier_lock_count").unwrap_or_else(|| "0".to_string())
    );
    println!(
        "updated_at: {}",
        json_number_field(content, "updated_at_unix_ms").unwrap_or_else(|| "0".to_string())
    );
    Ok(())
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

fn path_state(path: &std::path::Path) -> &'static str {
    if path.is_dir() {
        "ok"
    } else if path.exists() {
        "not_directory"
    } else {
        "missing"
    }
}

fn path_state_with_path(path: &std::path::Path) -> String {
    format!("{} {}", path.display(), path_state(path))
}

fn yai_env_file() -> Option<PathBuf> {
    if let Some(path) = std::env::var_os("YAI_ENV_FILE")
        .map(PathBuf::from)
        .filter(|path| path.is_file())
    {
        return Some(path);
    }

    let mut dir = std::env::current_dir().ok()?;
    loop {
        let candidate = dir.join(".yai").join("env");
        if candidate.is_file() {
            return Some(candidate);
        }
        if !dir.pop() {
            break;
        }
    }

    let candidate = yai_home().join("env");
    candidate.is_file().then_some(candidate)
}

fn parse_env_assignment(line: &str) -> Option<(String, String)> {
    let line = line.trim();
    if line.is_empty() || line.starts_with('#') {
        return None;
    }
    let line = line.strip_prefix("export ").unwrap_or(line).trim();
    let (key, value) = line.split_once('=')?;
    let key = key.trim();
    if key.is_empty()
        || !key
            .chars()
            .all(|ch| ch == '_' || ch.is_ascii_alphanumeric())
        || key.chars().next().is_some_and(|ch| ch.is_ascii_digit())
    {
        return None;
    }

    let value = value.trim();
    let value = if value.len() >= 2 {
        let bytes = value.as_bytes();
        if (bytes[0] == b'"' && bytes[value.len() - 1] == b'"')
            || (bytes[0] == b'\'' && bytes[value.len() - 1] == b'\'')
        {
            &value[1..value.len() - 1]
        } else {
            value
        }
    } else {
        value
    };
    Some((key.to_string(), value.to_string()))
}

fn env_file_var(name: &str) -> Option<String> {
    let path = yai_env_file()?;
    let content = fs::read_to_string(path).ok()?;
    content
        .lines()
        .filter_map(parse_env_assignment)
        .find_map(|(key, value)| (key == name && !value.is_empty()).then_some(value))
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

fn latest_filesystem_journal() -> Option<PathBuf> {
    fn visit(dir: &std::path::Path, best: &mut Option<PathBuf>) {
        let Ok(entries) = fs::read_dir(dir) else {
            return;
        };
        for entry in entries.flatten() {
            let path = entry.path();
            if path.is_dir() {
                visit(&path, best);
            } else if path.file_name().and_then(|name| name.to_str()) == Some("journal.jsonl")
                && path
                    .components()
                    .any(|component| component.as_os_str() == "filesystem")
                && best.as_ref().map_or(true, |current| path > *current)
            {
                *best = Some(path);
            }
        }
    }

    let mut best = None;
    visit(std::path::Path::new("build/tmp"), &mut best);
    best
}

fn existing_env_path(name: &str) -> Option<PathBuf> {
    let path = env_var(name).map(PathBuf::from)?;
    if path.is_file() {
        Some(path)
    } else {
        None
    }
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

fn journal_inspect(args: &[String]) -> Result<(), String> {
    let path = PathBuf::from(named_arg(args, "--path")?);
    let show_errors = args.iter().any(|arg| arg == "--show-errors");
    println!("journal_path: {}", path.display());
    println!("parser_policy: diagnostic");
    println!("lmdb_write: no");
    if !path.exists() {
        println!("journal_status: missing");
        println!("lines_total: 0");
        println!("valid_entries: 0");
        println!("invalid_entries: 0");
        println!("unsupported_entries: 0");
        println!("duplicate_entries: 0");
        println!("replay_ready: no");
        return Ok(());
    }
    if !path.is_file() {
        println!("journal_status: unavailable");
        println!("lines_total: 0");
        println!("valid_entries: 0");
        println!("invalid_entries: 0");
        println!("unsupported_entries: 0");
        println!("duplicate_entries: 0");
        println!("replay_ready: no");
        return Ok(());
    }

    let inspection = Journal::inspect_jsonl(&path)
        .map_err(|error| format!("failed to inspect {}: {error}", path.display()))?;
    println!("journal_status: readable");
    println!("lines_total: {}", inspection.lines_total);
    println!("valid_entries: {}", inspection.valid_entries);
    println!("invalid_entries: {}", inspection.invalid_entries);
    println!("unsupported_entries: {}", inspection.unsupported_entries);
    println!("duplicate_entries: {}", inspection.duplicate_entries);
    println!("replay_ready: {}", bool_word(inspection.replay_ready()));
    if show_errors {
        for diagnostic in inspection.diagnostics {
            println!("line: {}", diagnostic.line_number);
            println!("entry_status: {}", diagnostic.entry_status.as_str());
            println!("record_id: {}", diagnostic.record_id);
            println!("record_kind: {}", diagnostic.record_kind);
            println!("schema: {}", diagnostic.schema);
            println!("error_code: {}", diagnostic.error_code);
            println!("error_message: {}", diagnostic.error_message);
            println!("action: {}", diagnostic.action);
        }
    }
    Ok(())
}

fn journal_replay(args: &[String]) -> Result<(), String> {
    let path = PathBuf::from(named_arg(args, "--path")?);
    let dry_run = args.iter().any(|arg| arg == "--dry-run");
    let lmdb_path = record_store_path();
    if !path.exists() {
        let profile = replay_profile_for_missing(&path);
        let started_at = unix_timestamp_string();
        let completed_at = unix_timestamp_string();
        let report_path = write_replay_report(&ReplayReport {
            report_id: replay_report_id(&profile.journal_identity),
            journal_identity: profile.journal_identity.clone(),
            journal_path: path.display().to_string(),
            lmdb_path: lmdb_path.display().to_string(),
            record_schema: profile.record_schema.clone(),
            journal_schema: profile.journal_schema.clone(),
            compatibility: profile.compatibility.clone(),
            started_at,
            completed_at,
            lines_total: 0,
            valid_entries: 0,
            invalid_entries: 0,
            unsupported_entries: 0,
            duplicate_entries: 0,
            records_seen: 0,
            records_written: 0,
            records_duplicate: 0,
            records_skipped: 0,
            cursor_line: 0,
            replay_status: "failed".to_string(),
            replay_ready: false,
            idempotent: false,
            errors: vec![ReplayReportIssue::new(
                0,
                "missing_journal",
                "journal path is missing",
            )],
            warnings: Vec::new(),
            summary: "Replay failed before materialization because the journal path is missing."
                .to_string(),
        })?;
        println!("journal_replay: failed");
        println!("journal_path: {}", path.display());
        println!("journal_identity: {}", profile.journal_identity);
        println!("lmdb_path: {}", lmdb_path.display());
        println!("record_schema: {}", profile.record_schema);
        println!("journal_schema: {}", profile.journal_schema);
        println!("compatibility: {}", profile.compatibility);
        println!("lines_total: 0");
        println!("valid_entries: 0");
        println!("invalid_entries: 0");
        println!("unsupported_entries: 0");
        println!("duplicate_entries: 0");
        println!("records_seen: 0");
        println!("records_written: 0");
        println!("records_duplicate: 0");
        println!("records_skipped: 0");
        println!("cursor_line: 0");
        println!("replay_status: failed");
        println!("replay_ready: no");
        println!("replay_report: {}", report_path.display());
        println!("reason: missing_journal");
        return Ok(());
    }
    if !path.is_file() {
        let profile = replay_profile_for_missing(&path);
        let started_at = unix_timestamp_string();
        let completed_at = unix_timestamp_string();
        let report_path = write_replay_report(&ReplayReport {
            report_id: replay_report_id(&profile.journal_identity),
            journal_identity: profile.journal_identity.clone(),
            journal_path: path.display().to_string(),
            lmdb_path: lmdb_path.display().to_string(),
            record_schema: profile.record_schema.clone(),
            journal_schema: profile.journal_schema.clone(),
            compatibility: profile.compatibility.clone(),
            started_at,
            completed_at,
            lines_total: 0,
            valid_entries: 0,
            invalid_entries: 0,
            unsupported_entries: 0,
            duplicate_entries: 0,
            records_seen: 0,
            records_written: 0,
            records_duplicate: 0,
            records_skipped: 0,
            cursor_line: 0,
            replay_status: "failed".to_string(),
            replay_ready: false,
            idempotent: false,
            errors: vec![ReplayReportIssue::new(
                0,
                "journal_unavailable",
                "journal path is not a regular file",
            )],
            warnings: Vec::new(),
            summary:
                "Replay failed before materialization because the journal path is unavailable."
                    .to_string(),
        })?;
        println!("journal_replay: failed");
        println!("journal_path: {}", path.display());
        println!("journal_identity: {}", profile.journal_identity);
        println!("lmdb_path: {}", lmdb_path.display());
        println!("record_schema: {}", profile.record_schema);
        println!("journal_schema: {}", profile.journal_schema);
        println!("compatibility: {}", profile.compatibility);
        println!("lines_total: 0");
        println!("valid_entries: 0");
        println!("invalid_entries: 0");
        println!("unsupported_entries: 0");
        println!("duplicate_entries: 0");
        println!("records_seen: 0");
        println!("records_written: 0");
        println!("records_duplicate: 0");
        println!("records_skipped: 0");
        println!("cursor_line: 0");
        println!("replay_status: failed");
        println!("replay_ready: no");
        println!("replay_report: {}", report_path.display());
        println!("reason: journal_unavailable");
        return Ok(());
    }

    let inspection = Journal::inspect_jsonl(&path)
        .map_err(|error| format!("failed to inspect {}: {error}", path.display()))?;
    let contents = fs::read_to_string(&path).map_err(|error| {
        format!(
            "failed to read {} for replay identity: {error}",
            path.display()
        )
    })?;
    let profile = replay_profile_for_inspection(&path, &contents, &inspection);
    if !inspection.replay_ready() {
        let metadata = replay_metadata_from_failure(&path, &profile, &inspection);
        persist_replay_metadata(&lmdb_path, metadata.clone())?;
        let report_path = write_replay_report(&replay_report_from_metadata(
            &metadata,
            &inspection,
            inspection.valid_entries,
            false,
            replay_report_issues_from_inspection(&inspection),
            Vec::new(),
            replay_summary(&metadata.status, 0, 0, inspection.invalid_entries),
        ))?;
        println!("journal_replay: failed");
        println!("journal_path: {}", path.display());
        println!("journal_identity: {}", profile.journal_identity);
        println!("lmdb_path: {}", lmdb_path.display());
        println!("record_schema: {}", profile.record_schema);
        println!("journal_schema: {}", profile.journal_schema);
        println!("supported_schema: {}", RECORD_SCHEMA);
        println!("compatibility: {}", profile.compatibility);
        println!("lines_total: {}", inspection.lines_total);
        println!("valid_entries: {}", inspection.valid_entries);
        println!("invalid_entries: {}", inspection.invalid_entries);
        println!("unsupported_entries: {}", inspection.unsupported_entries);
        println!("duplicate_entries: {}", inspection.duplicate_entries);
        println!("records_seen: {}", inspection.valid_entries);
        println!("records_written: 0");
        println!("records_duplicate: 0");
        println!("records_skipped: {}", inspection.lines_total);
        println!("cursor_line: 0");
        println!(
            "replay_status: {}",
            if inspection.invalid_entries == 0 && profile.compatibility == "schema_mismatch" {
                "incompatible"
            } else {
                "failed"
            }
        );
        println!("replay_ready: no");
        println!("replay_report: {}", report_path.display());
        println!("reason: {}", replay_failure_reason(&inspection));
        return Ok(());
    }

    if dry_run {
        let cursor_line = current_replay_metadata(&lmdb_path, &profile.journal_identity)?
            .map(|metadata| metadata.cursor_line)
            .unwrap_or(0);
        println!("journal_replay: dry_run");
        println!("journal_path: {}", path.display());
        println!("journal_identity: {}", profile.journal_identity);
        println!("lmdb_path: {}", lmdb_path.display());
        println!("record_schema: {}", profile.record_schema);
        println!("journal_schema: {}", profile.journal_schema);
        println!("compatibility: {}", profile.compatibility);
        println!("lines_total: {}", inspection.lines_total);
        println!("valid_entries: {}", inspection.valid_entries);
        println!("invalid_entries: {}", inspection.invalid_entries);
        println!("unsupported_entries: {}", inspection.unsupported_entries);
        println!("duplicate_entries: {}", inspection.duplicate_entries);
        println!("records_to_write: {}", inspection.valid_entries);
        println!("cursor_line: {cursor_line}");
        println!("would_update_cursor: yes");
        println!("would_write_lmdb: yes");
        println!("replay_ready: yes");
        return Ok(());
    }

    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("journal replay failed to load journal: {error}"))?;
    let store = LmdbRecordStore::open(&lmdb_path)
        .map_err(|error| format!("journal replay failed to open LMDB: {error}"))?;
    let started_at = unix_timestamp_string();
    store.put_replay_metadata(&replay_metadata_in_progress(
        &path,
        &profile,
        &inspection,
        &started_at,
    ))?;
    let report = store.import_journal_with_report(&journal, &path.display().to_string())?;
    let metadata = replay_metadata_from_report(
        &path,
        &profile,
        &inspection,
        &report,
        &started_at,
        &unix_timestamp_string(),
    );
    store.put_replay_metadata(&metadata)?;
    let idempotent = report.records_seen > 0
        && report.records_written == 0
        && report.records_duplicate == report.records_seen;
    let report_path = write_replay_report(&replay_report_from_metadata(
        &metadata,
        &inspection,
        report.records_seen,
        idempotent,
        Vec::new(),
        Vec::new(),
        replay_summary(
            &metadata.status,
            report.records_written,
            report.records_duplicate,
            inspection.invalid_entries,
        ),
    ))?;
    let status = LmdbRecordStore::status(&lmdb_path);
    println!("journal_replay: completed");
    println!("journal_path: {}", path.display());
    println!("journal_identity: {}", profile.journal_identity);
    println!("lmdb_path: {}", lmdb_path.display());
    println!("record_schema: {}", profile.record_schema);
    println!("journal_schema: {}", profile.journal_schema);
    println!("compatibility: {}", profile.compatibility);
    println!("lines_total: {}", inspection.lines_total);
    println!("lines_replayed: {}", metadata.lines_replayed);
    println!("valid_entries: {}", inspection.valid_entries);
    println!("invalid_entries: {}", inspection.invalid_entries);
    println!("unsupported_entries: {}", inspection.unsupported_entries);
    println!("duplicate_entries: {}", inspection.duplicate_entries);
    println!("records_seen: {}", report.records_seen);
    println!("records_written: {}", report.records_written);
    println!("records_duplicate: {}", report.records_duplicate);
    println!("records_skipped: {}", report.records_skipped);
    println!("cursor_line: {}", metadata.cursor_line);
    println!("replay_status: {}", metadata.status);
    println!("replay_ready: yes");
    println!("replay_report: {}", report_path.display());
    println!("record_store_status: {}", status.status.as_str());
    println!("idempotent: {}", bool_word(idempotent));
    Ok(())
}

fn journal_replay_status(args: &[String]) -> Result<(), String> {
    let path = PathBuf::from(named_arg(args, "--path")?);
    let lmdb_path = record_store_path();
    if !path.exists() || !path.is_file() {
        let profile = replay_profile_for_missing(&path);
        println!("journal_replay_status:");
        println!("journal_path: {}", path.display());
        println!("journal_identity: {}", profile.journal_identity);
        println!("lmdb_path: {}", lmdb_path.display());
        println!("record_schema: {}", profile.record_schema);
        println!("journal_schema: {}", profile.journal_schema);
        println!("cursor_line: 0");
        println!("replay_status: not_started");
        println!("records_written: 0");
        println!("records_duplicate: 0");
        println!("compatibility: {}", profile.compatibility);
        return Ok(());
    }

    let inspection = Journal::inspect_jsonl(&path)
        .map_err(|error| format!("failed to inspect {}: {error}", path.display()))?;
    let contents = fs::read_to_string(&path).map_err(|error| {
        format!(
            "failed to read {} for replay identity: {error}",
            path.display()
        )
    })?;
    let profile = replay_profile_for_inspection(&path, &contents, &inspection);
    let metadata = current_replay_metadata(&lmdb_path, &profile.journal_identity)?;
    println!("journal_replay_status:");
    println!("journal_path: {}", path.display());
    println!("journal_identity: {}", profile.journal_identity);
    println!("lmdb_path: {}", lmdb_path.display());
    match metadata {
        Some(metadata) => {
            println!("record_schema: {}", metadata.record_schema);
            println!("journal_schema: {}", metadata.journal_schema);
            println!("cursor_line: {}", metadata.cursor_line);
            println!("replay_status: {}", metadata.status);
            println!("lines_total: {}", metadata.lines_total);
            println!("lines_replayed: {}", metadata.lines_replayed);
            println!("records_written: {}", metadata.records_written);
            println!("records_duplicate: {}", metadata.records_duplicate);
            println!("records_skipped: {}", metadata.records_skipped);
            println!("invalid_entries: {}", metadata.invalid_entries);
            println!("unsupported_entries: {}", metadata.unsupported_entries);
            println!("compatibility: {}", metadata.compatibility);
        }
        None => {
            println!("record_schema: {}", profile.record_schema);
            println!("journal_schema: {}", profile.journal_schema);
            println!("cursor_line: 0");
            println!("replay_status: not_started");
            println!("lines_total: {}", inspection.lines_total);
            println!("lines_replayed: 0");
            println!("records_written: 0");
            println!("records_duplicate: 0");
            println!("records_skipped: 0");
            println!("invalid_entries: {}", inspection.invalid_entries);
            println!("unsupported_entries: {}", inspection.unsupported_entries);
            println!("compatibility: {}", profile.compatibility);
        }
    }
    Ok(())
}

fn journal_replay_report(args: &[String]) -> Result<(), String> {
    let path = PathBuf::from(named_arg(args, "--path")?);
    let (profile, inspection) = replay_profile_and_inspection_for_report(&path)?;
    let report_path = replay_report_path(&profile.journal_identity);
    if !report_path.is_file() {
        println!("replay_report_schema: yai.replay_report.v1");
        println!("replay_report: not_found");
        println!("journal_identity: {}", profile.journal_identity);
        println!("journal_path: {}", path.display());
        println!("lmdb_path: {}", record_store_path().display());
        println!("compatibility: {}", profile.compatibility);
        println!("replay_status: not_started");
        return Ok(());
    }

    let report = fs::read_to_string(&report_path)
        .map_err(|error| format!("failed to read {}: {error}", report_path.display()))?;
    println!(
        "replay_report_schema: {}",
        json_string_or(&report, "report_schema", "yai.replay_report.v1")
    );
    println!("replay_report: {}", report_path.display());
    println!(
        "report_id: {}",
        json_string_or(&report, "report_id", "unknown")
    );
    println!(
        "journal_identity: {}",
        json_string_or(&report, "journal_identity", &profile.journal_identity)
    );
    println!(
        "journal_path: {}",
        json_string_or(&report, "journal_path", &path.display().to_string())
    );
    println!(
        "lmdb_path: {}",
        json_string_or(
            &report,
            "lmdb_path",
            &record_store_path().display().to_string()
        )
    );
    println!(
        "record_schema: {}",
        json_string_or(&report, "record_schema", RECORD_SCHEMA)
    );
    println!(
        "journal_schema: {}",
        json_string_or(&report, "journal_schema", &profile.journal_schema)
    );
    println!(
        "compatibility: {}",
        json_string_or(&report, "compatibility", &profile.compatibility)
    );
    println!(
        "replay_status: {}",
        json_string_or(&report, "replay_status", "unknown")
    );
    print_report_number(&report, "lines_total", inspection.lines_total);
    print_report_number(&report, "valid_entries", inspection.valid_entries);
    print_report_number(&report, "invalid_entries", inspection.invalid_entries);
    print_report_number(
        &report,
        "unsupported_entries",
        inspection.unsupported_entries,
    );
    print_report_number(&report, "duplicate_entries", inspection.duplicate_entries);
    print_report_number(&report, "records_seen", 0);
    print_report_number(&report, "records_written", 0);
    print_report_number(&report, "records_duplicate", 0);
    print_report_number(&report, "records_skipped", 0);
    print_report_number(&report, "cursor_line", 0);
    println!(
        "replay_ready: {}",
        json_string_or(&report, "replay_ready", "no")
    );
    println!(
        "idempotent: {}",
        json_string_or(&report, "idempotent", "no")
    );
    println!(
        "summary: {}",
        json_string_or(&report, "summary", "Replay report summary unavailable.")
    );
    if !inspection.replay_ready() && !inspection.diagnostics.is_empty() {
        println!("errors:");
        for diagnostic in inspection.diagnostics {
            println!("- line: {}", diagnostic.line_number);
            println!("  status: {}", diagnostic.entry_status.as_str());
            println!("  error_code: {}", diagnostic.error_code);
        }
    }
    Ok(())
}

fn replay_failure_reason(inspection: &JournalInspection) -> String {
    if inspection.invalid_entries > 0 {
        return "invalid_json".to_string();
    }
    inspection
        .diagnostics
        .first()
        .map(|diagnostic| diagnostic.error_code.clone())
        .unwrap_or_else(|| "not_replay_ready".to_string())
}

#[derive(Clone, Debug)]
struct ReplayProfile {
    journal_identity: String,
    record_schema: String,
    journal_schema: String,
    compatibility: String,
}

fn replay_profile_for_missing(path: &std::path::Path) -> ReplayProfile {
    ReplayProfile {
        journal_identity: journal_identity(path, ""),
        record_schema: RECORD_SCHEMA.to_string(),
        journal_schema: "unknown".to_string(),
        compatibility: "unknown".to_string(),
    }
}

fn replay_profile_for_inspection(
    path: &std::path::Path,
    contents: &str,
    inspection: &JournalInspection,
) -> ReplayProfile {
    let mut observed_schema = if inspection.valid_entries > 0 {
        JOURNAL_RECORD_SCHEMA.to_string()
    } else {
        "unknown".to_string()
    };
    let mut compatibility = if inspection.valid_entries > 0 {
        "compatible".to_string()
    } else {
        "unknown".to_string()
    };

    for diagnostic in &inspection.diagnostics {
        if diagnostic.error_code == "invalid_schema" || diagnostic.error_code == "missing_record_id"
        {
            observed_schema = diagnostic.schema.clone();
            if observed_schema == "none" {
                compatibility = "unknown".to_string();
            } else {
                compatibility = "schema_mismatch".to_string();
            }
            break;
        }
    }

    let record_schema = if compatibility == "schema_mismatch" {
        observed_schema.clone()
    } else {
        RECORD_SCHEMA.to_string()
    };

    ReplayProfile {
        journal_identity: journal_identity(path, contents),
        record_schema,
        journal_schema: observed_schema,
        compatibility,
    }
}

fn current_replay_metadata(
    lmdb_path: &std::path::Path,
    journal_identity: &str,
) -> Result<Option<ReplayMetadata>, String> {
    if LmdbRecordStore::status(lmdb_path).status != RecordStoreStatusKind::Ready {
        return Ok(None);
    }
    let store = LmdbRecordStore::open(lmdb_path)
        .map_err(|error| format!("failed to open LMDB for replay status: {error}"))?;
    store.replay_metadata(journal_identity)
}

fn persist_replay_metadata(
    lmdb_path: &std::path::Path,
    metadata: ReplayMetadata,
) -> Result<(), String> {
    let store = LmdbRecordStore::open(lmdb_path)
        .map_err(|error| format!("failed to open LMDB for replay metadata: {error}"))?;
    store.put_replay_metadata(&metadata)
}

fn replay_metadata_in_progress(
    path: &std::path::Path,
    profile: &ReplayProfile,
    inspection: &JournalInspection,
    started_at: &str,
) -> ReplayMetadata {
    ReplayMetadata {
        replay_id: format!("replay:{}", profile.journal_identity),
        journal_identity: profile.journal_identity.clone(),
        journal_path: path.display().to_string(),
        record_schema: profile.record_schema.clone(),
        journal_schema: profile.journal_schema.clone(),
        started_at: started_at.to_string(),
        completed_at: "none".to_string(),
        lines_total: inspection.lines_total,
        lines_replayed: 0,
        records_written: 0,
        records_duplicate: 0,
        records_skipped: 0,
        invalid_entries: inspection.invalid_entries,
        unsupported_entries: inspection.unsupported_entries,
        cursor_line: 0,
        status: "in_progress".to_string(),
        compatibility: profile.compatibility.clone(),
    }
}

fn replay_metadata_from_report(
    path: &std::path::Path,
    profile: &ReplayProfile,
    inspection: &JournalInspection,
    report: &yai_core_engine::store::lmdb::JournalImportReport,
    started_at: &str,
    completed_at: &str,
) -> ReplayMetadata {
    ReplayMetadata {
        replay_id: format!("replay:{}", profile.journal_identity),
        journal_identity: profile.journal_identity.clone(),
        journal_path: path.display().to_string(),
        record_schema: profile.record_schema.clone(),
        journal_schema: profile.journal_schema.clone(),
        started_at: started_at.to_string(),
        completed_at: completed_at.to_string(),
        lines_total: inspection.lines_total,
        lines_replayed: inspection.valid_entries,
        records_written: report.records_written,
        records_duplicate: report.records_duplicate,
        records_skipped: report.records_skipped,
        invalid_entries: inspection.invalid_entries,
        unsupported_entries: inspection.unsupported_entries,
        cursor_line: inspection.lines_total,
        status: "completed".to_string(),
        compatibility: profile.compatibility.clone(),
    }
}

fn replay_metadata_from_failure(
    path: &std::path::Path,
    profile: &ReplayProfile,
    inspection: &JournalInspection,
) -> ReplayMetadata {
    ReplayMetadata {
        replay_id: format!("replay:{}", profile.journal_identity),
        journal_identity: profile.journal_identity.clone(),
        journal_path: path.display().to_string(),
        record_schema: profile.record_schema.clone(),
        journal_schema: profile.journal_schema.clone(),
        started_at: unix_timestamp_string(),
        completed_at: unix_timestamp_string(),
        lines_total: inspection.lines_total,
        lines_replayed: 0,
        records_written: 0,
        records_duplicate: 0,
        records_skipped: inspection.lines_total,
        invalid_entries: inspection.invalid_entries,
        unsupported_entries: inspection.unsupported_entries,
        cursor_line: 0,
        status: if inspection.invalid_entries == 0 && profile.compatibility == "schema_mismatch" {
            "incompatible".to_string()
        } else {
            "failed".to_string()
        },
        compatibility: profile.compatibility.clone(),
    }
}

#[derive(Clone, Debug)]
struct ReplayReportIssue {
    line: usize,
    status: String,
    message: String,
}

impl ReplayReportIssue {
    fn new(line: usize, status: &str, message: &str) -> Self {
        Self {
            line,
            status: status.to_string(),
            message: message.to_string(),
        }
    }
}

#[derive(Clone, Debug)]
struct ReplayReport {
    report_id: String,
    journal_identity: String,
    journal_path: String,
    lmdb_path: String,
    record_schema: String,
    journal_schema: String,
    compatibility: String,
    started_at: String,
    completed_at: String,
    lines_total: usize,
    valid_entries: usize,
    invalid_entries: usize,
    unsupported_entries: usize,
    duplicate_entries: usize,
    records_seen: usize,
    records_written: usize,
    records_duplicate: usize,
    records_skipped: usize,
    cursor_line: usize,
    replay_status: String,
    replay_ready: bool,
    idempotent: bool,
    errors: Vec<ReplayReportIssue>,
    warnings: Vec<ReplayReportIssue>,
    summary: String,
}

fn replay_profile_and_inspection_for_report(
    path: &Path,
) -> Result<(ReplayProfile, JournalInspection), String> {
    if !path.exists() || !path.is_file() {
        return Ok((
            replay_profile_for_missing(path),
            JournalInspection::default(),
        ));
    }
    let inspection = Journal::inspect_jsonl(path)
        .map_err(|error| format!("failed to inspect {}: {error}", path.display()))?;
    let contents = fs::read_to_string(path).map_err(|error| {
        format!(
            "failed to read {} for replay identity: {error}",
            path.display()
        )
    })?;
    Ok((
        replay_profile_for_inspection(path, &contents, &inspection),
        inspection,
    ))
}

fn replay_report_id(journal_identity: &str) -> String {
    format!("replay-report:{journal_identity}")
}

fn replay_report_from_metadata(
    metadata: &ReplayMetadata,
    inspection: &JournalInspection,
    records_seen: usize,
    idempotent: bool,
    errors: Vec<ReplayReportIssue>,
    warnings: Vec<ReplayReportIssue>,
    summary: String,
) -> ReplayReport {
    ReplayReport {
        report_id: replay_report_id(&metadata.journal_identity),
        journal_identity: metadata.journal_identity.clone(),
        journal_path: metadata.journal_path.clone(),
        lmdb_path: record_store_path().display().to_string(),
        record_schema: metadata.record_schema.clone(),
        journal_schema: metadata.journal_schema.clone(),
        compatibility: metadata.compatibility.clone(),
        started_at: metadata.started_at.clone(),
        completed_at: metadata.completed_at.clone(),
        lines_total: metadata.lines_total,
        valid_entries: inspection.valid_entries,
        invalid_entries: metadata.invalid_entries,
        unsupported_entries: metadata.unsupported_entries,
        duplicate_entries: inspection.duplicate_entries,
        records_seen,
        records_written: metadata.records_written,
        records_duplicate: metadata.records_duplicate,
        records_skipped: metadata.records_skipped,
        cursor_line: metadata.cursor_line,
        replay_status: metadata.status.clone(),
        replay_ready: inspection.replay_ready(),
        idempotent,
        errors,
        warnings,
        summary,
    }
}

fn replay_report_issues_from_inspection(inspection: &JournalInspection) -> Vec<ReplayReportIssue> {
    inspection
        .diagnostics
        .iter()
        .map(|diagnostic| ReplayReportIssue {
            line: diagnostic.line_number,
            status: diagnostic.entry_status.as_str().to_string(),
            message: diagnostic.error_code.clone(),
        })
        .collect()
}

fn replay_summary(
    replay_status: &str,
    records_written: usize,
    records_duplicate: usize,
    invalid_entries: usize,
) -> String {
    if replay_status == "completed" && records_duplicate > 0 && records_written == 0 {
        return format!(
            "Replay completed idempotently with {records_duplicate} duplicate records and no new writes."
        );
    }
    if replay_status == "completed" {
        return format!("Replay completed with {records_written} records written.");
    }
    format!("Replay failed with {invalid_entries} invalid entries and no durable writes.")
}

fn write_replay_report(report: &ReplayReport) -> Result<PathBuf, String> {
    let report_path = replay_report_path(&report.journal_identity);
    fs::create_dir_all(replay_report_dir()).map_err(|error| {
        format!(
            "failed to create replay report dir {}: {error}",
            replay_report_dir().display()
        )
    })?;
    fs::write(&report_path, replay_report_json(report))
        .map_err(|error| format!("failed to write {}: {error}", report_path.display()))?;
    Ok(report_path)
}

fn replay_report_json(report: &ReplayReport) -> String {
    format!(
        concat!(
            "{{\n",
            "  \"report_schema\":\"yai.replay_report.v1\",\n",
            "  \"report_id\":\"{}\",\n",
            "  \"journal_identity\":\"{}\",\n",
            "  \"journal_path\":\"{}\",\n",
            "  \"lmdb_path\":\"{}\",\n",
            "  \"record_schema\":\"{}\",\n",
            "  \"journal_schema\":\"{}\",\n",
            "  \"compatibility\":\"{}\",\n",
            "  \"started_at\":\"{}\",\n",
            "  \"completed_at\":\"{}\",\n            \"duration_ms\":{},\n",
            "  \"lines_total\":{},\n",
            "  \"valid_entries\":{},\n",
            "  \"invalid_entries\":{},\n",
            "  \"unsupported_entries\":{},\n",
            "  \"duplicate_entries\":{},\n",
            "  \"records_seen\":{},\n",
            "  \"records_written\":{},\n",
            "  \"records_duplicate\":{},\n",
            "  \"records_skipped\":{},\n",
            "  \"cursor_line\":{},\n",
            "  \"replay_status\":\"{}\",\n",
            "  \"replay_ready\":\"{}\",\n",
            "  \"idempotent\":\"{}\",\n",
            "  \"errors\":[{}],\n",
            "  \"warnings\":[{}],\n",
            "  \"summary\":\"{}\"\n",
            "}}\n"
        ),
        json_escape(&report.report_id),
        json_escape(&report.journal_identity),
        json_escape(&report.journal_path),
        json_escape(&report.lmdb_path),
        json_escape(&report.record_schema),
        json_escape(&report.journal_schema),
        json_escape(&report.compatibility),
        json_escape(&report.started_at),
        json_escape(&report.completed_at),
        replay_duration_ms(&report.started_at, &report.completed_at),
        report.lines_total,
        report.valid_entries,
        report.invalid_entries,
        report.unsupported_entries,
        report.duplicate_entries,
        report.records_seen,
        report.records_written,
        report.records_duplicate,
        report.records_skipped,
        report.cursor_line,
        json_escape(&report.replay_status),
        bool_word(report.replay_ready),
        bool_word(report.idempotent),
        replay_report_issues_json(&report.errors),
        replay_report_issues_json(&report.warnings),
        json_escape(&report.summary)
    )
}

fn replay_report_issues_json(issues: &[ReplayReportIssue]) -> String {
    issues
        .iter()
        .map(|issue| {
            format!(
                "{{\"line\":{},\"status\":\"{}\",\"message\":\"{}\"}}",
                issue.line,
                json_escape(&issue.status),
                json_escape(&issue.message)
            )
        })
        .collect::<Vec<_>>()
        .join(",")
}

fn replay_duration_ms(started_at: &str, completed_at: &str) -> u64 {
    let started = started_at.parse::<u64>().unwrap_or(0);
    let completed = completed_at.parse::<u64>().unwrap_or(started);
    completed.saturating_sub(started) * 1000
}

fn print_report_number(report: &str, key: &str, fallback: usize) {
    println!(
        "{key}: {}",
        json_number_field(report, key).unwrap_or_else(|| fallback.to_string())
    );
}

fn journal_identity(path: &std::path::Path, contents: &str) -> String {
    let seed = format!("{}|{}", path.display(), contents);
    format!("journal:{:016x}", fnv1a64(seed.as_bytes()))
}

fn fnv1a64(bytes: &[u8]) -> u64 {
    let mut hash = 0xcbf29ce484222325u64;
    for byte in bytes {
        hash ^= u64::from(*byte);
        hash = hash.wrapping_mul(0x100000001b3);
    }
    hash
}

fn unix_timestamp_string() -> String {
    std::time::SystemTime::now()
        .duration_since(std::time::UNIX_EPOCH)
        .map(|duration| duration.as_secs().to_string())
        .unwrap_or_else(|_| "0".to_string())
}

fn import_journal_to_record_store(journal_path: &std::path::Path) -> Result<(), String> {
    let journal = Journal::load_jsonl(journal_path)
        .map_err(|error| format!("record store import failed to load journal: {error}"))?;
    let store = LmdbRecordStore::open(record_store_path()).map_err(|error| {
        format!(
            "record store import failed after journal write remained at {}: {error}",
            journal_path.display()
        )
    })?;
    store
        .import_journal(&journal, &journal_path.display().to_string())
        .map_err(|error| {
            format!(
                "record store import failed after journal write remained at {}: {error}",
                journal_path.display()
            )
        })
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
    let consumer = optional_arg(args, "--consumer").unwrap_or_else(|| "model".to_string());
    let journal = Journal::load_jsonl(&path)
        .map_err(|error| format!("failed to load {}: {error}", path.display()))?;
    let projection = ProjectionSummary::from_journal("projection", &journal);
    let freshness = projection_freshness_view(&projection.case_ref, &consumer);
    println!("records: {}", projection.source_record_count);
    if !projection.case_ref.is_empty() {
        println!("case: {}", projection.case_ref);
    }
    println!("case_domains: {}", projection.case_domain_count);
    println!("case_attachments: {}", projection.case_attachment_count);
    println!("case_bindings: {}", projection.case_binding_count);
    println!(
        "interaction_threads: {}",
        projection.interaction_thread_count
    );
    println!("interaction_turns: {}", projection.interaction_turn_count);
    println!(
        "participant_view_frames: {}",
        projection.participant_view_frame_count
    );
    println!(
        "projection_requests: {}",
        projection.projection_request_count
    );
    println!("projection_results: {}", projection.projection_result_count);
    println!("projection_rules: {}", projection.projection_rule_count);
    println!("authority_scopes: {}", projection.authority_scope_count);
    println!(
        "model_interpretations: {}",
        projection.model_interpretation_count
    );
    println!("operator: {}", projection.operator_projection_count);
    println!("model: {}", projection.model_projection_count);
    println!("audit: {}", projection.audit_projection_count);
    println!(
        "redacted_or_limited: {}",
        projection.limited_projection_count
    );
    println!("consumer: {}", freshness.consumer);
    println!("projection_freshness: {}", freshness.freshness);
    println!("stale_reason: {}", freshness.stale_reason);
    println!("freshness_policy: {}", freshness.policy);
    println!("freshness_source: {}", freshness.source);
    println!("source: {}", freshness.source);
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

fn render_model_context_records(
    output: &mut String,
    title: &str,
    journal: &Journal,
    case_ref: Option<&str>,
    kinds: &[RecordKind],
) {
    let _ = writeln!(output, "## {title}");
    let mut count = 0usize;
    for record in journal
        .records()
        .iter()
        .filter(|record| record_in_case(record, case_ref) && kinds.contains(&record.kind))
    {
        let _ = writeln!(
            output,
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
        let _ = writeln!(output, "- none");
    }
    let _ = writeln!(output);
}

fn render_model_context_view(journal: &Journal, case_ref: Option<&str>) -> String {
    let projection = ProjectionSummary::from_journal("model", &journal);
    let case_ref = case_ref
        .or_else(|| (!projection.case_ref.is_empty()).then_some(projection.case_ref.as_str()));

    let mut output = String::new();
    let _ = writeln!(output, "case_ref: {}", case_ref.unwrap_or("unknown"));
    let _ = writeln!(output, "case_world: materialized");
    let _ = writeln!(output, "case_context: active");
    let _ = writeln!(output, "consumer: model");
    let _ = writeln!(output, "projection_kind: model_context");
    let _ = writeln!(output, "redaction: summary_only");
    let _ = writeln!(output, "source: case_projection_graph_memory");
    let _ = writeln!(output, "raw_journal_access: not_provided");
    let _ = writeln!(output, "filesystem_access: not_provided");
    let _ = writeln!(output, "decision_authority: not_provided");
    let _ = writeln!(output, "receipt_authority: not_provided");
    let _ = writeln!(
        output,
        "terminal_authority: prompt_surface_only_no_decision_authority"
    );
    let _ = writeln!(output, "records: {}", projection.source_record_count);
    let _ = writeln!(output, "case_domains: {}", projection.case_domain_count);
    let _ = writeln!(
        output,
        "case_attachments: {}",
        projection.case_attachment_count
    );
    let _ = writeln!(output, "case_bindings: {}", projection.case_binding_count);
    let _ = writeln!(
        output,
        "interaction_threads: {}",
        projection.interaction_thread_count
    );
    let _ = writeln!(
        output,
        "interaction_turns: {}",
        projection.interaction_turn_count
    );
    let _ = writeln!(
        output,
        "participant_view_frames: {}",
        projection.participant_view_frame_count
    );
    let _ = writeln!(
        output,
        "model_projection_records: {}",
        projection.model_projection_count
    );
    let _ = writeln!(
        output,
        "operator_projection_records: {}",
        projection.operator_projection_count
    );
    let _ = writeln!(
        output,
        "filesystem_receipts: {}",
        projection.filesystem_receipt_count
    );
    let _ = writeln!(
        output,
        "memory_candidates: {}",
        projection.memory_candidate_count
    );
    let _ = writeln!(
        output,
        "projection_rules: {}",
        projection.projection_rule_count
    );
    let _ = writeln!(
        output,
        "authority_scopes: {}",
        projection.authority_scope_count
    );
    let _ = writeln!(
        output,
        "model_interpretations: {}",
        projection.model_interpretation_count
    );
    let _ = writeln!(output, "graph_edges: {}", projection.graph_edge_count);
    let _ = writeln!(output);

    render_model_context_records(
        &mut output,
        "Case World",
        &journal,
        case_ref,
        &[
            RecordKind::CaseDomain,
            RecordKind::CaseAttachment,
            RecordKind::CaseBinding,
        ],
    );
    render_model_context_records(
        &mut output,
        "Subjects",
        &journal,
        case_ref,
        &[RecordKind::SubjectBinding],
    );
    render_model_context_records(
        &mut output,
        "Policy",
        &journal,
        case_ref,
        &[RecordKind::PolicyRule],
    );
    render_model_context_records(
        &mut output,
        "Projection Rules",
        &journal,
        case_ref,
        &[RecordKind::ProjectionRule],
    );
    render_model_context_records(
        &mut output,
        "Authority Scopes",
        &journal,
        case_ref,
        &[RecordKind::AuthorityScope],
    );
    render_model_context_records(
        &mut output,
        "Decisions",
        &journal,
        case_ref,
        &[RecordKind::Decision],
    );
    render_model_context_records(
        &mut output,
        "Filesystem Receipts",
        &journal,
        case_ref,
        &[RecordKind::FilesystemReceipt],
    );
    render_model_context_records(
        &mut output,
        "Memory",
        &journal,
        case_ref,
        &[RecordKind::MemoryCandidate],
    );
    render_model_context_records(
        &mut output,
        "Graph",
        &journal,
        case_ref,
        &[RecordKind::GraphEdge],
    );
    render_model_context_records(
        &mut output,
        "Projection Records",
        &journal,
        case_ref,
        &[RecordKind::ProjectionRequest, RecordKind::ProjectionResult],
    );
    render_model_context_records(
        &mut output,
        "Model Interpretations",
        &journal,
        case_ref,
        &[RecordKind::ModelInterpretation],
    );
    let _ = writeln!(output, "## Authority Boundaries");
    let _ = writeln!(
        output,
        "- case_domain, case_attachment and case_binding records define the operational case world visible to this participant."
    );
    let _ = writeln!(
        output,
        "- subject:linenoise-terminal is a vendored prompt surface only; it does not generate decisions, authorize writes, mutate receipts or own provider semantics."
    );
    let _ = writeln!(
        output,
        "- subject:llm-provider may produce claims, proposals and model_interpretation records; those are not authoritative state until checked against decisions, receipts, graph and memory."
    );
    let _ = writeln!(
        output,
        "- filesystem decisions are represented by decision records; existing decisions/receipts are historical residue, not mutable by model output."
    );
    let _ = writeln!(
        output,
        "- When answering, state authority granted by the current projection, not physical capability of the host process."
    );
    let _ = writeln!(
        output,
        "- raw_journal_access, filesystem_access, decision_authority and receipt_authority are not provided to the model participant view."
    );
    let _ = writeln!(output);
    output
}

fn print_model_context_view(journal: &Journal, case_ref: Option<&str>) {
    print!("{}", render_model_context_view(journal, case_ref));
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
    println!("printf '%s\\n' {}", shell_quote("case_session: active"));
    println!("printf '%s\\n' {}", shell_quote("case_world: materialized"));
    println!("printf '%s\\n' {}", shell_quote("case_context: active"));
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
    println!("case_session: active");
    println!("case_world: materialized");
    println!("case_context: active");
    println!("case_ref: {case_ref}");
    println!("subject_ref: {subject_ref}");
    println!("participant_view: {kind}");
    println!("consumer: {consumer}");
    println!("redaction: summary_only");
    println!("raw_journal_access: not_provided");
    println!("filesystem_access: not_provided");
    println!("authority_scope: model interpretation_only");
    println!();
    print_model_context_view(&journal, Some(&case_ref));
    Ok(())
}

fn append_record_to_journal(path: &PathBuf, record: &Record) -> Result<(), String> {
    let mut file = OpenOptions::new()
        .append(true)
        .open(path)
        .map_err(|error| format!("failed to append {}: {error}", path.display()))?;
    file.write_all(record.to_jsonl().as_bytes())
        .map_err(|error| format!("failed to write {}: {error}", path.display()))?;
    Ok(())
}

fn compact_text(value: &str, max_chars: usize) -> String {
    let compact = value.split_whitespace().collect::<Vec<_>>().join(" ");
    let mut result = String::new();
    for ch in compact.chars().take(max_chars) {
        result.push(ch);
    }
    if compact.chars().count() > max_chars {
        result.push_str("...");
    }
    result
}

fn print_provider_attach_shell(
    case_ref: &str,
    subject_ref: &str,
    base_url: &str,
    model: &str,
    api_key_env: &str,
    status: &str,
) {
    println!(
        "printf '%s\\n' {}",
        shell_quote("provider_attachment: accepted")
    );
    println!(
        "printf '%s\\n' {}",
        shell_quote(&format!("provider_attachment_status: {status}"))
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
        shell_quote(&format!("provider_base_url: {base_url}"))
    );
    println!(
        "printf '%s\\n' {}",
        shell_quote(&format!("provider_model: {model}"))
    );
    println!("printf '%s\\n' {}", shell_quote("case_session: active"));
    println!("printf '%s\\n' {}", shell_quote("case_context: active"));
    println!(
        "printf '%s\\n' {}",
        shell_quote("authority_scope: model interpretation_only")
    );
    println!("export YAI_PROVIDER_BASE_URL={}", shell_quote(base_url));
    println!("export YAI_PROVIDER_MODEL={}", shell_quote(model));
    println!(
        "export YAI_PROVIDER_SUBJECT_REF={}",
        shell_quote(subject_ref)
    );
    println!(
        "export YAI_PROVIDER_API_KEY_ENV={}",
        shell_quote(api_key_env)
    );
}

fn case_attach_provider(args: &[String]) -> Result<(), String> {
    let path = journal_arg(args)?;
    let case_ref = named_arg(args, "--case")?;
    let subject_ref = named_arg(args, "--subject")?;
    let base_url = named_arg(args, "--base-url")?;
    let model = named_arg(args, "--model")?;
    let api_key_env =
        optional_arg(args, "--api-key-env").unwrap_or_else(|| "OPENCODE_LLM_API_KEY".to_string());
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

    let provider_summary = format!(
        "provider_attachment:attached provider:openai_compatible base_url:{base_url} model:{model} api_key_env:{api_key_env} prompt_surface:vendored_linenoise context:case_projection_graph_memory"
    );
    let already_attached = journal.records().iter().any(|record| {
        record.case_ref == case_ref
            && record.kind == RecordKind::SubjectState
            && record.subject_ref == subject_ref
            && record.summary == provider_summary
    });

    if !already_attached {
        let record = Record::from_parts(
            format!(
                "provider-attachment:{}:{}",
                subject_ref.replace(':', "-"),
                journal.count() + 1
            ),
            &case_ref,
            RecordKind::SubjectState,
            &subject_ref,
            "",
            "",
            "",
            provider_summary,
        );
        append_record_to_journal(&path, &record)?;
    }

    let status = if already_attached {
        "already_attached"
    } else {
        "attached"
    };
    if let Some(shell) = shell.as_deref() {
        if shell != "zsh" && shell != "sh" {
            return Err(format!("unsupported shell: {shell}"));
        }
        print_provider_attach_shell(
            &case_ref,
            &subject_ref,
            &base_url,
            &model,
            &api_key_env,
            status,
        );
        return Ok(());
    }

    println!("provider_attachment: accepted");
    println!("provider_attachment_status: {status}");
    println!("case_ref: {case_ref}");
    println!("subject_ref: {subject_ref}");
    println!("case_session: active");
    println!("case_context: active");
    println!("authority_scope: model interpretation_only");
    println!("provider_base_url: {base_url}");
    println!("provider_model: {model}");
    println!("api_key_env: {api_key_env}");
    Ok(())
}

struct ProviderConfig {
    base_url: String,
    model: String,
    api_key: Option<String>,
    language_mode: String,
}

struct PromptSession {
    journal_path: PathBuf,
    case_ref: String,
    subject_ref: String,
    provider: ProviderConfig,
    active_thread_id: String,
    participant_view: String,
    transcript_enabled: bool,
}

const DEFAULT_THREAD_ID: &str = "thread:default";

fn env_var(name: &str) -> Option<String> {
    std::env::var(name)
        .ok()
        .filter(|value| !value.is_empty())
        .or_else(|| env_file_var(name))
}

fn color_enabled() -> bool {
    std::io::stdout().is_terminal()
        && env_var("NO_COLOR").is_none()
        && env_var("YAI_NO_COLOR").is_none()
        && env_var("TERM").as_deref() != Some("dumb")
}

fn paint(enabled: bool, code: &str, value: &str) -> String {
    if enabled {
        format!("{code}{value}{ANSI_RESET}")
    } else {
        value.to_string()
    }
}

fn transcript_retention_enabled(journal: &Journal, case_ref: &str, subject_ref: &str) -> bool {
    let mut enabled = false;
    for record in journal.records().iter().filter(|record| {
        record.case_ref == case_ref
            && record.kind == RecordKind::SubjectState
            && record.subject_ref == subject_ref
            && record.summary.contains("prompt_transcript_retention:")
    }) {
        if record
            .summary
            .contains("prompt_transcript_retention:enabled")
        {
            enabled = true;
        } else if record
            .summary
            .contains("prompt_transcript_retention:disabled")
        {
            enabled = false;
        }
    }
    enabled
}

fn transcript_retention_label(enabled: bool) -> &'static str {
    if enabled {
        "full_redacted_case_local"
    } else {
        "preview_only"
    }
}

fn summary_token(summary: &str, key: &str) -> Option<String> {
    let prefix = format!("{key}:");
    summary.split_whitespace().find_map(|part| {
        part.strip_prefix(&prefix)
            .map(|value| value.trim_matches(',').to_string())
            .filter(|value| !value.is_empty())
    })
}

fn active_thread_id(journal: &Journal, case_ref: &str) -> Option<String> {
    let mut active = None;
    for record in journal.records().iter().filter(|record| {
        record.case_ref == case_ref && record.kind == RecordKind::InteractionThread
    }) {
        if record.summary.contains("state:active") {
            if let Some(thread_id) = summary_token(&record.summary, "thread_id") {
                active = Some(thread_id);
            }
        }
    }
    active
}

fn thread_turn_count(journal: &Journal, case_ref: &str, thread_id: &str) -> usize {
    let needle = format!("thread_id:{thread_id}");
    journal
        .records()
        .iter()
        .filter(|record| {
            record.case_ref == case_ref
                && record.kind == RecordKind::InteractionTurn
                && record.summary.contains(&needle)
        })
        .count()
}

fn latest_frame_id(journal: &Journal, case_ref: &str, thread_id: &str) -> Option<String> {
    let needle = format!("thread_id:{thread_id}");
    let mut frame_id = None;
    for record in journal.records().iter().filter(|record| {
        record.case_ref == case_ref
            && record.kind == RecordKind::ParticipantViewFrame
            && record.summary.contains(&needle)
    }) {
        frame_id = summary_token(&record.summary, "frame_id").or_else(|| Some(record.id.clone()));
    }
    frame_id
}

fn append_thread_record(
    journal_path: &PathBuf,
    case_ref: &str,
    subject_ref: &str,
    thread_id: &str,
    label: &str,
    state: &str,
) -> Result<(), String> {
    let journal = Journal::load_jsonl(journal_path)
        .map_err(|error| format!("failed to load {}: {error}", journal_path.display()))?;
    let sequence = journal.count() + 1;
    let summary = format!(
        "interaction_thread:{thread_id} thread_id:{thread_id} state:{state} label:{} journal_role:replay_audit active_context:thread_plus_projection",
        compact_text(label, 48)
    );
    let record = Record::from_parts(
        format!(
            "interaction-thread:{}:{sequence}",
            thread_id.replace(':', "-")
        ),
        case_ref,
        RecordKind::InteractionThread,
        subject_ref,
        "",
        "",
        "",
        summary,
    );
    append_record_to_journal(journal_path, &record)
}

fn ensure_default_thread(
    journal_path: &PathBuf,
    journal: &Journal,
    case_ref: &str,
    subject_ref: &str,
) -> Result<String, String> {
    if let Some(thread_id) = active_thread_id(journal, case_ref) {
        return Ok(thread_id);
    }
    append_thread_record(
        journal_path,
        case_ref,
        subject_ref,
        DEFAULT_THREAD_ID,
        "default",
        "active",
    )?;
    Ok(DEFAULT_THREAD_ID.to_string())
}

fn render_thread_context(journal: &Journal, case_ref: &str, thread_id: &str) -> String {
    let mut output = String::new();
    let needle = format!("thread_id:{thread_id}");
    let mut count = 0usize;
    let _ = writeln!(output, "## Active Interaction Thread");
    let _ = writeln!(output, "interaction_thread: {thread_id}");
    let _ = writeln!(output, "journal_role: replay_audit_not_chat_memory");
    for record in journal.records().iter().filter(|record| {
        record.case_ref == case_ref
            && record.kind == RecordKind::InteractionTurn
            && record.summary.contains(&needle)
    }) {
        count += 1;
        let _ = writeln!(
            output,
            "- kind:interaction_turn record_id:{} summary:{}",
            record.id, record.summary
        );
    }
    let _ = writeln!(output, "included_turn_count: {count}");
    if count == 0 {
        let _ = writeln!(output, "thread_state: empty");
    }
    output
}

fn render_participant_view(journal: &Journal, case_ref: &str, thread_id: &str) -> String {
    let mut view = render_model_context_view(journal, Some(case_ref));
    view.push('\n');
    view.push_str(&render_thread_context(journal, case_ref, thread_id));
    view
}

fn prompt_session_from_args(args: &[String]) -> Result<PromptSession, String> {
    let journal_path = optional_arg(args, "--journal")
        .map(PathBuf::from)
        .filter(|path| path.is_file())
        .or_else(|| existing_env_path("YAI_JOURNAL"))
        .or_else(latest_filesystem_journal)
        .ok_or_else(|| "YAI_JOURNAL is required; run `yai case enter` first".to_string())?;
    let case_ref = optional_arg(args, "--case")
        .or_else(|| env_var("YAI_CASE_REF"))
        .ok_or_else(|| "YAI_CASE_REF is required; run `yai case enter` first".to_string())?;
    let subject_ref = optional_arg(args, "--subject")
        .or_else(|| env_var("YAI_PROVIDER_SUBJECT_REF"))
        .or_else(|| env_var("YAI_SUBJECT_REF"))
        .unwrap_or_else(|| "subject:llm-provider".to_string());
    let base_url = optional_arg(args, "--base-url")
        .or_else(|| env_var("YAI_PROVIDER_BASE_URL"))
        .or_else(|| env_var("YAI_LLM_BASE_URL"))
        .ok_or_else(|| {
            "provider base URL missing; run `yai case attach-provider` or export YAI_PROVIDER_BASE_URL"
                .to_string()
        })?;
    let model = optional_arg(args, "--model")
        .or_else(|| env_var("YAI_PROVIDER_MODEL"))
        .or_else(|| env_var("YAI_LLM_MODEL"))
        .ok_or_else(|| {
            "provider model missing; run `yai case attach-provider` or export YAI_PROVIDER_MODEL"
                .to_string()
        })?;
    let api_key_env = optional_arg(args, "--api-key-env")
        .or_else(|| env_var("YAI_PROVIDER_API_KEY_ENV"))
        .unwrap_or_else(|| "OPENCODE_LLM_API_KEY".to_string());
    let api_key = env_var("YAI_PROVIDER_API_KEY")
        .or_else(|| env_var(&api_key_env))
        .or_else(|| env_var("OPENCODE_LLM_API_KEY"));
    let language_mode = optional_arg(args, "--language-mode")
        .or_else(|| env_var("YAI_PROVIDER_LANGUAGE_MODE"))
        .unwrap_or_else(|| "none".to_string());
    if language_mode != "none" && language_mode != "auto" {
        return Err("--language-mode must be auto or none".to_string());
    }
    let journal = Journal::load_jsonl(&journal_path)
        .map_err(|error| format!("failed to load {}: {error}", journal_path.display()))?;
    let admitted = journal.records().iter().any(|record| {
        record.case_ref == case_ref
            && record.kind == RecordKind::SubjectState
            && record.subject_ref == subject_ref
            && record.summary.contains("case_entry:admitted")
    });
    if !admitted {
        return Err(format!(
            "{subject_ref} has not entered {case_ref}; run `yai case enter` first"
        ));
    }
    let attached = journal.records().iter().any(|record| {
        record.case_ref == case_ref
            && record.kind == RecordKind::SubjectState
            && record.subject_ref == subject_ref
            && record.summary.contains("provider_attachment:attached")
    });
    if !attached {
        return Err(format!(
            "{subject_ref} has no provider attachment in {case_ref}; run `yai case attach-provider` first"
        ));
    }
    let transcript_enabled = transcript_retention_enabled(&journal, &case_ref, &subject_ref);
    let active_thread_id = ensure_default_thread(&journal_path, &journal, &case_ref, &subject_ref)?;
    let journal = Journal::load_jsonl(&journal_path)
        .map_err(|error| format!("failed to load {}: {error}", journal_path.display()))?;

    Ok(PromptSession {
        journal_path,
        case_ref: case_ref.clone(),
        subject_ref,
        provider: ProviderConfig {
            base_url,
            model,
            api_key,
            language_mode,
        },
        active_thread_id: active_thread_id.clone(),
        participant_view: render_participant_view(&journal, &case_ref, &active_thread_id),
        transcript_enabled,
    })
}

fn linenoise_read_line(prompt: &str) -> Result<Option<String>, String> {
    let prompt = CString::new(prompt).map_err(|_| "prompt contains a NUL byte".to_string())?;
    let ptr = unsafe { linenoise(prompt.as_ptr()) };
    if ptr.is_null() {
        return Ok(None);
    }
    let line = unsafe { CStr::from_ptr(ptr) }
        .to_string_lossy()
        .into_owned();
    unsafe {
        linenoiseFree(ptr.cast::<c_void>());
    }
    Ok(Some(line))
}

fn prompt_label(case_ref: &str, colors: bool) -> String {
    if colors {
        format!(
            "{}{}{}({}{}{})> ",
            ANSI_BOLD, ANSI_CYAN, "yai", ANSI_YELLOW, case_ref, ANSI_RESET
        )
    } else {
        format!("yai({case_ref})> ")
    }
}

fn terminal_width() -> usize {
    env_var("COLUMNS")
        .and_then(|value| value.parse::<usize>().ok())
        .filter(|width| *width >= 50)
        .map(|width| width.min(120))
        .unwrap_or(92)
}

fn print_cli_section(colors: bool, label: &str, detail: &str, color: &str) {
    let width = terminal_width();
    let title = if detail.is_empty() {
        label.to_string()
    } else {
        format!("{label} {detail}")
    };
    let plain = format!("-- {title} ");
    let line = if plain.len() >= width {
        plain
    } else {
        format!("{}{}", plain, "-".repeat(width - plain.len()))
    };
    println!("{}", paint(colors, color, &line));
}

fn wrap_words(text: &str, indent: &str, width: usize) -> Vec<String> {
    let available = width.saturating_sub(indent.len()).max(24);
    let mut lines = Vec::new();
    let mut current = String::new();
    for word in text.split_whitespace() {
        if current.is_empty() {
            current.push_str(word);
        } else if current.len() + 1 + word.len() > available {
            lines.push(format!("{indent}{current}"));
            current.clear();
            current.push_str(word);
        } else {
            current.push(' ');
            current.push_str(word);
        }
    }
    if !current.is_empty() {
        lines.push(format!("{indent}{current}"));
    }
    if lines.is_empty() {
        lines.push(String::new());
    }
    lines
}

fn ordered_list_marker(line: &str) -> Option<usize> {
    let (prefix, rest) = line.split_once(". ")?;
    if prefix.is_empty() || prefix.chars().any(|ch| !ch.is_ascii_digit()) || rest.is_empty() {
        return None;
    }
    Some(prefix.len() + 2)
}

fn print_wrapped_text(colors: bool, line: &str, width: usize) {
    let trimmed = line.trim();
    if trimmed.is_empty() {
        println!();
        return;
    }

    if trimmed.starts_with("```") {
        println!("{}", paint(colors, ANSI_DIM, trimmed));
        return;
    }

    if let Some(title) = trimmed.strip_prefix("### ") {
        println!();
        println!("{}", paint(colors, ANSI_BOLD, title));
        return;
    }
    if let Some(title) = trimmed.strip_prefix("## ") {
        println!();
        println!("{}", paint(colors, ANSI_BOLD, title));
        return;
    }
    if let Some(title) = trimmed.strip_prefix("# ") {
        println!();
        println!("{}", paint(colors, ANSI_BOLD, title));
        return;
    }

    if let Some(rest) = trimmed
        .strip_prefix("* ")
        .or_else(|| trimmed.strip_prefix("- "))
    {
        let bullet = paint(colors, ANSI_GREEN, "-");
        let first_indent = format!("  {bullet} ");
        let next_indent = "    ";
        let wrapped = wrap_words(rest, "", width.saturating_sub(4));
        for (index, item) in wrapped.iter().enumerate() {
            if index == 0 {
                println!("{first_indent}{item}");
            } else {
                println!("{next_indent}{item}");
            }
        }
        return;
    }

    if let Some(marker_len) = ordered_list_marker(trimmed) {
        let marker = &trimmed[..marker_len];
        let rest = &trimmed[marker_len..];
        let marker = paint(colors, ANSI_GREEN, marker.trim_end());
        let first_indent = format!("  {marker} ");
        let next_indent = "    ";
        let wrapped = wrap_words(rest, "", width.saturating_sub(4));
        for (index, item) in wrapped.iter().enumerate() {
            if index == 0 {
                println!("{first_indent}{item}");
            } else {
                println!("{next_indent}{item}");
            }
        }
        return;
    }

    for item in wrap_words(trimmed, "  ", width) {
        println!("{item}");
    }
}

fn print_model_output(colors: bool, output: &str) {
    let width = terminal_width();
    for line in output.lines() {
        print_wrapped_text(colors, line, width);
    }
}

fn json_escape(value: &str) -> String {
    let mut escaped = String::new();
    for ch in value.chars() {
        match ch {
            '"' => escaped.push_str("\\\""),
            '\\' => escaped.push_str("\\\\"),
            '\n' => escaped.push_str("\\n"),
            '\r' => escaped.push_str("\\r"),
            '\t' => escaped.push_str("\\t"),
            ch if ch.is_control() => {
                let _ = write!(escaped, "\\u{:04x}", ch as u32);
            }
            ch => escaped.push(ch),
        }
    }
    escaped
}

fn parse_json_string_at(bytes: &[u8], mut index: usize) -> Option<(String, usize)> {
    if bytes.get(index).copied()? != b'"' {
        return None;
    }
    index += 1;
    let mut output = String::new();
    while index < bytes.len() {
        let byte = bytes[index];
        index += 1;
        match byte {
            b'"' => return Some((output, index)),
            b'\\' => {
                let escaped = *bytes.get(index)?;
                index += 1;
                match escaped {
                    b'"' => output.push('"'),
                    b'\\' => output.push('\\'),
                    b'/' => output.push('/'),
                    b'b' => output.push('\u{0008}'),
                    b'f' => output.push('\u{000c}'),
                    b'n' => output.push('\n'),
                    b'r' => output.push('\r'),
                    b't' => output.push('\t'),
                    b'u' => {
                        let hex = std::str::from_utf8(bytes.get(index..index + 4)?).ok()?;
                        let value = u16::from_str_radix(hex, 16).ok()?;
                        if let Some(ch) = char::from_u32(value as u32) {
                            output.push(ch);
                        }
                        index += 4;
                    }
                    other => output.push(other as char),
                }
            }
            other => output.push(other as char),
        }
    }
    None
}

fn extract_json_string_field(source: &str, field: &str) -> Option<String> {
    let needle = format!("\"{field}\"");
    let bytes = source.as_bytes();
    let mut start = 0usize;
    while let Some(relative) = source.get(start..)?.find(&needle) {
        let mut index = start + relative + needle.len();
        while bytes.get(index).copied()?.is_ascii_whitespace() {
            index += 1;
        }
        if bytes.get(index).copied()? != b':' {
            start = index;
            continue;
        }
        index += 1;
        while bytes.get(index).copied()?.is_ascii_whitespace() {
            index += 1;
        }
        if let Some((value, _)) = parse_json_string_at(bytes, index) {
            return Some(value);
        }
        start = index;
    }
    None
}

struct HttpUrl {
    host: String,
    port: u16,
    path: String,
}

fn parse_http_url(url: &str) -> Result<HttpUrl, String> {
    let rest = url
        .strip_prefix("http://")
        .ok_or_else(|| "only http:// provider URLs are supported in this carrier".to_string())?;
    let (authority, path) = rest
        .split_once('/')
        .map(|(authority, path)| (authority, format!("/{path}")))
        .unwrap_or((rest, "/".to_string()));
    let (host, port) = authority
        .rsplit_once(':')
        .map(|(host, port)| {
            port.parse::<u16>()
                .map(|port| (host.to_string(), port))
                .map_err(|error| format!("invalid provider port: {error}"))
        })
        .transpose()?
        .unwrap_or_else(|| (authority.to_string(), 80));
    Ok(HttpUrl { host, port, path })
}

fn decode_chunked_body(body: &[u8]) -> Result<Vec<u8>, String> {
    let mut index = 0usize;
    let mut decoded = Vec::new();
    loop {
        let Some(line_end) = body[index..].windows(2).position(|pair| pair == b"\r\n") else {
            return Err("invalid chunked response".to_string());
        };
        let size_line = std::str::from_utf8(&body[index..index + line_end])
            .map_err(|error| format!("invalid chunk header: {error}"))?;
        let size_text = size_line.split(';').next().unwrap_or(size_line).trim();
        let size = usize::from_str_radix(size_text, 16)
            .map_err(|error| format!("invalid chunk size: {error}"))?;
        index += line_end + 2;
        if size == 0 {
            break;
        }
        let chunk_end = index + size;
        if chunk_end + 2 > body.len() {
            return Err("truncated chunked response".to_string());
        }
        decoded.extend_from_slice(&body[index..chunk_end]);
        index = chunk_end + 2;
    }
    Ok(decoded)
}

fn provider_chat_completion(
    config: &ProviderConfig,
    participant_view: &str,
    prompt: &str,
) -> Result<String, String> {
    let url = parse_http_url(&config.base_url)?;
    let mut system_prompt = "You are the case-bound model provider subject inside YAI. Answer only from the supplied YAI participant view and the operator prompt. Use a natural, direct style: start with the answer, avoid boilerplate openings, and do not begin every response with phrases like `according to the current projection`. Speak in terms of authority granted by the current case projection when authority matters, especially for refusals, missing access, proposed actions or boundary explanations. In those cases, prefer bounded authority language such as `the participant view does not grant authority to...` over absolute physical-capability claims. For ordinary factual answers, cite the visible record refs or record kinds when useful instead of repeating the authority preamble. Format for terminal reading with short paragraphs or compact bullets, and keep technical identifiers unchanged. Your outputs are claims/proposals/model interpretations, not authoritative case state, YAI decisions, policy rules, receipts or filesystem effects. subject:linenoise-terminal is only a display/input surface and never owns execution or decision authority. Any proposed action must become an op_attempt and pass through control/carrier before any effect. Do not claim raw journal, filesystem, shell, environment, API-key or out-of-case memory access unless explicitly provided by the participant view.".to_string();
    if config.language_mode == "auto" {
        system_prompt.push_str(" Respond in the same natural language as the operator prompt. Do not translate technical identifiers, record kinds, subject refs, case refs, decision ids, receipt ids, command names or code spans.");
    }
    let user_content =
        format!("YAI participant view:\n{participant_view}\n\nOperator prompt:\n{prompt}");
    let body = format!(
        "{{\"model\":\"{}\",\"stream\":false,\"messages\":[{{\"role\":\"system\",\"content\":\"{}\"}},{{\"role\":\"user\",\"content\":\"{}\"}}]}}",
        json_escape(&config.model),
        json_escape(&system_prompt),
        json_escape(&user_content)
    );
    let auth = config
        .api_key
        .as_deref()
        .map(|key| format!("Authorization: Bearer {key}\r\n"))
        .unwrap_or_default();
    let request = format!(
        "POST {} HTTP/1.1\r\nHost: {}\r\nContent-Type: application/json\r\nAccept: application/json\r\n{}Content-Length: {}\r\nConnection: close\r\n\r\n{}",
        url.path,
        url.host,
        auth,
        body.len(),
        body
    );
    let mut stream = TcpStream::connect((url.host.as_str(), url.port))
        .map_err(|error| format!("failed to connect provider: {error}"))?;
    stream
        .write_all(request.as_bytes())
        .map_err(|error| format!("failed to write provider request: {error}"))?;
    let mut response = Vec::new();
    stream
        .read_to_end(&mut response)
        .map_err(|error| format!("failed to read provider response: {error}"))?;
    let split = response
        .windows(4)
        .position(|window| window == b"\r\n\r\n")
        .ok_or_else(|| "invalid HTTP provider response".to_string())?;
    let headers = String::from_utf8_lossy(&response[..split]).to_string();
    let body_bytes = &response[split + 4..];
    if !headers.starts_with("HTTP/1.1 2") && !headers.starts_with("HTTP/1.0 2") {
        return Err(format!(
            "provider returned non-2xx response: {}",
            compact_text(&String::from_utf8_lossy(body_bytes), 240)
        ));
    }
    let lower_headers = headers.to_ascii_lowercase();
    let body_bytes = if lower_headers.contains("transfer-encoding: chunked") {
        decode_chunked_body(body_bytes)?
    } else {
        body_bytes.to_vec()
    };
    let body_text = String::from_utf8_lossy(&body_bytes).to_string();
    extract_json_string_field(&body_text, "content").ok_or_else(|| {
        format!(
            "provider response did not contain message content: {}",
            compact_text(&body_text, 240)
        )
    })
}

fn redact_sensitive(value: &str, session: &PromptSession) -> String {
    if let Some(api_key) = session
        .provider
        .api_key
        .as_deref()
        .filter(|api_key| !api_key.is_empty())
    {
        value.replace(api_key, "[redacted:api_key]")
    } else {
        value.to_string()
    }
}

fn transcript_text(value: &str, session: &PromptSession) -> String {
    redact_sensitive(value, session)
        .split_whitespace()
        .collect::<Vec<_>>()
        .join(" ")
}

fn append_model_prompt_attempt(session: &PromptSession, prompt: &str) -> Result<String, String> {
    let journal = Journal::load_jsonl(&session.journal_path)
        .map_err(|error| format!("failed to load {}: {error}", session.journal_path.display()))?;
    let sequence = journal.count() + 1;
    let attempt_id = format!("attempt:model-prompt-{sequence}");
    let record = Record::from_parts(
        format!(
            "model-prompt:{}:{sequence}",
            session.subject_ref.replace(':', "-")
        ),
        &session.case_ref,
        RecordKind::Attempt,
        &session.subject_ref,
        &attempt_id,
        "",
        "",
        prompt_attempt_summary(session, prompt),
    );
    append_record_to_journal(&session.journal_path, &record)?;
    Ok(attempt_id)
}

fn append_model_output_receipt(
    session: &PromptSession,
    attempt_id: &str,
    output: &str,
) -> Result<(), String> {
    let journal = Journal::load_jsonl(&session.journal_path)
        .map_err(|error| format!("failed to load {}: {error}", session.journal_path.display()))?;
    let sequence = journal.count() + 1;
    let receipt_id = format!("receipt:model-output-{sequence}");
    let record = Record::from_parts(
        format!(
            "model-output:{}:{sequence}",
            session.subject_ref.replace(':', "-")
        ),
        &session.case_ref,
        RecordKind::EffectReceipt,
        &session.subject_ref,
        attempt_id,
        "",
        &receipt_id,
        model_output_summary(session, output),
    );
    append_record_to_journal(&session.journal_path, &record)
}

fn append_model_interpretation_record(
    session: &PromptSession,
    attempt_id: &str,
    output: &str,
) -> Result<String, String> {
    let journal = Journal::load_jsonl(&session.journal_path)
        .map_err(|error| format!("failed to load {}: {error}", session.journal_path.display()))?;
    let sequence = journal.count() + 1;
    let summary = format!(
        "model_interpretation:observed source:provider_output authority:not_authoritative_state output_is:claim_or_proposal check_against:decisions_receipts_graph preview:{}",
        compact_text(output, 140)
    );
    let record = Record::from_parts(
        format!(
            "model-interpretation:{}:{sequence}",
            session.subject_ref.replace(':', "-")
        ),
        &session.case_ref,
        RecordKind::ModelInterpretation,
        &session.subject_ref,
        attempt_id,
        "",
        "",
        summary.clone(),
    );
    append_record_to_journal(&session.journal_path, &record)?;
    Ok(summary)
}

fn append_participant_view_frame(session: &PromptSession) -> Result<String, String> {
    let journal = Journal::load_jsonl(&session.journal_path)
        .map_err(|error| format!("failed to load {}: {error}", session.journal_path.display()))?;
    let sequence = journal.count() + 1;
    let previous_frame_id =
        latest_frame_id(&journal, &session.case_ref, &session.active_thread_id).unwrap_or_default();
    let frame_id = format!("frame:participant-view-{sequence}");
    let included_turn_count =
        thread_turn_count(&journal, &session.case_ref, &session.active_thread_id);
    let included_memory_count = journal
        .records()
        .iter()
        .filter(|record| {
            record.case_ref == session.case_ref && record.kind == RecordKind::MemoryCandidate
        })
        .count();
    let included_receipt_count = journal
        .records()
        .iter()
        .filter(|record| {
            record.case_ref == session.case_ref
                && matches!(
                    record.kind,
                    RecordKind::Receipt | RecordKind::EffectReceipt | RecordKind::FilesystemReceipt
                )
        })
        .count();
    let projection_id = journal
        .records()
        .iter()
        .rev()
        .find(|record| {
            record.case_ref == session.case_ref && record.kind == RecordKind::ProjectionResult
        })
        .map(|record| record.id.as_str())
        .unwrap_or("projection:current");
    let freshness = projection_freshness_view(&session.case_ref, "model");
    let summary = format!(
        "participant_view_frame frame_id:{frame_id} case_ref:{} thread_id:{} projection_id:{projection_id} previous_frame_id:{} delta_since_frame_id:{} included_turn_count:{included_turn_count} included_memory_count:{included_memory_count} included_receipt_count:{included_receipt_count} redaction:summary_only freshness:{} freshness_source:{} stale_reason_at_build:{} freshness_policy:{} source:active_thread_plus_case_projection",
        session.case_ref,
        session.active_thread_id,
        if previous_frame_id.is_empty() { "none" } else { previous_frame_id.as_str() },
        if previous_frame_id.is_empty() { "none" } else { previous_frame_id.as_str() },
        freshness.freshness,
        freshness.source,
        freshness.stale_reason,
        freshness.policy
    );
    let record = Record::from_parts(
        &frame_id,
        &session.case_ref,
        RecordKind::ParticipantViewFrame,
        &session.subject_ref,
        "",
        "",
        "",
        summary,
    );
    append_record_to_journal(&session.journal_path, &record)?;
    Ok(frame_id)
}

fn append_interaction_turn(
    session: &PromptSession,
    attempt_id: &str,
    prompt: &str,
    output: &str,
) -> Result<String, String> {
    let journal = Journal::load_jsonl(&session.journal_path)
        .map_err(|error| format!("failed to load {}: {error}", session.journal_path.display()))?;
    let sequence = journal.count() + 1;
    let record_id = format!(
        "interaction-turn:{}:{sequence}",
        session.active_thread_id.replace(':', "-")
    );
    let summary = format!(
        "interaction_turn:{sequence} thread_id:{} attempt_id:{attempt_id} prompt_preview:{} output_preview:{} transcript_retention:{} lane:selected_thread audit:journal_retained",
        session.active_thread_id,
        compact_text(prompt, 100),
        compact_text(output, 120),
        transcript_retention_label(session.transcript_enabled)
    );
    let record = Record::from_parts(
        &record_id,
        &session.case_ref,
        RecordKind::InteractionTurn,
        &session.subject_ref,
        attempt_id,
        "",
        "",
        summary,
    );
    append_record_to_journal(&session.journal_path, &record)?;
    Ok(record_id)
}

fn prompt_attempt_summary(session: &PromptSession, prompt: &str) -> String {
    if session.transcript_enabled {
        format!(
            "op:model.prompt.submit prompt_surface:vendored_linenoise context:participant_view_frame thread_id:{} transcript_retention:full_redacted_case_local prompt_text:{}",
            session.active_thread_id,
            transcript_text(prompt, session)
        )
    } else {
        format!(
            "op:model.prompt.submit prompt_surface:vendored_linenoise context:participant_view_frame thread_id:{} transcript_retention:preview_only prompt_preview:{}",
            session.active_thread_id,
            compact_text(prompt, 120)
        )
    }
}

fn model_output_summary(session: &PromptSession, output: &str) -> String {
    if session.transcript_enabled {
        format!(
            "model.output status:observed provider:openai_compatible model:{} output_chars:{} transcript_retention:full_redacted_case_local output_text:{}",
            session.provider.model,
            output.chars().count(),
            transcript_text(output, session)
        )
    } else {
        format!(
            "model.output status:observed provider:openai_compatible model:{} output_chars:{} transcript_retention:preview_only output_preview:{}",
            session.provider.model,
            output.chars().count(),
            compact_text(output, 160)
        )
    }
}

fn append_transcript_retention_state(
    session: &PromptSession,
    enabled: bool,
) -> Result<String, String> {
    let journal = Journal::load_jsonl(&session.journal_path)
        .map_err(|error| format!("failed to load {}: {error}", session.journal_path.display()))?;
    let sequence = journal.count() + 1;
    let state = if enabled { "enabled" } else { "disabled" };
    let full_transcript = if enabled { "on_explicit" } else { "off" };
    let summary = format!(
        "prompt_transcript_retention:{state} scope:case_local redaction:secret_redacted prompt_preview:on provider_output_preview:on full_transcript:{full_transcript} memory_candidate:derived_not_raw_chat"
    );
    let record = Record::from_parts(
        format!(
            "prompt-retention:{}:{sequence}",
            session.subject_ref.replace(':', "-")
        ),
        &session.case_ref,
        RecordKind::SubjectState,
        &session.subject_ref,
        "",
        "",
        "",
        summary.clone(),
    );
    append_record_to_journal(&session.journal_path, &record)?;
    Ok(summary)
}

fn append_memory_proposal(session: &PromptSession, note: Option<&str>) -> Result<String, String> {
    let journal = Journal::load_jsonl(&session.journal_path)
        .map_err(|error| format!("failed to load {}: {error}", session.journal_path.display()))?;
    let prompt_attempts = journal
        .records()
        .iter()
        .filter(|record| {
            record.case_ref == session.case_ref
                && record.kind == RecordKind::Attempt
                && record.summary.contains("op:model.prompt.submit")
        })
        .count();
    let model_outputs = journal
        .records()
        .iter()
        .filter(|record| {
            record.case_ref == session.case_ref
                && record.kind == RecordKind::EffectReceipt
                && record.summary.contains("model.output")
        })
        .count();
    let basis_records = prompt_attempts + model_outputs;
    let sequence = journal.count() + 1;
    let note = note
        .filter(|value| !value.trim().is_empty())
        .map(|value| format!(" note:{}", compact_text(value, 120)))
        .unwrap_or_default();
    let summary = format!(
        "memory:operational scope:case source:prompt_session transcript_retention:{} basis_records:{} basis_prompt_attempts:{} basis_model_outputs:{} summary:prompt session tested case boundary and model/provider residue{}",
        transcript_retention_label(session.transcript_enabled),
        basis_records,
        prompt_attempts,
        model_outputs,
        note
    );
    let record = Record::from_parts(
        format!(
            "prompt-memory:{}:{sequence}",
            session.subject_ref.replace(':', "-")
        ),
        &session.case_ref,
        RecordKind::MemoryCandidate,
        &session.subject_ref,
        "",
        "",
        "",
        summary.clone(),
    );
    append_record_to_journal(&session.journal_path, &record)?;
    Ok(summary)
}

fn run_prompt_once(session: &mut PromptSession, prompt: &str, dry_run: bool) -> Result<(), String> {
    let colors = color_enabled();
    let freshness = projection_freshness_view(&session.case_ref, "model");
    if dry_run {
        println!("model_prompt: dry_run");
        println!("case_ref: {}", session.case_ref);
        println!("case_session: active");
        println!("case_context: active");
        println!("interaction_thread: {}", session.active_thread_id);
        println!("participant_view_frame: would_build");
        println!("projection_freshness: {}", freshness.freshness);
        println!("stale_reason: {}", freshness.stale_reason);
        println!("freshness_policy: {}", freshness.policy);
        println!("freshness_source: {}", freshness.source);
        if freshness.policy == "refresh_required" || freshness.policy == "blocked_for_model" {
            println!("refresh_required: true");
        }
        println!("subject_ref: {}", session.subject_ref);
        println!("provider_base_url: {}", session.provider.base_url);
        println!("provider_model: {}", session.provider.model);
        println!("context_source: interaction_thread_plus_projection_frame");
        println!(
            "transcript_retention: {}",
            transcript_retention_label(session.transcript_enabled)
        );
        println!("raw_journal_access: not_provided");
        println!("filesystem_access: not_provided");
        println!("decision_authority: not_provided");
        println!("receipt_authority: not_provided");
        println!("prompt_preview: {}", compact_text(prompt, 160));
        return Ok(());
    }

    if freshness.policy == "refresh_required" || freshness.policy == "blocked_for_model" {
        println!("projection: stale");
        println!("stale_reason: {}", freshness.stale_reason);
        println!("freshness_policy: {}", freshness.policy);
        println!("refresh_required: true");
    }
    let frame_id = append_participant_view_frame(session)?;
    let attempt_id = append_model_prompt_attempt(session, prompt)?;
    let output = provider_chat_completion(&session.provider, &session.participant_view, prompt)?;
    println!();
    print_cli_section(colors, "MODEL", &session.provider.model, ANSI_MAGENTA);
    print_model_output(colors, &output);
    println!();
    append_model_output_receipt(session, &attempt_id, &output)?;
    let interpretation_summary = append_model_interpretation_record(session, &attempt_id, &output)?;
    let turn_id = append_interaction_turn(session, &attempt_id, prompt, &output)?;
    let journal = Journal::load_jsonl(&session.journal_path)
        .map_err(|error| format!("failed to load {}: {error}", session.journal_path.display()))?;
    session.participant_view =
        render_participant_view(&journal, &session.case_ref, &session.active_thread_id);
    println!("participant_view_frame: {frame_id}");
    println!("interaction_turn: {turn_id}");
    println!(
        "model_interpretation: {}",
        compact_text(&interpretation_summary, 120)
    );
    Ok(())
}

fn handle_prompt_command(session: &mut PromptSession, command: &str) -> Result<bool, String> {
    if command == "/refresh" {
        let journal = Journal::load_jsonl(&session.journal_path).map_err(|error| {
            format!("failed to load {}: {error}", session.journal_path.display())
        })?;
        session.participant_view =
            render_participant_view(&journal, &session.case_ref, &session.active_thread_id);
        session.transcript_enabled =
            transcript_retention_enabled(&journal, &session.case_ref, &session.subject_ref);
        let frame_id = append_participant_view_frame(session)?;
        println!("case_prompt: refreshed");
        println!("participant_view_frame: {frame_id}");
        println!("interaction_thread: {}", session.active_thread_id);
        println!(
            "transcript_retention: {}",
            transcript_retention_label(session.transcript_enabled)
        );
        return Ok(true);
    }

    if command == "/transcript status" {
        println!(
            "transcript_retention: {}",
            transcript_retention_label(session.transcript_enabled)
        );
        println!(
            "prompt_transcript_retention: {}",
            if session.transcript_enabled {
                "enabled"
            } else {
                "disabled"
            }
        );
        println!("prompt_preview: on");
        println!("provider_output_preview: on");
        println!(
            "full_transcript: {}",
            if session.transcript_enabled {
                "on_explicit_redacted_case_local"
            } else {
                "off"
            }
        );
        println!("memory_candidate: derived_not_raw_chat");
        return Ok(true);
    }

    if command == "/thread status" {
        let journal = Journal::load_jsonl(&session.journal_path).map_err(|error| {
            format!("failed to load {}: {error}", session.journal_path.display())
        })?;
        println!("interaction_thread: {}", session.active_thread_id);
        println!(
            "thread_turn_count: {}",
            thread_turn_count(&journal, &session.case_ref, &session.active_thread_id)
        );
        println!("participant_view: active_thread_plus_projection");
        println!("journal_role: replay_audit_not_chat_memory");
        return Ok(true);
    }

    if command == "/thread list" {
        let journal = Journal::load_jsonl(&session.journal_path).map_err(|error| {
            format!("failed to load {}: {error}", session.journal_path.display())
        })?;
        let mut seen = Vec::<String>::new();
        for record in journal.records().iter().filter(|record| {
            record.case_ref == session.case_ref && record.kind == RecordKind::InteractionThread
        }) {
            if let Some(thread_id) = summary_token(&record.summary, "thread_id") {
                if !seen.iter().any(|value| value == &thread_id) {
                    println!(
                        "interaction_thread: {} turns:{}",
                        thread_id,
                        thread_turn_count(&journal, &session.case_ref, &thread_id)
                    );
                    seen.push(thread_id);
                }
            }
        }
        if seen.is_empty() {
            println!("interaction_thread: none");
        }
        return Ok(true);
    }

    if command == "/thread new" || command.starts_with("/thread new ") {
        let label = command
            .strip_prefix("/thread new")
            .map(str::trim)
            .filter(|value| !value.is_empty())
            .unwrap_or("thread");
        let journal = Journal::load_jsonl(&session.journal_path).map_err(|error| {
            format!("failed to load {}: {error}", session.journal_path.display())
        })?;
        let thread_id = format!("thread:{}", journal.count() + 1);
        append_thread_record(
            &session.journal_path,
            &session.case_ref,
            &session.subject_ref,
            &thread_id,
            label,
            "active",
        )?;
        let journal = Journal::load_jsonl(&session.journal_path).map_err(|error| {
            format!("failed to load {}: {error}", session.journal_path.display())
        })?;
        session.active_thread_id = thread_id.clone();
        session.participant_view =
            render_participant_view(&journal, &session.case_ref, &session.active_thread_id);
        println!("interaction_thread: new active");
        println!("thread_id: {thread_id}");
        println!("participant_view: empty thread");
        println!("journal:audit retained");
        return Ok(true);
    }

    if let Some(thread_id) = command.strip_prefix("/thread use ").map(str::trim) {
        if thread_id.is_empty() {
            println!("thread_use: missing_thread_id");
            return Ok(true);
        }
        append_thread_record(
            &session.journal_path,
            &session.case_ref,
            &session.subject_ref,
            thread_id,
            thread_id,
            "active",
        )?;
        let journal = Journal::load_jsonl(&session.journal_path).map_err(|error| {
            format!("failed to load {}: {error}", session.journal_path.display())
        })?;
        session.active_thread_id = thread_id.to_string();
        session.participant_view =
            render_participant_view(&journal, &session.case_ref, &session.active_thread_id);
        println!("interaction_thread: restored previous");
        println!("thread_id: {thread_id}");
        println!(
            "thread_turn_count: {}",
            thread_turn_count(&journal, &session.case_ref, &session.active_thread_id)
        );
        return Ok(true);
    }

    if let Some(thread_id) = command.strip_prefix("/thread archive ").map(str::trim) {
        if thread_id.is_empty() {
            println!("thread_archive: missing_thread_id");
            return Ok(true);
        }
        append_thread_record(
            &session.journal_path,
            &session.case_ref,
            &session.subject_ref,
            thread_id,
            thread_id,
            "archived",
        )?;
        if session.active_thread_id == thread_id {
            session.active_thread_id = DEFAULT_THREAD_ID.to_string();
            append_thread_record(
                &session.journal_path,
                &session.case_ref,
                &session.subject_ref,
                DEFAULT_THREAD_ID,
                "default",
                "active",
            )?;
        }
        let journal = Journal::load_jsonl(&session.journal_path).map_err(|error| {
            format!("failed to load {}: {error}", session.journal_path.display())
        })?;
        session.participant_view =
            render_participant_view(&journal, &session.case_ref, &session.active_thread_id);
        println!("interaction_thread: archived");
        println!("thread_id: {thread_id}");
        println!("active_thread_id: {}", session.active_thread_id);
        return Ok(true);
    }

    if command == "/transcript on" {
        let summary = append_transcript_retention_state(session, true)?;
        session.transcript_enabled = true;
        let _ = writeln!(session.participant_view, "## Prompt Session State");
        let _ = writeln!(
            session.participant_view,
            "- kind:subject_state subject_ref:{} summary:{}",
            session.subject_ref, summary
        );
        let _ = writeln!(session.participant_view);
        println!("prompt_transcript_retention: enabled");
        println!("transcript_retention: full_redacted_case_local");
        println!("full_transcript: on_explicit_redacted_case_local");
        println!("redaction: secret_redacted");
        return Ok(true);
    }

    if command == "/transcript off" {
        let summary = append_transcript_retention_state(session, false)?;
        session.transcript_enabled = false;
        let _ = writeln!(session.participant_view, "## Prompt Session State");
        let _ = writeln!(
            session.participant_view,
            "- kind:subject_state subject_ref:{} summary:{}",
            session.subject_ref, summary
        );
        let _ = writeln!(session.participant_view);
        println!("prompt_transcript_retention: disabled");
        println!("transcript_retention: preview_only");
        println!("full_transcript: off");
        return Ok(true);
    }

    if command == "/memory propose" || command.starts_with("/memory propose ") {
        let note = command.strip_prefix("/memory propose").map(str::trim);
        let summary = append_memory_proposal(session, note)?;
        let _ = writeln!(session.participant_view, "## Prompt Session Memory");
        let _ = writeln!(
            session.participant_view,
            "- kind:memory_candidate subject_ref:{} summary:{}",
            session.subject_ref, summary
        );
        let _ = writeln!(session.participant_view);
        println!("memory_proposal: accepted");
        println!("record_kind: memory_candidate");
        println!(
            "transcript_retention: {}",
            transcript_retention_label(session.transcript_enabled)
        );
        return Ok(true);
    }

    if command.starts_with('/') {
        println!("unknown_command: {command}");
        println!("commands: /thread status /thread new [label] /thread list /thread use <thread_id> /thread archive <thread_id> /refresh /transcript on /transcript off /transcript status /memory propose /exit");
        return Ok(true);
    }

    Ok(false)
}

fn prompt_repl(args: &[String]) -> Result<(), String> {
    let dry_run = args.iter().any(|arg| arg == "--dry-run");
    let once = optional_arg(args, "--once");
    let mut session = prompt_session_from_args(args)?;
    let colors = color_enabled();
    if let Some(prompt) = once {
        if handle_prompt_command(&mut session, prompt.trim())? {
            return Ok(());
        }
        return run_prompt_once(&mut session, &prompt, dry_run);
    }

    let mut stdin = std::io::stdin();
    if !stdin.is_terminal() {
        let mut prompt = String::new();
        stdin
            .read_to_string(&mut prompt)
            .map_err(|error| format!("failed to read prompt from stdin: {error}"))?;
        let prompt = prompt.trim();
        if prompt.is_empty() {
            return Err("prompt stdin is empty".to_string());
        }
        if handle_prompt_command(&mut session, prompt)? {
            return Ok(());
        }
        return run_prompt_once(&mut session, prompt, dry_run);
    }

    unsafe {
        let _ = linenoiseHistorySetMaxLen(200);
    }
    println!("case_prompt: entered");
    println!("case_ref: {}", session.case_ref);
    println!("case_session: active");
    println!("case_context: active");
    println!("interaction_thread: {}", session.active_thread_id);
    println!("subject_ref: {}", session.subject_ref);
    println!("provider_model: {}", session.provider.model);
    println!("context_source: interaction_thread_plus_projection_frame");
    println!(
        "transcript_retention: {}",
        transcript_retention_label(session.transcript_enabled)
    );
    println!("commands: /thread status /thread new [label] /thread list /thread use <thread_id> /thread archive <thread_id> /refresh /transcript on /transcript off /transcript status /memory propose /exit");

    loop {
        println!();
        print_cli_section(colors, "QUESTION", &session.case_ref, ANSI_BLUE);
        let prompt = prompt_label(&session.case_ref, colors);
        let Some(line) = linenoise_read_line(&prompt)? else {
            break;
        };
        let trimmed = line.trim();
        if trimmed.is_empty() {
            continue;
        }
        if trimmed == "/exit" || trimmed == "/quit" {
            break;
        }
        if handle_prompt_command(&mut session, trimmed)? {
            continue;
        }
        if let Ok(history_line) = CString::new(trimmed) {
            unsafe {
                let _ = linenoiseHistoryAdd(history_line.as_ptr());
            }
        }
        if let Err(error) = run_prompt_once(&mut session, trimmed, dry_run) {
            eprintln!("{error}");
        }
    }
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

const GRAPH_NODE_KINDS: &[&str] = &[
    "case",
    "subject",
    "operation",
    "attempt",
    "decision",
    "dispatch",
    "carrier",
    "receipt",
    "effect",
    "observation",
    "divergence",
    "policy",
    "projection",
    "memory_candidate",
    "model_output",
    "model_interpretation",
    "record",
    "unknown",
];

const GRAPH_EDGE_KINDS: &[&str] = &[
    "belongs_to_case",
    "subject_participates_in_case",
    "attempt_targets_subject",
    "decision_controls_attempt",
    "dispatch_routes_decision",
    "carrier_realizes_dispatch",
    "receipt_records_effect",
    "observation_checks_effect",
    "divergence_describes_mismatch",
    "policy_constrains_subject",
    "policy_constrains_operation",
    "projection_exposes_record",
    "model_output_produces_interpretation",
    "memory_derived_from_receipt",
    "record_materializes_node",
    "derived_from",
    "supports",
    "contradicts",
    "unknown",
];

fn graph_schema(args: &[String]) -> Result<(), String> {
    if !args.is_empty() {
        return Err("usage: yai graph schema".to_string());
    }
    println!("graph_schema:");
    println!("  node_kinds:");
    for kind in GRAPH_NODE_KINDS {
        println!("  - {kind}");
    }
    println!();
    println!("  edge_kinds:");
    for kind in GRAPH_EDGE_KINDS {
        println!("  - {kind}");
    }
    println!();
    println!("graph_persistence:");
    println!("  status: active_minimal");
    println!("  durable_truth: typed_relations");
    println!("  relation_write_path: active_minimal");
    println!("  graph_store: {GRAPH_RELATION_STORE_NAME}");
    println!("runtime_graph:");
    println!("  status: active_minimal");
    println!("  role: in_memory_active_case_working_set");
    println!("  working_set: per_command_ephemeral");
    println!("  resident_service: planned");
    println!("  source: graph_relations");
    println!("  hnsw: future_candidate_index");
    println!("  context_compiler: future_consumer");
    Ok(())
}

fn graph_runtime_status(args: &[String]) -> Result<(), String> {
    if !args.is_empty() {
        return Err("usage: yai graph runtime-status".to_string());
    }
    println!("runtime_graph:");
    println!("  status: active_minimal");
    println!("  role: in_memory_active_case_working_set");
    println!("  working_set: per_command_ephemeral");
    println!("  resident_service: planned");
    println!("  source: graph_relations");
    println!("  durable_truth: graph_persistence");
    println!("  hnsw: future_candidate_index");
    println!("  context_compiler: future_consumer");
    println!("  relation_write_path: active_minimal");
    println!("  graph_store: {GRAPH_RELATION_STORE_NAME}");
    println!("  graph_persistence: durable_typed_relations");
    println!("  implementation_claim: ephemeral_working_set_only");
    Ok(())
}

fn graph_runtime_load(args: &[String], summary_only: bool) -> Result<(), String> {
    let case_ref = named_arg(args, "--case")?;
    let status = LmdbRecordStore::status(record_store_path());
    if status.status != RecordStoreStatusKind::Ready {
        print_non_ready_record_store(&status);
        return Ok(());
    }
    let store = LmdbRecordStore::open(&status.path)?;
    let graph = store.load_runtime_graph_for_case(&case_ref)?;
    if summary_only {
        println!("runtime_graph_summary:");
    } else {
        println!("runtime_graph_load:");
    }
    println!("case_ref: {}", graph.case_ref);
    println!("source: {}", graph.source);
    println!("nodes_total: {}", graph.nodes_total);
    println!("edges_total: {}", graph.edges_total);
    println!("outgoing_index_entries: {}", graph.outgoing_index_entries);
    println!("incoming_index_entries: {}", graph.incoming_index_entries);
    println!("generation: {}", graph.generation);
    println!("dirty: {}", yes_no(graph.dirty));
    println!("stale: {}", yes_no(graph.stale));
    println!("durable_truth: {}", graph.durable_truth);
    println!("resident: false");
    println!("resident_service: planned");
    println!("hnsw: future_candidate_index");
    println!("context_compiler: future_consumer");
    Ok(())
}

fn graph_materialize(args: &[String]) -> Result<(), String> {
    let case_ref = named_arg(args, "--case")?;
    let status = LmdbRecordStore::status(record_store_path());
    if status.status != RecordStoreStatusKind::Ready {
        print_non_ready_record_store(&status);
        return Ok(());
    }
    let store = LmdbRecordStore::open(&status.path)?;
    let report = store.materialize_graph_relations_for_case(&case_ref)?;
    println!("graph_materialize:");
    println!("case_ref: {case_ref}");
    println!("source: lmdb_records");
    println!("relations_seen: {}", report.relations_seen);
    println!("relations_written: {}", report.relations_written);
    println!("relations_duplicate: {}", report.relations_duplicate);
    println!("relations_skipped: {}", report.relations_skipped);
    println!("schema: {GRAPH_RELATION_SCHEMA}");
    println!("graph_store: {GRAPH_RELATION_STORE_NAME}");
    println!("runtime_graph_updated: false");
    Ok(())
}

fn graph_relations(args: &[String]) -> Result<(), String> {
    let case_ref = named_arg(args, "--case")?;
    let limit = parse_limit(args)?;
    let status = LmdbRecordStore::status(record_store_path());
    if status.status != RecordStoreStatusKind::Ready {
        print_non_ready_record_store(&status);
        return Ok(());
    }
    let store = LmdbRecordStore::open(&status.path)?;
    let result = store.list_graph_relations_by_case(&case_ref, limit)?;
    println!("graph_relations:");
    println!("case_ref: {case_ref}");
    println!("relations_total: {}", result.relations_total);
    println!("limit: {limit}");
    if result.relations.is_empty() {
        println!("relations: none");
    } else {
        println!("relations:");
        for relation in result.relations {
            println!("- relation_id: {}", relation.relation_id);
            println!("  edge_kind: {}", relation.edge_kind);
            println!("  from_ref: {}", relation.from_ref);
            println!("  to_ref: {}", relation.to_ref);
            println!("  source_record_id: {}", relation.source_record_id);
        }
    }
    Ok(())
}

fn yes_no(value: bool) -> &'static str {
    if value {
        "yes"
    } else {
        "no"
    }
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
fn daemon_request_response(args: &[String], request: &str) -> Result<String, String> {
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
    Ok(response)
}

#[cfg(unix)]
fn daemon_request(args: &[String], request: &str) -> Result<(), String> {
    let response = daemon_request_response(args, request)?;
    print!("{response}");
    Ok(())
}

#[cfg(unix)]
fn daemon_request_and_import_records(args: &[String], request: &str) -> Result<(), String> {
    let response = daemon_request_response(args, request)?;
    print!("{response}");
    if extract_json_string_field(&response, "status").as_deref() != Some("completed") {
        return Ok(());
    }
    let journal_path = extract_json_string_field(&response, "journal_path")
        .ok_or_else(|| "daemon response did not include journal_path".to_string())?;
    import_journal_to_record_store(&PathBuf::from(journal_path))
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
fn daemon_request_and_import_records(_args: &[String], _request: &str) -> Result<(), String> {
    Err("daemon IPC is only implemented on Unix in NEW.11".to_string())
}

#[cfg(not(unix))]
fn daemon_request_with_journal(_args: &[String], _request: &str) -> Result<(), String> {
    Err("daemon IPC is only implemented on Unix in NEW.13".to_string())
}

fn main() {
    let args = std::env::args().skip(1).collect::<Vec<_>>();
    let result = match args.first().map(String::as_str) {
        None if env_var("YAI_CASE_REF").is_some() => {
            if let Err(error) = prompt_repl(&[]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("--version") | Some("version") => println!("yai {}", VERSION),
        Some("info") => print_info(),
        Some("doctor") => print_doctor(),
        Some("store") if args.get(1).map(String::as_str) == Some("status") => print_store_status(),
        Some("store") if args.get(1).map(String::as_str) == Some("summary") => {
            if let Err(error) = print_store_summary() {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("store")
            if args.get(1).map(String::as_str) == Some("record")
                && args.get(2).map(String::as_str) == Some("get") =>
        {
            if let Err(error) = store_record_get(&args[3..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("store")
            if args.get(1).map(String::as_str) == Some("record")
                && args.get(2).map(String::as_str) == Some("list") =>
        {
            if let Err(error) = store_record_list(&args[3..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("store") if args.get(1).map(String::as_str) == Some("tail") => {
            if let Err(error) = store_tail(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("journal") if args.get(1).map(String::as_str) == Some("inspect") => {
            if let Err(error) = journal_inspect(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("journal") if args.get(1).map(String::as_str) == Some("replay") => {
            if let Err(error) = journal_replay(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("journal") if args.get(1).map(String::as_str) == Some("replay-status") => {
            if let Err(error) = journal_replay_status(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("journal") if args.get(1).map(String::as_str) == Some("replay-report") => {
            if let Err(error) = journal_replay_report(&args[2..]) {
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
        Some("case") if args.get(1).map(String::as_str) == Some("enter") => {
            if let Err(error) = case_enter(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("case") if args.get(1).map(String::as_str) == Some("attach-provider") => {
            if let Err(error) = case_attach_provider(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("prompt") => {
            if let Err(error) = prompt_repl(&args[1..]) {
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
        Some("graph") if args.get(1).map(String::as_str) == Some("schema") => {
            if let Err(error) = graph_schema(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("graph") if args.get(1).map(String::as_str) == Some("runtime-status") => {
            if let Err(error) = graph_runtime_status(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("graph") if args.get(1).map(String::as_str) == Some("runtime-load") => {
            if let Err(error) = graph_runtime_load(&args[2..], false) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("graph") if args.get(1).map(String::as_str) == Some("runtime-summary") => {
            if let Err(error) = graph_runtime_load(&args[2..], true) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("graph") if args.get(1).map(String::as_str) == Some("materialize") => {
            if let Err(error) = graph_materialize(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("graph") if args.get(1).map(String::as_str) == Some("relations") => {
            if let Err(error) = graph_relations(&args[2..]) {
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
        Some("hot") if args.get(1).map(String::as_str) == Some("status") => {
            if let Err(error) = print_hot_status() {
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
            if let Err(error) = daemon_request_and_import_records(
                &args[2..],
                "run_minimum_loop request_id=yai-minimum case_ref=case:new12-daemon subject_ref=subject:repo-test",
            ) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("daemon") if args.get(1).map(String::as_str) == Some("run-filesystem-loop") => {
            if let Err(error) = daemon_request_and_import_records(
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
        Some("carrier") if args.get(1).map(String::as_str) == Some("families") => {
            print_carrier_families();
        }
        Some("carrier") if args.get(1).map(String::as_str) == Some("lanes") => {
            print_carrier_lanes();
        }
        Some("carrier") if args.get(1).map(String::as_str) == Some("route") => {
            if let Err(error) = carrier_route(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("carrier") if args.get(1).map(String::as_str) == Some("coverage") => {
            if let Err(error) = carrier_coverage(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("carrier") if args.get(1).map(String::as_str) == Some("inspect") => {
            if let Err(error) = carrier_inspect(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("carrier") if args.get(1).map(String::as_str) == Some("outcome-test") => {
            if let Err(error) = carrier_outcome_test(&args[2..]) {
                eprintln!("error: {error}");
                std::process::exit(2);
            }
        }
        Some("carrier") if args.get(1).map(String::as_str) == Some("reconcile-outcome") => {
            if let Err(error) = carrier_reconcile_outcome(&args[2..]) {
                eprintln!("error: {error}");
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
        Some("device") if args.get(1).map(String::as_str) == Some("add") => {
            if let Err(error) = device_add(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("device") if args.get(1).map(String::as_str) == Some("list") => {
            if let Err(error) = device_list() {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("device") if args.get(1).map(String::as_str) == Some("inspect") => {
            if let Err(error) = device_inspect(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("device") if args.get(1).map(String::as_str) == Some("trust") => {
            if let Err(error) = set_device_trust(&args[2..], "trusted") {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("device") if args.get(1).map(String::as_str) == Some("untrust") => {
            if let Err(error) = set_device_trust(&args[2..], "untrusted") {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("provider")
            if args.get(1).map(String::as_str) == Some("runtime")
                && args.get(2).map(String::as_str) == Some("status") =>
        {
            print_provider_runtime_status();
        }
        Some("provider") if args.get(1).map(String::as_str) == Some("targets") => {
            print_provider_targets();
        }
        Some("provider") if args.get(1).map(String::as_str) == Some("start") => {
            if let Err(error) = provider_start_plan(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("provider") if args.get(1).map(String::as_str) == Some("logs-path") => {
            if let Err(error) = print_provider_logs_path() {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("model")
            if args.get(1).map(String::as_str) == Some("catalog")
                && args.get(2).map(String::as_str) == Some("status") =>
        {
            print_model_catalog_status();
        }
        Some("model")
            if args.get(1).map(String::as_str) == Some("runtime")
                && args.get(2).map(String::as_str) == Some("status") =>
        {
            print_model_runtime_status();
        }
        Some("process") if args.get(1).map(String::as_str) == Some("observe") => {
            if let Err(error) = process_observe(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("process") if args.get(1).map(String::as_str) == Some("signal") => {
            if let Err(error) = process_signal(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("observe") if args.get(1).map(String::as_str) == Some("process") => {
            if let Err(error) = observe_process(&args[2..]) {
                eprintln!("{error}");
                std::process::exit(2);
            }
        }
        Some("observe") if args.get(1).map(String::as_str) == Some("compare-process") => {
            if let Err(error) = observe_compare_process(&args[2..]) {
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
