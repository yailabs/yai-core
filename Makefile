.PHONY: info check-layout check-docs build-c build-rust build-rust-ffi build install-local uninstall-local doctor-local print-install-paths smoke-new1 smoke-new2 smoke-new3 smoke-new4 smoke-new5 smoke-new6 smoke-new7 smoke-new8 smoke-new9 smoke-new10 smoke-new11 smoke-new12 smoke check clean

CC ?= cc
AR ?= ar
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -Iinclude
RUST_FFI_LIBS ?= -lpthread -ldl -lm
PREFIX ?= $(HOME)/.local
YAI_HOME ?= $(HOME)/.yai
BUILD_DIR := build
RUST_ENGINE_STATIC := crates/target/debug/libyai_core_engine.a
INSTALL_BINDIR := $(PREFIX)/bin
YAI_BIN := crates/target/debug/yai

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
	lib/daemon/daemon_status.c \
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
C_LIBRARY := $(BUILD_DIR)/libyai_core_new13.a
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
SMOKE_DAEMON_IPC := tests/smoke/daemon-ipc/test_daemon_ipc.sh
SMOKE_DAEMON_CORE_LOOP := tests/smoke/daemon-core-loop/test_daemon_core_loop.sh

info:
	@printf "yai-core: local AI operational control core\n"
	@printf "status: NEW.13 target filesystem doctrine / refactor plan\n"
	@printf "next: NEW.14 move Rust workspace crates/ -> engine/\n"
	@printf "target-layout: include/ system/ engine/ cmd/\n"
	@printf "transitional: lib/ crates/ ctl/ daemon/\n"
	@printf "install-local: delayed to NEW.20\n"

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

$(YAID): daemon/main.c daemon/ipc.c daemon/core_loop.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) daemon/main.c daemon/ipc.c daemon/core_loop.c $(C_LIBRARY) -o "$@"

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

print-install-paths:
	@printf "PREFIX=%s\n" "$(PREFIX)"
	@printf "YAI_HOME=%s\n" "$(YAI_HOME)"
	@printf "yai=%s/yai\n" "$(INSTALL_BINDIR)"
	@printf "yaid=%s/yaid\n" "$(INSTALL_BINDIR)"
	@printf "run=%s/run\n" "$(YAI_HOME)"
	@printf "store=%s/store\n" "$(YAI_HOME)"
	@printf "log=%s/log\n" "$(YAI_HOME)"
	@printf "tmp=%s/tmp\n" "$(YAI_HOME)"
	@printf "socket=%s/run/yaid.sock\n" "$(YAI_HOME)"

install-local: build
	@mkdir -p "$(INSTALL_BINDIR)"
	@mkdir -p "$(YAI_HOME)/run" "$(YAI_HOME)/store" "$(YAI_HOME)/log" "$(YAI_HOME)/tmp"
	@cp "$(YAI_BIN)" "$(INSTALL_BINDIR)/yai"
	@cp "$(YAID)" "$(INSTALL_BINDIR)/yaid"
	@chmod +x "$(INSTALL_BINDIR)/yai" "$(INSTALL_BINDIR)/yaid"
	@printf "installed: %s/yai\n" "$(INSTALL_BINDIR)"
	@printf "installed: %s/yaid\n" "$(INSTALL_BINDIR)"
	@$(MAKE) --no-print-directory doctor-local PREFIX="$(PREFIX)" YAI_HOME="$(YAI_HOME)"

uninstall-local:
	@rm -f "$(INSTALL_BINDIR)/yai" "$(INSTALL_BINDIR)/yaid"
	@printf "uninstalled local yai binaries from %s\n" "$(INSTALL_BINDIR)"

doctor-local:
	@printf "yai local doctor\n"
	@printf "PREFIX: %s\n" "$(PREFIX)"
	@printf "YAI_HOME: %s\n" "$(YAI_HOME)"
	@printf "yai: %s\n" "$$(if [ -x "$(INSTALL_BINDIR)/yai" ]; then printf "%s/yai" "$(INSTALL_BINDIR)"; else printf "missing"; fi)"
	@printf "yaid: %s\n" "$$(if [ -x "$(INSTALL_BINDIR)/yaid" ]; then printf "%s/yaid" "$(INSTALL_BINDIR)"; else printf "missing"; fi)"
	@printf "run_dir: %s\n" "$$(if [ -d "$(YAI_HOME)/run" ]; then printf "%s/run" "$(YAI_HOME)"; else printf "missing"; fi)"
	@printf "store_dir: %s\n" "$$(if [ -d "$(YAI_HOME)/store" ]; then printf "%s/store" "$(YAI_HOME)"; else printf "missing"; fi)"
	@printf "log_dir: %s\n" "$$(if [ -d "$(YAI_HOME)/log" ]; then printf "%s/log" "$(YAI_HOME)"; else printf "missing"; fi)"
	@printf "tmp_dir: %s\n" "$$(if [ -d "$(YAI_HOME)/tmp" ]; then printf "%s/tmp" "$(YAI_HOME)"; else printf "missing"; fi)"
	@printf "daemon_socket_default: %s/run/yaid.sock\n" "$(YAI_HOME)"
	@case ":$$PATH:" in *:"$(INSTALL_BINDIR)":*) printf "PATH_status: ok\n" ;; *) printf "PATH_status: warning add %s to PATH\n" "$(INSTALL_BINDIR)" ;; esac

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

smoke-new11: $(YAID) build-rust
	@$(SMOKE_DAEMON_IPC)

smoke-new12: $(YAID) build-rust
	@$(SMOKE_DAEMON_CORE_LOOP)

smoke: smoke-new1 smoke-new2 smoke-new3 smoke-new4 smoke-new5 smoke-new6 smoke-new7 smoke-new8 smoke-new9 smoke-new10 smoke-new11 smoke-new12

check: check-layout check-docs build smoke

clean:
	rm -rf "$(BUILD_DIR)" crates/target
