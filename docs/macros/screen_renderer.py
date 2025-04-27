# SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

import os
import json
import hashlib
import shutil
from pathlib import Path
from typing import List, Tuple, Dict, Any, NewType

from PIL import Image, ImageDraw

SCREEN_WIDTH = 144
SCREEN_HEIGHT = 168
BEZEL_WIDTH = 10
CHAMFER_WIDTH = 5
TOP_LEFT_RIGHT_BORDER_WIDTH = 30
BOTTOM_BORDER_WIDTH = 60
BORDER_RADIUS = 20
BUTTON_RADIUS = 13
BUTTON_HEIGHT_FROM_BOTTOM = int(CHAMFER_WIDTH + BOTTOM_BORDER_WIDTH * 0.6)
D_PAD_HEIGHT_FROM_BOTTOM = int(CHAMFER_WIDTH + BOTTOM_BORDER_WIDTH * 0.5)
EDGE_INDENT_HEIGHT = 15
EDGE_INDENT_SEPARATION = 25
EDGE_INDENT_HEIGHT_FROM_BOTTOM = int(
    CHAMFER_WIDTH + BOTTOM_BORDER_WIDTH + 0.30 * SCREEN_HEIGHT
)
ON_SWITCH_HEIGHT = EDGE_INDENT_HEIGHT
ON_SWITCH_HEIGHT_FROM_TOP = int(
    CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH + 0.5 * SCREEN_HEIGHT
)

GREEN = "#38a832"
DARK_GREEN = "#2b8026"
BLACK = "#000000"
GREY = "#adadad"
RED = "#b51500"
CLEAR = "#00000000"

FULL_WIDTH = (
    CHAMFER_WIDTH
    + TOP_LEFT_RIGHT_BORDER_WIDTH
    + CHAMFER_WIDTH
    + BEZEL_WIDTH
    + SCREEN_WIDTH
    + BEZEL_WIDTH
    + CHAMFER_WIDTH
    + TOP_LEFT_RIGHT_BORDER_WIDTH
    + CHAMFER_WIDTH
)

FULL_HEIGHT = (
    CHAMFER_WIDTH
    + TOP_LEFT_RIGHT_BORDER_WIDTH
    + CHAMFER_WIDTH
    + BEZEL_WIDTH
    + SCREEN_HEIGHT
    + BEZEL_WIDTH
    + CHAMFER_WIDTH
    + BOTTOM_BORDER_WIDTH
    + CHAMFER_WIDTH
)

SCREEN_ORIGIN_X = (
    0 + CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH + CHAMFER_WIDTH + BEZEL_WIDTH
)
SCREEN_ORIGIN_Y = SCREEN_ORIGIN_X


def draw_left_button(draw, color):
    draw.circle(
        [
            0 + CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH + CHAMFER_WIDTH,
            FULL_HEIGHT - BUTTON_HEIGHT_FROM_BOTTOM,
        ],
        BUTTON_RADIUS,
        fill=color,
    )


def draw_right_button(draw, color):
    draw.circle(
        [
            FULL_WIDTH - (CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH + CHAMFER_WIDTH),
            FULL_HEIGHT - BUTTON_HEIGHT_FROM_BOTTOM,
        ],
        BUTTON_RADIUS,
        fill=color,
    )


def draw_d_pad(draw, color, direction=None):
    # outer grey circle
    draw.circle(
        [int(FULL_WIDTH / 2), FULL_HEIGHT - D_PAD_HEIGHT_FROM_BOTTOM],
        BUTTON_RADIUS,
        fill=GREY,
    )

    x_shift = 0
    y_shift = 0

    if direction is not None:
        if "up" in direction:
            y_shift = -5
        elif "down" in direction:
            y_shift = 5

        if "left" in direction:
            x_shift = -5
        elif "right" in direction:
            x_shift = 5

    draw.circle(
        [
            int(FULL_WIDTH / 2) + x_shift,
            FULL_HEIGHT - D_PAD_HEIGHT_FROM_BOTTOM + y_shift,
        ],
        BUTTON_RADIUS - 2,
        fill=color,
    )


