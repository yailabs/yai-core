.PHONY: info check-layout check-docs check-repository-identity check-archive-historical-records check-source-surface-clean check-file-header-standard check-pack-doctrine check-foundation-freeze check-hot-state-doctrine check-hot-state-freeze check-lmdb-record-plane-doctrine build-c build-rust build-rust-ffi build install-local uninstall-local doctor-local print-install-paths smoke-new1 smoke-new2 smoke-new3 smoke-new4 smoke-new5 smoke-new6 smoke-new7 smoke-new8 smoke-new9 smoke-new10 smoke-new11 smoke-new12 smoke-new18b smoke-new18c smoke-spine23 smoke-spine24 smoke-spine24a smoke-spine25 smoke-spine26 smoke-spine27 smoke-spine29 smoke-spine30 smoke-spine31 smoke-spine32 smoke check clean

CC ?= cc
AR ?= ar
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -Iinclude
RUST_FFI_LIBS ?= -lpthread -ldl -lm
PREFIX ?= $(HOME)/.local
YAI_HOME ?= $(HOME)/.yai
BUILD_DIR := build
RUST_TARGET_DIR := target
RUST_ENGINE_STATIC := $(RUST_TARGET_DIR)/debug/libyai_core_engine.a
INSTALL_BINDIR := $(PREFIX)/bin
YAI_BIN := $(RUST_TARGET_DIR)/debug/yai
YAI_RUN_DIR := $(YAI_HOME)/run
YAI_STORE_DIR := $(YAI_HOME)/store
YAI_RECORD_STORE_DIR := $(YAI_STORE_DIR)/lmdb
YAI_LOG_DIR := $(YAI_HOME)/log
YAI_TMP_DIR := $(YAI_HOME)/tmp
YAI_CASES_DIR := $(YAI_HOME)/cases
YAI_SOCKETS_DIR := $(YAI_HOME)/sockets
YAI_CONFIG_DIR := $(YAI_HOME)/config
YAI_DAEMON_SOCKET := $(YAI_RUN_DIR)/yaid.sock

C_SOURCES := \
	system/internal/string_util.c \
	system/base/id.c \
	system/base/error.c \
	system/hot/hot_flags.c \
	system/hot/hot_snapshot.c \
	system/hot/hot_state.c \
	system/case/case_context.c \
	system/case/interaction_thread.c \
	system/case/participant_view.c \
	system/case/case_ref.c \
	system/case/case_session.c \
	system/case/case_world.c \
	system/subject/subject_ref.c \
	system/subject/subject_binding.c \
	system/subject/subject_state.c \
	system/op/attempt.c \
	system/control/failure_mode.c \
	system/control/policy_rule.c \
	system/control/gate.c \
	system/control/decision_basis.c \
	system/control/decision.c \
	system/control/obligation.c \
	system/control/receipt_requirement.c \
	system/daemon/daemon_status.c \
	system/effect/carrier.c \
	system/effect/effect_hash.c \
	system/effect/receipt.c \
	system/effect/carriers/filesystem_carrier.c \
	system/memory/memory_kind.c \
	system/memory/memory_scope.c \
	system/memory/memory_candidate.c \
	system/reconcile/divergence.c \
	system/reconcile/reconcile.c \
	system/reconcile/recovery.c \
	system/index/query.c \
	system/index/query_filter.c \
	system/index/query_result.c \
	system/graph/edge.c \
	system/graph/graph.c \
	system/graph/reconstruct.c \
	system/store/record.c \
	system/store/journal.c \
	system/store/record_codec.c \
	system/store/journal_file.c \
	system/engine_bridge/rust_engine_backend.c \
	system/projection/projection.c \
	system/projection/projection_kind.c \
	system/projection/redaction.c \
	system/projection/freshness.c \
	system/projection/projection_request.c \
	system/projection/projection_result.c

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
SMOKE_CASE_CONTEXT := $(BUILD_DIR)/test_case_context
SMOKE_INTERACTION_THREAD := $(BUILD_DIR)/test_interaction_thread
SMOKE_HOT_STATE := $(BUILD_DIR)/test_hot_state
SMOKE_HOT_STATE_SNAPSHOT := tests/smoke/hot-state-snapshot/test_hot_state_snapshot.sh
SMOKE_COMMAND_SURFACE := tests/smoke/command-surface/test_command_surface.sh
SMOKE_HOT_STATE_SESSION := $(BUILD_DIR)/test_hot_state_session
SMOKE_PROJECTION_FRESHNESS := $(BUILD_DIR)/test_projection_freshness
SMOKE_HOT_STATE_CLI := tests/smoke/hot-state-cli/test_hot_state_cli.sh
SMOKE_RECORD_STORE_CLI := tests/smoke/record-store-cli/test_record_store_cli.sh
SMOKE_RECORD_STORE_WRITE := tests/smoke/record-store-write/test_record_store_write.sh
SMOKE_RECORD_STORE_READ_QUERY := tests/smoke/record-store-read-query/test_record_store_read_query.sh
SMOKE_RECORD_STORE_SUBJECT_RECEIPT := tests/smoke/record-store-subject-receipt-indexes/test_record_store_subject_receipt_indexes.sh
SMOKE_DAEMON_IPC := tests/smoke/daemon-ipc/test_daemon_ipc.sh
SMOKE_DAEMON_CORE_LOOP := tests/smoke/daemon-core-loop/test_daemon_core_loop.sh

