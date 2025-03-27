# SPDX-FileCopyrightText: 2023 - 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

PYTHON_DEV_TOOLS := $(CACHE)/.python-dev-tools
$(PYTHON_DEV_TOOLS):
	pipenv install --dev
	@touch $(PYTHON_DEV_TOOLS)


# update licenses with
# pipenv run reuse annotate \
#     --copyright "KOINSLOT, Inc." \
#     --year 2025 \
#     --license "GPL-3.0-or-later" \
#     --merge-copyrights \
#     --recursive \
#     --fallback-dot-license \
#     .
#
.PHONY: lint
lint:
	pipenv run reuse lint

