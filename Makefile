.PHONY: info check-layout check-docs build-c build-rust build-rust-ffi build smoke-new1 smoke-new2 smoke-new3 smoke-new4 smoke-new5 smoke-new6 smoke-new7 smoke-new8 smoke-new9 smoke-new10 smoke check clean

CC ?= cc
AR ?= ar
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -Iinclude
RUST_FFI_LIBS ?= -lpthread -ldl -lm
BUILD_DIR := build
RUST_ENGINE_STATIC := crates/target/debug/libyai_core_engine.a

C_SOURCES := \
	lib/internal/string_util.c \
	lib/base/id.c \
	lib/base/error.c \
	lib/case/case_ref.c \
	lib/subject/subject_ref.c \
	lib/subject/subject_binding.c \
	lib/subject/subject_state.c \
	lib/op/attempt.c \
	lib/control/failure_mode.c \
	lib/control/policy_rule.c \
	lib/control/gate.c \
	lib/control/decision_basis.c \
	lib/control/decision.c \
	lib/control/obligation.c \
	lib/control/receipt_requirement.c \
	lib/effect/carrier.c \
	lib/effect/effect_hash.c \
	lib/effect/receipt.c \
	lib/effect/carriers/filesystem_carrier.c \
	lib/memory/memory_kind.c \
	lib/memory/memory_scope.c \
	lib/memory/memory_candidate.c \
	lib/reconcile/divergence.c \
	lib/reconcile/reconcile.c \
	lib/reconcile/recovery.c \
	lib/index/query.c \
	lib/index/query_filter.c \
	lib/index/query_result.c \
	lib/graph/edge.c \
	lib/graph/graph.c \
	lib/graph/reconstruct.c \
	lib/store/record.c \
	lib/store/journal.c \
	lib/store/record_codec.c \
	lib/store/journal_file.c \
	lib/store/rust_engine_backend.c \
	lib/projection/projection.c \
	lib/projection/projection_kind.c \
	lib/projection/redaction.c \
	lib/projection/freshness.c \
	lib/projection/projection_request.c \
	lib/projection/projection_result.c

C_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
C_LIBRARY := $(BUILD_DIR)/libyai_core_new10.a
YAID := $(BUILD_DIR)/yaid
SMOKE_MINIMUM_LOOP := $(BUILD_DIR)/test_minimum_loop
SMOKE_PERSISTENT_JOURNAL := $(BUILD_DIR)/test_persistent_journal
SMOKE_CONTROL_GATE := $(BUILD_DIR)/test_control_gate
SMOKE_FILESYSTEM_CARRIER := $(BUILD_DIR)/test_filesystem_carrier
SMOKE_GRAPH_RECONSTRUCTION := $(BUILD_DIR)/test_graph_reconstruction
SMOKE_OPERATIONAL_MEMORY := $(BUILD_DIR)/test_operational_memory
SMOKE_RECONCILE_DIVERGENCE := $(BUILD_DIR)/test_reconcile_divergence
SMOKE_PROJECTION_HARDENING := $(BUILD_DIR)/test_projection_hardening
SMOKE_QUERY_BOUNDARY := $(BUILD_DIR)/test_query_boundary
SMOKE_RUST_ENGINE_R1 := $(BUILD_DIR)/test_rust_engine_r1

info:
	@printf "yai-core: rust operational data engine r1\n"
	@printf "status: NEW.10\n"
	@printf "ctl: Rust yaictl\n"
	@printf "engine: Rust operational data skeleton, C file journal path\n"

check-layout:
	@./tools/checks/check-no-old-roots.sh
	@./tools/checks/check-required-layout.sh
	@./tools/checks/check-source-placement.sh

check-docs:
	@./tools/checks/check-doc-canonical-location.sh
	@./tools/checks/check-doc-required-files.sh
	@./tools/checks/check-doc-no-old-root-language.sh

$(BUILD_DIR)/%.o: %.c
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(C_LIBRARY): $(C_OBJECTS)
	@mkdir -p "$(dir $@)"
	$(AR) rcs "$@" $(C_OBJECTS)

$(YAID): daemon/main.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) daemon/main.c $(C_LIBRARY) -o "$@"

$(SMOKE_MINIMUM_LOOP): tests/smoke/minimum-loop/test_minimum_loop.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/minimum-loop/test_minimum_loop.c $(C_LIBRARY) -o "$@"

$(SMOKE_PERSISTENT_JOURNAL): tests/smoke/persistent-journal/test_persistent_journal.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/persistent-journal/test_persistent_journal.c $(C_LIBRARY) -o "$@"

