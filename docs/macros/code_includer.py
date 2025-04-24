# SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

import os
import textwrap

from pathlib import Path

def include_code(env, file: str, block: str):
    file_path = Path(env.project_dir) / file

    code = ""
    with open(file_path) as f:
        brace_depth = 0
        in_block = False
        for line in f.readlines():
            if block in line and not in_block:
                in_block = True

            if in_block and "{" in line:
                brace_depth += line.count("{")

            if in_block and "}" in line:
                brace_depth -= line.count("}")

            if in_block:
                code += line

            if in_block and brace_depth == 0:
                in_block = False

    code = textwrap.dedent(code)

    code = "```c++\n" + code + "\n```"

    return code
