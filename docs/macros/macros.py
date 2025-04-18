# SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# see the link below for documentation on mkdocs-macros modules
# https://mkdocs-macros-plugin.readthedocs.io/en/latest/macros/

def define_env(env):
    with open("library.properties") as f:
        lines = f.readlines()

    for line in lines:
        if line.startswith("version="):
            env.variables["version"] = line.strip().replace("version=", "")