def create_blank_kywy() -> Image:
    # add chamfer width / 2 for on switch but don't include in full width so we don't have to worry about it in most
    # calculations below
    image = Image.new("RGBA", (FULL_WIDTH + CHAMFER_WIDTH, FULL_HEIGHT), CLEAR)
    draw = ImageDraw.Draw(image)

    # power button
    draw.rounded_rectangle(
        [
            [FULL_WIDTH + -1 * CHAMFER_WIDTH / 2, ON_SWITCH_HEIGHT_FROM_TOP],
            [
                FULL_WIDTH + CHAMFER_WIDTH / 2,
                ON_SWITCH_HEIGHT_FROM_TOP + ON_SWITCH_HEIGHT,
            ],
        ],
        CHAMFER_WIDTH / 2,
        fill=BLACK,
    )

    # outer chamfer
    draw.rounded_rectangle(
        [[0, 0], [FULL_WIDTH, FULL_HEIGHT]], BORDER_RADIUS, fill=DARK_GREEN
    )

    # main face
    draw.rounded_rectangle(
        [
            [0 + CHAMFER_WIDTH, 0 + CHAMFER_WIDTH],
            [FULL_WIDTH - CHAMFER_WIDTH, FULL_HEIGHT - CHAMFER_WIDTH],
        ],
        BORDER_RADIUS - CHAMFER_WIDTH,
        fill=GREEN,
    )

    # screen chamfer
    draw.rectangle(
        [
            [
                0 + CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH,
                0 + CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH,
            ],
            [
                FULL_WIDTH - CHAMFER_WIDTH - TOP_LEFT_RIGHT_BORDER_WIDTH,
                FULL_HEIGHT - CHAMFER_WIDTH - BOTTOM_BORDER_WIDTH,
            ],
        ],
        fill=DARK_GREEN,
    )

    # screen bezel
    draw.rectangle(
        [
            [
                0 + CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH + CHAMFER_WIDTH,
                0 + CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH + CHAMFER_WIDTH,
            ],
            [
                FULL_WIDTH
                - CHAMFER_WIDTH
                - TOP_LEFT_RIGHT_BORDER_WIDTH
                - CHAMFER_WIDTH,
                FULL_HEIGHT - CHAMFER_WIDTH - BOTTOM_BORDER_WIDTH - CHAMFER_WIDTH,
            ],
        ],
        fill=BLACK,
    )

    # screen
    draw.rectangle(
        [
            [
                SCREEN_ORIGIN_X,
                SCREEN_ORIGIN_Y,
            ],
            [
                FULL_WIDTH
                - CHAMFER_WIDTH
                - TOP_LEFT_RIGHT_BORDER_WIDTH
                - CHAMFER_WIDTH
                - BEZEL_WIDTH,
                FULL_HEIGHT
                - CHAMFER_WIDTH
                - BOTTOM_BORDER_WIDTH
                - CHAMFER_WIDTH
                - BEZEL_WIDTH,
            ],
        ],
        fill=GREY,
    )

    # buttons
    draw_left_button(draw, BLACK)
    draw_right_button(draw, BLACK)
    draw_d_pad(draw, BLACK)

    # edge indents
    for i in [0, EDGE_INDENT_SEPARATION + EDGE_INDENT_HEIGHT]:
        for j in [0, FULL_WIDTH]:
            draw.rounded_rectangle(
                [
                    [
                        j + -1 * CHAMFER_WIDTH / 2,
                        FULL_HEIGHT - EDGE_INDENT_HEIGHT_FROM_BOTTOM + i,
                    ],
                    [
                        j + CHAMFER_WIDTH / 2,
                        FULL_HEIGHT
                        - EDGE_INDENT_HEIGHT_FROM_BOTTOM
                        + EDGE_INDENT_HEIGHT
                        + i,
                    ],
                ],
                CHAMFER_WIDTH / 2,
                fill=CLEAR,
            )

    return image


Operation = NewType("Operation", Tuple[str, List[Any], Dict[str, Any]])


def kywy_screen_image(
    operations: List[Operation], post_overlay_operations: List[Operation] = []
) -> Image:
    kywy = create_blank_kywy()
    kywy_draw = ImageDraw.Draw(kywy)

    screen = Image.new("RGBA", (SCREEN_WIDTH, SCREEN_HEIGHT), GREY)
    screen_draw = ImageDraw.Draw(screen)

    left_button_pressed = False
    right_button_pressed = False
    d_pad_direction = None
    for operation, args, kwargs in operations:
        if operation == "press_left_button":
            left_button_pressed = True
        elif operation == "press_right_button":
            right_button_pressed = True
        elif operation.startswith("d_pad_"):
            d_pad_direction = operation[6:]
        else:
            getattr(screen_draw, operation)(*args, **kwargs)

    kywy.paste(screen, (SCREEN_ORIGIN_X, SCREEN_ORIGIN_Y), mask=screen)

    if left_button_pressed:
        draw_left_button(kywy_draw, RED)

    if right_button_pressed:
        draw_right_button(kywy_draw, RED)

    if d_pad_direction is not None:
        draw_d_pad(kywy_draw, RED, d_pad_direction)

    for operation, args, kwargs in post_overlay_operations:
        getattr(kywy_draw, operation)(*args, **kwargs)

    return kywy


def kywy_screen(
    env,
    name: str,
    operations: List[Operation],
    post_overlay_operations: List[Operation] = [],
) -> str:
    img_directory = (
        Path(env.conf["site_dir"]) / Path(os.path.dirname(env.page.url)) / "img"
    )

    os.makedirs(img_directory, exist_ok=True)

    image_path = img_directory / f"{name}.png"

    if os.path.exists(image_path):
        raise Exception(f"image '{name}' already exists")

    input_hash = hashlib.md5(
        json.dumps(
            {
                "operations": operations,
                "post_overlay_operations": post_overlay_operations,
            }
        ).encode()
    ).hexdigest()
    cached_image_path = (
        ".cache/img"
        / Path(os.path.dirname(env.page.url))
        / "img"
        / f"{name}.{input_hash}.png"
    )

    if not os.path.isfile(cached_image_path):
        os.makedirs(os.path.dirname(cached_image_path), exist_ok=True)

        kywy = kywy_screen_image(operations, post_overlay_operations)
        kywy.save(cached_image_path)

    shutil.copy(cached_image_path, image_path)

    return f"""
    <p align="center">
        <img src="./img/{name}.png"/>
    </p>
    """.strip()


def kywy_screen_gif(
    env, name: str, frames: List[List[Operation]], duration: int = 100
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


if __name__ == "__main__":
    create_blank_kywy().show()
