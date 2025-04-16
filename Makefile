# SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

.PHONY: help
help:
	@echo "Kywy Makefile"
	@echo ""
	@echo "Commands:"
	@echo "- 'check-licenses': runs 'reuse' to check licenses"
	@echo "- 'update-licenses': runs 'reuse' to update licenses"
	@echo "- 'lint': lints all files (code, config, license, etc.)"
	@echo "- 'upload/examples/<example>': uploads the specified '<example>'"

CACHE := .cache
$(CACHE):
	@mkdir .cache

ARDUINO_CLI := $(CACHE)/.arduino-cli
$(ARDUINO_CLI): $(CACHE)
	@which arduino-cli 2>&1 > /dev/null || (echo "no arduino-cli found, try `brew install arduino-cli`" && exit 1)
	@touch $(ARDUINO_CLI)

ARDUINO_LINT := $(CACHE)/bin/arduino-lint
$(ARDUINO_LINT): $(CACHE)
	mkdir -p $(CACHE)/bin
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | BINDIR=$(CACHE)/bin sh

.clang-format:
	@curl -fsSL https://raw.githubusercontent.com/arduino/tooling-project-assets/main/other/clang-format-configuration/.clang-format > .clang-format

CLANG_FORMAT := $(CACHE)/.clang-format
$(CLANG_FORMAT): $(CACHE) .clang-format
	@which clang-format 2>&1 > /dev/null || (echo "no clang-format found, try `brew install clang-format`" && exit 1)
	@if clang-format --version | grep -v -q '14.0'; then (echo "wrong clang-format version found, v14.0 required" && exit 1); fi
	@touch $(CLANG_FORMAT)

.PHONY: check-licenses
check-licenses:
	@pipenv run reuse lint

.PHONY: update-licenses
update-licenses:
	@pipenv run reuse annotate \
	    --copyright "KOINSLOT, Inc." \
	    --year $$(date +%Y) \
	    --license "GPL-3.0-or-later" \
	    --merge-copyrights \
	    --recursive \
	    --fallback-dot-license \
	    .

.PHONY: lint-arduino-code
lint-arduino-code: $(ARDUINO_LINT) $(CLANG_FORMAT)
	@if [ -f .development ]; then rm .development; fi; \
		$(ARDUINO_LINT) --compliance strict --recursive --library-manager update; \
		exit_code=$$?; \
		touch .development; \
		exit $$exit_code;
	@clang-format --dry-run  **/*.cpp **/*.hpp **/*.ino

.PHONY: format-arduino-code
format-arduino-code: $(CLANG_FORMAT)
	@clang-format -i **/*.cpp **/*.hpp **/*.ino

.PHONY: compile-arduino-sketches
compile-arduino-sketches: $(ARDUINO_CLI)
	@start=$$(date +%s) \
	&& num_examples=$$(find examples/* -maxdepth 0 -type d | wc -l | xargs) \
	&& num=0 \
	&& for example in examples/*/; do \
		num=$$((num + 1)) \
		&& echo "($${num}/$${num_examples}) compiling $${example}..." \
		&& arduino-cli compile \
		--fqbn arduino:mbed_rp2040:pico \
		--warnings all \
		$${example} \
		|| exit $$? && echo; \
	done \
	&& echo "compiled $$num_examples examples in $$(($$(date +%s) - start)) seconds"

.PHONY: lint
lint: check-licenses lint-arduino-code

.PHONY: format
format: format-arduino-code

.PHONY: upload/examples/%
upload/examples/%: $(ARDUINO_CLI)
	@port=$$(arduino-cli board list --json \
		| jq -r '.detected_ports | map(select(.matching_boards)) | .[0].port.address' \
	) \
	&& arduino-cli upload \
		-b arduino:mbed_rp2040:pico \
		-p $$port \
		$$(echo $@ | cut -d'/' -f 2,3)
