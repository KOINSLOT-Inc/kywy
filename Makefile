# SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

.PHONY: help
help:
	@echo "Kywy Makefile"
	@echo ""
	@echo "Commands:"
	@echo "- 'lint': lints all files (code, config, license, etc.)"
	@echo "- 'check-licenses': runs 'reuse' to check licenses"
	@echo "- 'update-licenses': runs 'reuse' to update licenses"

.PHONY: check-licenses
check-licenses:
	@pipenv run reuse lint

.PHONY: update-licenses
update-licenses: $(PYTHON)
	@pipenv run reuse annotate \
	    --copyright "KOINSLOT, Inc." \
	    --year $$(date +%Y) \
	    --license "GPL-3.0-or-later" \
	    --merge-copyrights \
	    --recursive \
	    --fallback-dot-license \
	    .

.PHONY: lint
lint: check-licenses