info:
	@printf "yai: local AI operational control core\n"
	@printf "status: SPINE.32 LMDB Case / Subject / Receipt Indexes\n"
	@printf "completed: SPINE.20 Local Runtime Layout through SPINE.32 LMDB Case / Subject / Receipt Indexes\n"
	@printf "next: SPINE.33 LMDB CLI + Manual Validation\n"
	@printf "target-layout: include/ system/ engine/ cmd/\n"
	@printf "runtime-home: YAI_HOME=%s socket=%s\n" "$(YAI_HOME)" "$(YAI_DAEMON_SOCKET)"
	@printf "hot-state: %s/hot-state.json\n" "$(YAI_RUN_DIR)"
	@printf "record-store: %s\n" "$(YAI_RECORD_STORE_DIR)"
	@printf "pack-doctrine: active docs/engineering/pack-format.md\n"
	@printf "foundation-freeze: filesystem closed; runtime layout exists; active docs compact; extraction contract active\n"
	@printf "data-spine-c: transitional keep_temporarily\n"
	@printf "engine-bridge: active\n"
	@printf "lib: removed\n"
	@printf "daemon: moved to cmd/yaid + system/daemon\n"
	@printf "crates: removed\n"
	@printf "ctl: removed\n"
	@printf "install-local: active PREFIX=%s YAI_HOME=%s\n" "$(PREFIX)" "$(YAI_HOME)"

check-layout:
	@./tools/checks/check-no-old-roots.sh
	@./tools/checks/check-required-layout.sh
	@./tools/checks/check-source-placement.sh
	@./tools/checks/check-source-surface-clean.sh

check-docs:
	@./tools/checks/check-doc-canonical-location.sh
	@./tools/checks/check-doc-required-files.sh
	@./tools/checks/check-doc-no-old-root-language.sh
	@./tools/checks/check-repository-identity.sh
	@./tools/checks/check-archive-historical-records.sh
	@./tools/checks/check-pack-doctrine.sh
	@./tools/checks/check-foundation-freeze.sh
	@./tools/checks/check-hot-state-doctrine.sh
	@./tools/checks/check-hot-state-freeze.sh
	@./tools/checks/check-lmdb-record-plane-doctrine.sh
	@./tools/checks/check-file-header-standard.sh

check-repository-identity:
	@./tools/checks/check-repository-identity.sh

check-archive-historical-records:
	@./tools/checks/check-archive-historical-records.sh

check-source-surface-clean:
	@./tools/checks/check-source-surface-clean.sh

check-file-header-standard:
	@./tools/checks/check-file-header-standard.sh

check-pack-doctrine:
	@./tools/checks/check-pack-doctrine.sh

check-foundation-freeze:
	@./tools/checks/check-foundation-freeze.sh

check-hot-state-doctrine:
	@./tools/checks/check-hot-state-doctrine.sh

check-hot-state-freeze:
	@./tools/checks/check-hot-state-freeze.sh

check-lmdb-record-plane-doctrine:
	@./tools/checks/check-lmdb-record-plane-doctrine.sh

$(BUILD_DIR)/%.o: %.c
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) -c "$<" -o "$@"

$(C_LIBRARY): $(C_OBJECTS)
	@mkdir -p "$(dir $@)"
	$(AR) rcs "$@" $(C_OBJECTS)

$(YAID): cmd/yaid/main.c system/daemon/ipc.c system/daemon/core_loop.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) cmd/yaid/main.c system/daemon/ipc.c system/daemon/core_loop.c $(C_LIBRARY) -o "$@"

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

$(SMOKE_CASE_CONTEXT): tests/smoke/case-context/test_case_context.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/case-context/test_case_context.c $(C_LIBRARY) -o "$@"

$(SMOKE_INTERACTION_THREAD): tests/smoke/interaction-thread/test_interaction_thread.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/interaction-thread/test_interaction_thread.c $(C_LIBRARY) -o "$@"

