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
lint-arduino-code: $(ARDUINO_LINT)
	@if [ -f .development ]; then rm .development; fi; \
		$(ARDUINO_LINT) --compliance strict --recursive; \
		exit_code=$$?; \
		touch .development; \
		exit $$exit_code;

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

.PHONY: upload/examples/%
upload/examples/%: $(ARDUINO_CLI)
	@port=$$(arduino-cli board list --json \
		| jq -r '.detected_ports | map(select(.matching_boards)) | .[0].port.address' \
	) \
	&& arduino-cli upload \
		-b arduino:mbed_rp2040:pico \
		-p $$port \
		$$(echo $@ | cut -d'/' -f 2,3)
