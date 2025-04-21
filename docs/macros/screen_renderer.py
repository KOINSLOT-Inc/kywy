# SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
#
# SPDX-License-Identifier: GPL-3.0-or-later

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
    draw.circle(
        [
            0 + CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH + CHAMFER_WIDTH,
            FULL_HEIGHT - BUTTON_HEIGHT_FROM_BOTTOM,
        ],
        BUTTON_RADIUS,
        fill=BLACK,
    )
    draw.circle(
        [
            FULL_WIDTH - (CHAMFER_WIDTH + TOP_LEFT_RIGHT_BORDER_WIDTH + CHAMFER_WIDTH),
            FULL_HEIGHT - BUTTON_HEIGHT_FROM_BOTTOM,
        ],
        BUTTON_RADIUS,
        fill=BLACK,
    )
    draw.circle(
        [int(FULL_WIDTH / 2), FULL_HEIGHT - D_PAD_HEIGHT_FROM_BOTTOM],
        BUTTON_RADIUS,
        fill=GREY,
    )
    draw.circle(
        [int(FULL_WIDTH / 2), FULL_HEIGHT - D_PAD_HEIGHT_FROM_BOTTOM],
        BUTTON_RADIUS - 2,
        fill=BLACK,
    )

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


def draw_circle(image: Image, xy, *args, **kwargs):
    draw = ImageDraw.Draw(image)

    shifted_xy = (SCREEN_ORIGIN_X + xy[0], SCREEN_ORIGIN_Y + xy[1])
    draw.circle(shifted_xy, *args, **kwargs)

    return image


if __name__ == "__main__":
    create_blank_kywy().show()