$(SMOKE_HOT_STATE): tests/smoke/hot-state/test_hot_state.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/hot-state/test_hot_state.c $(C_LIBRARY) -o "$@"

$(SMOKE_HOT_STATE_SESSION): tests/smoke/hot-state-session/test_hot_state_session.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/hot-state-session/test_hot_state_session.c $(C_LIBRARY) -o "$@"

$(SMOKE_PROJECTION_FRESHNESS): tests/smoke/projection-freshness/test_projection_freshness.c $(C_LIBRARY)
	@mkdir -p "$(dir $@)"
	$(CC) $(CFLAGS) tests/smoke/projection-freshness/test_projection_freshness.c $(C_LIBRARY) -o "$@"

build-c: build-rust-ffi $(C_LIBRARY) $(YAID) $(SMOKE_MINIMUM_LOOP) $(SMOKE_PERSISTENT_JOURNAL) $(SMOKE_CONTROL_GATE) $(SMOKE_FILESYSTEM_CARRIER) $(SMOKE_GRAPH_RECONSTRUCTION) $(SMOKE_OPERATIONAL_MEMORY) $(SMOKE_RECONCILE_DIVERGENCE) $(SMOKE_PROJECTION_HARDENING) $(SMOKE_QUERY_BOUNDARY) $(SMOKE_RUST_ENGINE_R1) $(SMOKE_CASE_CONTEXT) $(SMOKE_INTERACTION_THREAD) $(SMOKE_HOT_STATE) $(SMOKE_HOT_STATE_SESSION) $(SMOKE_PROJECTION_FRESHNESS)

build-rust-ffi:
	CARGO_TARGET_DIR=$(RUST_TARGET_DIR) cargo build --manifest-path engine/Cargo.toml -p yai-engine

build-rust:
	CARGO_TARGET_DIR=$(RUST_TARGET_DIR) cargo build --manifest-path engine/Cargo.toml --workspace
	CARGO_TARGET_DIR=$(RUST_TARGET_DIR) cargo test --manifest-path engine/Cargo.toml --workspace
	CARGO_TARGET_DIR=$(RUST_TARGET_DIR) cargo build --manifest-path cmd/yai/Cargo.toml
	CARGO_TARGET_DIR=$(RUST_TARGET_DIR) cargo test --manifest-path cmd/yai/Cargo.toml

build: build-c build-rust

print-install-paths:
	@printf "PREFIX=%s\n" "$(PREFIX)"
	@printf "YAI_HOME=%s\n" "$(YAI_HOME)"
	@printf "yai=%s/yai\n" "$(INSTALL_BINDIR)"
	@printf "yaid=%s/yaid\n" "$(INSTALL_BINDIR)"
	@printf "run=%s\n" "$(YAI_RUN_DIR)"
	@printf "store=%s\n" "$(YAI_STORE_DIR)"
	@printf "record_store=%s\n" "$(YAI_RECORD_STORE_DIR)"
	@printf "log=%s\n" "$(YAI_LOG_DIR)"
	@printf "tmp=%s\n" "$(YAI_TMP_DIR)"
	@printf "cases=%s\n" "$(YAI_CASES_DIR)"
	@printf "sockets=%s\n" "$(YAI_SOCKETS_DIR)"
	@printf "config=%s\n" "$(YAI_CONFIG_DIR)"
	@printf "socket=%s\n" "$(YAI_DAEMON_SOCKET)"

