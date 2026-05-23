//! Rust operational data engine FFI boundary.
//!
//! The C ABI owns public semantics. These functions expose only an opaque
//! engine handle, JSONL input and caller-owned output buffers.

use crate::journal::Journal;
use crate::projection::ProjectionSummary;
use crate::query::{QueryFilter, QueryResult};
use crate::record::{Record, RecordKind};
use std::ffi::{CStr, CString};
use std::os::raw::c_char;
use std::ptr;

pub const FFI_BOUNDARY_RULE: &str = "no Rust types across public C ABI";

const YAI_ENGINE_OK: i32 = 0;
const YAI_ENGINE_INVALID: i32 = 1;
const YAI_ENGINE_BUFFER_TOO_SMALL: i32 = 2;

pub struct EngineHandle {
    journal: Journal,
    last_error: String,
}

impl EngineHandle {
    fn new() -> Self {
        Self {
            journal: Journal::new(),
            last_error: String::new(),
        }
    }

    fn set_error(&mut self, error: impl Into<String>) -> i32 {
        self.last_error = error.into();
        YAI_ENGINE_INVALID
    }
}

unsafe fn engine_mut<'a>(handle: *mut EngineHandle) -> Option<&'a mut EngineHandle> {
    if handle.is_null() {
        None
    } else {
        Some(&mut *handle)
    }
}

unsafe fn cstr_to_str<'a>(value: *const c_char) -> Option<&'a str> {
    if value.is_null() {
        return None;
    }
    CStr::from_ptr(value).to_str().ok()
}

unsafe fn write_output(buffer: *mut c_char, buffer_len: usize, value: &str) -> i32 {
    if buffer.is_null() || buffer_len == 0 {
        return YAI_ENGINE_INVALID;
    }
    let bytes = value.as_bytes();
    if bytes.len() + 1 > buffer_len {
        return YAI_ENGINE_BUFFER_TOO_SMALL;
    }
    ptr::copy_nonoverlapping(bytes.as_ptr(), buffer.cast::<u8>(), bytes.len());
    *buffer.add(bytes.len()) = 0;
    YAI_ENGINE_OK
}

fn count_kind(journal: &Journal, kind: RecordKind) -> usize {
    journal
        .records()
        .iter()
        .filter(|record| record.kind == kind)
        .count()
}

#[no_mangle]
pub extern "C" fn yai_engine_new() -> *mut EngineHandle {
    Box::into_raw(Box::new(EngineHandle::new()))
}

#[no_mangle]
pub unsafe extern "C" fn yai_engine_free(handle: *mut EngineHandle) {
    if !handle.is_null() {
        drop(Box::from_raw(handle));
    }
}

#[no_mangle]
pub unsafe extern "C" fn yai_engine_append_record_json(
    handle: *mut EngineHandle,
    json_line: *const c_char,
) -> i32 {
    let Some(engine) = engine_mut(handle) else {
        return YAI_ENGINE_INVALID;
    };
    let Some(line) = cstr_to_str(json_line) else {
        return engine.set_error("invalid json line");
    };
    let Some(record) = Record::from_jsonl(line) else {
        return engine.set_error("record decode failed");
    };
    engine.journal.append(record);
    engine.last_error.clear();
    YAI_ENGINE_OK
}

#[no_mangle]
pub unsafe extern "C" fn yai_engine_record_count(handle: *const EngineHandle) -> usize {
    if handle.is_null() {
        return 0;
    }
    (*handle).journal.count()
}

#[no_mangle]
pub unsafe extern "C" fn yai_engine_count_kind(
    handle: *const EngineHandle,
    kind: *const c_char,
) -> usize {
    if handle.is_null() {
        return 0;
    }
    let Some(kind_value) = cstr_to_str(kind) else {
        return 0;
    };
    let Some(record_kind) = RecordKind::from_str(kind_value) else {
        return 0;
    };
    count_kind(&(*handle).journal, record_kind)
}

#[no_mangle]
pub unsafe extern "C" fn yai_engine_query_kind(
    handle: *const EngineHandle,
    kind: *const c_char,
) -> usize {
    if handle.is_null() {
        return 0;
    }
    let Some(kind_value) = cstr_to_str(kind) else {
        return 0;
    };
    let Some(record_kind) = RecordKind::from_str(kind_value) else {
        return 0;
    };
    let filter = QueryFilter {
        record_kind: Some(record_kind),
        ..Default::default()
    };
    QueryResult::scan(&(*handle).journal, &filter).matched
}

#[no_mangle]
pub unsafe extern "C" fn yai_engine_projection_summary_json(
    handle: *const EngineHandle,
    buffer: *mut c_char,
    buffer_len: usize,
) -> i32 {
    if handle.is_null() {
        return YAI_ENGINE_INVALID;
    }
    let projection = ProjectionSummary::from_journal("rust_engine", &(*handle).journal);
    let summary = format!(
        "{{\"records\":{},\"receipts\":{},\"case_domains\":{},\"case_attachments\":{},\"case_bindings\":{},\"graph_edges\":{},\"memory_candidates\":{},\"projection_results\":{},\"divergences\":{}}}",
        projection.source_record_count,
        projection.receipt_count,
        projection.case_domain_count,
        projection.case_attachment_count,
        projection.case_binding_count,
        projection.graph_edge_count,
        projection.memory_candidate_count,
        projection.projection_result_count,
        projection.divergence_count
    );
    write_output(buffer, buffer_len, &summary)
}

#[no_mangle]
pub unsafe extern "C" fn yai_engine_last_error(
    handle: *const EngineHandle,
    buffer: *mut c_char,
    buffer_len: usize,
) -> i32 {
    if handle.is_null() {
        return YAI_ENGINE_INVALID;
    }
    write_output(buffer, buffer_len, &(*handle).last_error)
}

#[no_mangle]
pub unsafe extern "C" fn yai_engine_string_free(value: *mut c_char) {
    if !value.is_null() {
        drop(CString::from_raw(value));
    }
}
