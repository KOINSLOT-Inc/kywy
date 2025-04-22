# SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

# see the link below for documentation on mkdocs-macros modules
# https://mkdocs-macros-plugin.readthedocs.io/en/latest/macros/

import os
import json
import hashlib
import shutil
from pathlib import Path
from typing import List, Tuple, Dict, Any, NewType
from functools import wraps

from PIL import Image

import screen_renderer


def define_env(env):
    with open("library.properties") as f:
        lines = f.readlines()

    for line in lines:
        if line.startswith("version="):
            env.variables["version"] = line.strip().replace("version=", "")

    Operation = NewType("Operation", Tuple[str, List[Any], Dict[str, Any]])

    def kywy_screen_image(operations: List[Operation]) -> Image:
        kywy = screen_renderer.create_blank_kywy()

        for operation, args, kwargs in operations:
            method = getattr(screen_renderer, operation)
            kywy = method(kywy, *args, **kwargs)

        return kywy

    @env.macro
    def kywy_screen(name: str, operations: List[Operation]) -> str:
        img_directory = (
            Path(env.conf["site_dir"]) / Path(os.path.dirname(env.page.url)) / "img"
        )

        os.makedirs(img_directory, exist_ok=True)

        image_path = img_directory / f"{name}.png"

        if os.path.exists(image_path):
            raise Exception(f"image '{name}' already exists")

        input_hash = hashlib.md5(json.dumps(operations).encode()).hexdigest()
        cached_image_path = (
            ".cache/img"
            / Path(os.path.dirname(env.page.url))
            / "img"
            / f"{name}.{input_hash}.png"
        )

        if not os.path.isfile(cached_image_path):
            os.makedirs(os.path.dirname(cached_image_path), exist_ok=True)

            kywy = kywy_screen_image(operations)
            kywy.save(cached_image_path)

        shutil.copy(cached_image_path, image_path)

        return f"""
        <p align="center">
            <img src="./img/{name}.png"/>
        </p>
        """.strip()

    @env.macro
    def kywy_screen_gif(
        name: str, frames: List[List[Operation]], duration: int = 100
    ) -> str:
        img_directory = (
            Path(env.conf["site_dir"]) / Path(os.path.dirname(env.page.url)) / "img"
        )

        os.makedirs(img_directory, exist_ok=True)

        image_path = img_directory / f"{name}.gif"

        if os.path.exists(image_path):
            raise Exception(f"gif '{name}' already exists")

        if len(frames) == 0:
            raise Exception("must provide at least one frame for kywy_screen_gif")

        input_hash = hashlib.md5(json.dumps(frames).encode()).hexdigest()
        cached_image_path = (
            ".cache/img"
            / Path(os.path.dirname(env.page.url))
            / "img"
            / f"{name}.{input_hash}.gif"
        )

        if not os.path.isfile(cached_image_path):
            os.makedirs(os.path.dirname(cached_image_path), exist_ok=True)

            kywy = kywy_screen_image(frames[0])

            append_images = []
            for operations in frames:
                append_images.append(kywy_screen_image(operations))

            kywy.save(
                cached_image_path,
                append_images=append_images,
                loop=0,
                duration=duration,
            )

        shutil.copy(cached_image_path, image_path)

        return f"""
        <p align="center">
            <img src="./img/{name}.gif"/>
        </p>
        """.strip()