install-local: build
	@mkdir -p "$(INSTALL_BINDIR)"
	@mkdir -p "$(YAI_RUN_DIR)" "$(YAI_RECORD_STORE_DIR)" "$(YAI_LOG_DIR)" "$(YAI_TMP_DIR)" "$(YAI_CASES_DIR)" "$(YAI_SOCKETS_DIR)" "$(YAI_CONFIG_DIR)"
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
	@printf "YAI_HOME_status: %s\n" "$$(if [ -d "$(YAI_HOME)" ]; then printf "ok"; else printf "missing"; fi)"
	@printf "binary_path: %s\n" "$$(if [ -x "$(INSTALL_BINDIR)/yai" ]; then printf "%s/yai" "$(INSTALL_BINDIR)"; else printf "missing"; fi)"
	@printf "yaid_path: %s\n" "$$(if [ -x "$(INSTALL_BINDIR)/yaid" ]; then printf "%s/yaid" "$(INSTALL_BINDIR)"; else printf "missing"; fi)"
	@printf "yai_version: %s\n" "$$(if [ -x "$(INSTALL_BINDIR)/yai" ]; then "$(INSTALL_BINDIR)/yai" --version; else printf "missing"; fi)"
	@printf "run_dir: %s\n" "$$(if [ -d "$(YAI_RUN_DIR)" ]; then printf "%s" "$(YAI_RUN_DIR)"; else printf "missing"; fi)"
	@printf "store_dir: %s\n" "$$(if [ -d "$(YAI_STORE_DIR)" ]; then printf "%s" "$(YAI_STORE_DIR)"; else printf "missing"; fi)"
	@printf "record_store_path: %s\n" "$(YAI_RECORD_STORE_DIR)"
	@printf "record_store_status: %s\n" "$$(if [ -d "$(YAI_RECORD_STORE_DIR)" ]; then printf "not_initialized"; else printf "missing"; fi)"
	@printf "record_store_backend: lmdb\n"
	@printf "log_dir: %s\n" "$$(if [ -d "$(YAI_LOG_DIR)" ]; then printf "%s" "$(YAI_LOG_DIR)"; else printf "missing"; fi)"
	@printf "tmp_dir: %s\n" "$$(if [ -d "$(YAI_TMP_DIR)" ]; then printf "%s" "$(YAI_TMP_DIR)"; else printf "missing"; fi)"
	@printf "cases_dir: %s\n" "$$(if [ -d "$(YAI_CASES_DIR)" ]; then printf "%s" "$(YAI_CASES_DIR)"; else printf "missing"; fi)"
	@printf "sockets_dir: %s\n" "$$(if [ -d "$(YAI_SOCKETS_DIR)" ]; then printf "%s" "$(YAI_SOCKETS_DIR)"; else printf "missing"; fi)"
	@printf "config_dir: %s\n" "$$(if [ -d "$(YAI_CONFIG_DIR)" ]; then printf "%s" "$(YAI_CONFIG_DIR)"; else printf "missing"; fi)"
	@printf "daemon_socket_default: %s\n" "$(YAI_DAEMON_SOCKET)"
	@printf "hot_state_path: %s/hot-state.json\n" "$(YAI_RUN_DIR)"
	@printf "hot_state_status: %s\n" "$$(if [ -f "$(YAI_RUN_DIR)/hot-state.json" ]; then printf "present"; else printf "missing"; fi)"
	@printf "runtime_layout_status: %s\n" "$$(if [ -d "$(YAI_RUN_DIR)" ] && [ -d "$(YAI_STORE_DIR)" ] && [ -d "$(YAI_LOG_DIR)" ] && [ -d "$(YAI_TMP_DIR)" ] && [ -d "$(YAI_CASES_DIR)" ] && [ -d "$(YAI_SOCKETS_DIR)" ] && [ -d "$(YAI_CONFIG_DIR)" ]; then printf "ok"; else printf "incomplete"; fi)"
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

smoke-new18b: $(SMOKE_CASE_CONTEXT)
	@$(SMOKE_CASE_CONTEXT)

smoke-new18c: $(SMOKE_INTERACTION_THREAD)
	@$(SMOKE_INTERACTION_THREAD)

smoke-spine23: $(SMOKE_HOT_STATE)
	@$(SMOKE_HOT_STATE)

smoke-spine24: $(YAID) build-rust
	@$(SMOKE_HOT_STATE_SNAPSHOT)

smoke-spine24a: $(YAID) build-rust
	@$(SMOKE_COMMAND_SURFACE)

smoke-spine25: $(SMOKE_HOT_STATE_SESSION)
	@$(SMOKE_HOT_STATE_SESSION)

smoke-spine26: $(SMOKE_PROJECTION_FRESHNESS)
	@$(SMOKE_PROJECTION_FRESHNESS)

smoke-spine27: $(YAID) build-rust
	@$(SMOKE_HOT_STATE_CLI)

smoke-spine29: build-rust
	@$(SMOKE_RECORD_STORE_CLI)

smoke-spine30: $(YAID) build-rust
	@$(SMOKE_RECORD_STORE_WRITE)

smoke-spine31: $(YAID) build-rust
	@$(SMOKE_RECORD_STORE_READ_QUERY)

smoke-spine32: $(YAID) build-rust
	@$(SMOKE_RECORD_STORE_SUBJECT_RECEIPT)

smoke: smoke-new1 smoke-new2 smoke-new3 smoke-new4 smoke-new5 smoke-new6 smoke-new7 smoke-new8 smoke-new9 smoke-new10 smoke-new11 smoke-new12 smoke-new18b smoke-new18c smoke-spine23 smoke-spine24 smoke-spine24a smoke-spine25 smoke-spine26 smoke-spine27 smoke-spine29 smoke-spine30 smoke-spine31 smoke-spine32

check: check-layout check-docs build smoke

clean:
	rm -rf "$(BUILD_DIR)" "$(RUST_TARGET_DIR)" engine/target cmd/yai/target
