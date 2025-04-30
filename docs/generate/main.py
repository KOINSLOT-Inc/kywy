# SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

import os
import re

import mkdocs_gen_files

# copy in repo markdown files
for repo_file_name, docs_file_name in [
    ("README.md", "index.md"),
    ("ROADMAP.md", "roadmap.md"),
    ("getting_started.md", "getting_started.md"),
]:
    with mkdocs_gen_files.open(docs_file_name, "w") as f:
        with open(repo_file_name) as repo_file:
            text = repo_file.read()

            # replace links to docs site
            text = re.sub(
                r"\(http(s|)://docs.kywy.io/([^)]+)\)", "(./\\2/index.md)", text
            )
            text = text.replace("(./api_reference/index.md)", "(./api_reference.md)")

            print("---\nhide:\n  - navigation\n  - footer\n---\n", file=f)
            print(text, file=f)

# make repo assets available to docs
for asset in os.listdir("assets"):
    if asset.endswith(".license"):
        continue

    with open(f"assets/{asset}", "rb") as asset_file:
        with mkdocs_gen_files.open(f"assets/{asset}", "wb") as f:
            f.write(asset_file.read())
