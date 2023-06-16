# SPDX-FileCopyrightText: 2023 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

DEVKIT := docker run -v $$PWD:$$PWD -w $$PWD -it -e LOCAL_IDF_PATH=$$IDF_PATH \
	  -e LOCAL_IDF_TOOLS_PATH=$${IDF_TOOLS_PATH:-~/.espressif} \
	  kywy/devkit:latest

.PHONY: license
license:
	$(DEVKIT) reuse lint

.PHONY: flash-server
flash-server:
	@if [ -n $$IDF_PORT ]; then \
		echo "IDF_PORT is required but not set"; exit 1; \
	fi
	esp_rfc2217_server.py -v -p 4000 $$IDF_PORT

# combine targets and forward into devkit container to avoid duplicate container startup costs
.PHONY: build lint test clean compile_commands.json
$(eval $(firstword $(filter-out flash-server license fonts,$(MAKECMDGOALS))):; $$(DEVKIT) sh -c 'make --file /devkit/Makefile $(MAKECMDGOALS)')
