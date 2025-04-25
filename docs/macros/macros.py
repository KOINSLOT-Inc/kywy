# SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# see the link below for documentation on mkdocs-macros modules
# https://mkdocs-macros-plugin.readthedocs.io/en/latest/macros/

from PIL import Image

import screen_renderer
import code_includer


def define_env(env):
    with open("library.properties") as f:
        lines = f.readlines()

    for line in lines:
        if line.startswith("version="):
            env.variables["version"] = line.strip().replace("version=", "")

    @env.macro
    def kywy_screen(*args, **kwargs):
        return screen_renderer.kywy_screen(env, *args, **kwargs)

    @env.macro
    def kywy_screen_gif(*args, **kwargs):
        return screen_renderer.kywy_screen_gif(env, *args, **kwargs)

    @env.macro
    def include_code(*args, **kwargs):
        return code_includer.include_code(env, *args, **kwargs)
