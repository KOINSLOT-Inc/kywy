# SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

import sys
from pathlib import Path

sys.path.append(str(Path(__file__).parent.parent))

from screen_renderer import (
    create_blank_kywy,
    SCREEN_ORIGIN_X,
    SCREEN_ORIGIN_Y,
    BLACK,
)
from PIL import ImageDraw

GRID_SIZE = 12
SEGMENT_SIZE = 10


def shift(x, y):
    return [SCREEN_ORIGIN_X + x, SCREEN_ORIGIN_Y + y]


def draw_body_segment(draw, i, j):
    x = GRID_SIZE * i
    y = GRID_SIZE * j
    draw.rectangle(
        [shift(x + 2, y + 2), shift(x + SEGMENT_SIZE, y + SEGMENT_SIZE)], fill=BLACK
    )


append_images = []
snake = [
    [0, 0],
    [1, 0],
    [2, 0],
    [3, 0],
]

horizontal = 1
vertical = 0
while True:
    kywy = create_blank_kywy()
    draw = ImageDraw.Draw(kywy)

    for segment in snake:
        draw_body_segment(draw, segment[0], segment[1])

    if snake[-1][0] == 11 and snake[-1][1] == 0:
        horizontal = 0
        vertical = 1

    if snake[-1][0] == 11 and snake[-1][1] == 13:
        horizontal = -1
        vertical = 0

    if snake[-1][0] == 0 and snake[-1][1] == 13:
        horizontal = 0
        vertical = -1

    if snake[-1][0] == 0 and snake[-1][1] == 0:
        horizontal = 1
        vertical = 0

    snake[0] = snake[1]
    snake[1] = snake[2]
    snake[2] = snake[3]

    snake[3] = [snake[3][0] + horizontal, snake[3][1] + vertical]

    append_images.append(kywy)

    if snake[0] == [0, 0]:
        break

append_images[0].save(
    Path(__file__).parent / "snake.gif",
    append_images=append_images[1:],
    save_all=True,
    loop=0,
    duration=100,
)