$(SMOKE_CONTROL_GATE): tests/smoke/control-gate/test_control_gate.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/control-gate/test_control_gate.c $(C_LIBRARY) -o "$@"

$(SMOKE_FILESYSTEM_CARRIER): tests/smoke/filesystem-carrier/test_filesystem_carrier.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/filesystem-carrier/test_filesystem_carrier.c $(C_LIBRARY) -o "$@"

$(SMOKE_GRAPH_RECONSTRUCTION): tests/smoke/graph-reconstruction/test_graph_reconstruction.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/graph-reconstruction/test_graph_reconstruction.c $(C_LIBRARY) -o "$@"

$(SMOKE_OPERATIONAL_MEMORY): tests/smoke/operational-memory/test_operational_memory.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/operational-memory/test_operational_memory.c $(C_LIBRARY) -o "$@"

$(SMOKE_RECONCILE_DIVERGENCE): tests/smoke/reconcile-divergence/test_reconcile_divergence.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/reconcile-divergence/test_reconcile_divergence.c $(C_LIBRARY) -o "$@"

$(SMOKE_PROJECTION_HARDENING): tests/smoke/projection-hardening/test_projection_hardening.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/projection-hardening/test_projection_hardening.c $(C_LIBRARY) -o "$@"

$(SMOKE_QUERY_BOUNDARY): tests/smoke/query-boundary/test_query_boundary.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/query-boundary/test_query_boundary.c $(C_LIBRARY) -o "$@"

$(SMOKE_RUST_ENGINE_R1): tests/smoke/rust-engine-r1/test_rust_engine_r1.c $(C_LIBRARY) $(RUST_ENGINE_STATIC)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/rust-engine-r1/test_rust_engine_r1.c $(C_LIBRARY) $(RUST_ENGINE_STATIC) $(RUST_FFI_LIBS) -o "$@"

build-c: build-rust-ffi $(C_LIBRARY) $(YAID) $(SMOKE_MINIMUM_LOOP) $(SMOKE_PERSISTENT_JOURNAL) $(SMOKE_CONTROL_GATE) $(SMOKE_FILESYSTEM_CARRIER) $(SMOKE_GRAPH_RECONSTRUCTION) $(SMOKE_OPERATIONAL_MEMORY) $(SMOKE_RECONCILE_DIVERGENCE) $(SMOKE_PROJECTION_HARDENING) $(SMOKE_QUERY_BOUNDARY) $(SMOKE_RUST_ENGINE_R1)

build-rust-ffi:
	cargo build --manifest-path crates/Cargo.toml -p yai-core-engine

build-rust:
	cargo build --manifest-path crates/Cargo.toml --workspace
	cargo test --manifest-path crates/Cargo.toml --workspace

build: build-c build-rust

smoke-new1: $(SMOKE_MINIMUM_LOOP)
	@$(SMOKE_MINIMUM_LOOP)

smoke-new2: $(SMOKE_PERSISTENT_JOURNAL)
	@$(SMOKE_PERSISTENT_JOURNAL)

smoke-new3: $(SMOKE_CONTROL_GATE)
	@$(SMOKE_CONTROL_GATE)

smoke-new4: $(SMOKE_FILESYSTEM_CARRIER)
	@$(SMOKE_FILESYSTEM_CARRIER)

smoke-new5: $(SMOKE_GRAPH_RECONSTRUCTION)
	@$(SMOKE_GRAPH_RECONSTRUCTION)

smoke-new6: $(SMOKE_OPERATIONAL_MEMORY)
	@$(SMOKE_OPERATIONAL_MEMORY)

smoke-new7: $(SMOKE_RECONCILE_DIVERGENCE)
	@$(SMOKE_RECONCILE_DIVERGENCE)

smoke-new8: $(SMOKE_PROJECTION_HARDENING)
	@$(SMOKE_PROJECTION_HARDENING)

smoke-new9: $(SMOKE_QUERY_BOUNDARY)
	@$(SMOKE_QUERY_BOUNDARY)

smoke-new10: $(SMOKE_RUST_ENGINE_R1)
	@$(SMOKE_RUST_ENGINE_R1)

smoke: smoke-new1 smoke-new2 smoke-new3 smoke-new4 smoke-new5 smoke-new6 smoke-new7 smoke-new8 smoke-new9 smoke-new10

check: check-layout check-docs build smoke

clean:
	rm -rf "$(BUILD_DIR)" crates/target
