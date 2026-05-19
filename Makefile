.PHONY: info check-layout check-docs build-c build-rust build smoke check clean

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
	lib/op/attempt.c \
	lib/control/decision.c \
	lib/effect/receipt.c \
	lib/store/record.c \
	lib/store/journal.c \
	lib/projection/projection.c

C_OBJECTS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(C_SOURCES))
C_LIBRARY := $(BUILD_DIR)/libyai_core_new1.a
YAID := $(BUILD_DIR)/yaid
SMOKE_MINIMUM_LOOP := $(BUILD_DIR)/test_minimum_loop

info:
	@printf "yai-core: new core minimum loop\n"
	@printf "status: NEW.1\n"
	@printf "ctl: Rust yaictl\n"
	@printf "engine: Rust operational data skeleton\n"

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

build-c: $(C_LIBRARY) $(YAID) $(SMOKE_MINIMUM_LOOP)

build-rust:
	cargo build --manifest-path crates/Cargo.toml --workspace
	cargo test --manifest-path crates/Cargo.toml --workspace

build: build-c build-rust

smoke: $(SMOKE_MINIMUM_LOOP)
	@$(SMOKE_MINIMUM_LOOP)

check: check-layout check-docs build smoke

clean:
	rm -rf "$(BUILD_DIR)" crates/target
