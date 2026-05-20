.PHONY: info check-layout check-docs build-c build-rust build smoke-new1 smoke-new2 smoke-new3 smoke-new4 smoke check clean

CC ?= cc
AR ?= ar
CFLAGS ?= -std=c11 -Wall -Wextra -Werror -Iinclude
BUILD_DIR := build

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
	lib/store/record.c \
	lib/store/journal.c \
	lib/store/record_codec.c \
	lib/store/journal_file.c \
	lib/projection/projection.c

C_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
C_LIBRARY := $(BUILD_DIR)/libyai_core_new4.a
YAID := $(BUILD_DIR)/yaid
SMOKE_MINIMUM_LOOP := $(BUILD_DIR)/test_minimum_loop
SMOKE_PERSISTENT_JOURNAL := $(BUILD_DIR)/test_persistent_journal
SMOKE_CONTROL_GATE := $(BUILD_DIR)/test_control_gate
SMOKE_FILESYSTEM_CARRIER := $(BUILD_DIR)/test_filesystem_carrier

info:
	@printf "yai-core: filesystem carrier and controlled effects\n"
	@printf "status: NEW.4\n"
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

build-c: $(C_LIBRARY) $(YAID) $(SMOKE_MINIMUM_LOOP) $(SMOKE_PERSISTENT_JOURNAL) $(SMOKE_CONTROL_GATE) $(SMOKE_FILESYSTEM_CARRIER)

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

smoke: smoke-new1 smoke-new2 smoke-new3 smoke-new4

check: check-layout check-docs build smoke

clean:
	rm -rf "$(BUILD_DIR)" crates/target
