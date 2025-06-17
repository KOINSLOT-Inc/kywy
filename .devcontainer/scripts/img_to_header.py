#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2025 KOINSLOT, Inc.
# SPDX-License-Identifier: GPL-3.0-or-later

"""
Image to Kywy Header File Converter

This script converts PNG and BMP images to C header files compatible with the Kywy
graphics library. It supports single images and sprite sheets with customizable
thresholding, resizing, and output formatting.

Usage:
    # Convert single image
    python img_to_header.py input.png output.h [array_name]
    pExamples:
  # Convert splash.png to splash.h with default array name
  python img_to_header.py splash.png splash.h

  # Convert with custom array name and threshold
  python img_to_header.py monster.bmp monster_sprite.h myMonster --threshold 128

  # Convert with custom dimensions (no resizing)
  python img_to_header.py image.png output.h --width 100 --height 50 --no-resize

  # Extract sprite sheet with 4 columns, 2 rows
  python img_to_header.py spritesheet.png sprites.h --sprite-cols 4 --sprite-rows 2

  # Preview the converted image before generating header
  python img_to_header.py image.bmp output.h --previewader.py input.bmp output.h [array_name]

    # Convert with custom threshold
    python img_to_header.py input.png output.h --threshold 128

    # Create sprite sheet (4 frames horizontally, 2 vertically)
    python img_to_header.py spritesheet.png sprites.h --sprite-cols 4 --sprite-rows 2

Requirements:
    pip install Pillow numpy

Features:
- Converts PNG and BMP images to 1-bit black/white bitmaps
- Smart automatic threshold detection using Otsu's method
- Customizable manual threshold for black/white conversion (0-255)
- Automatic or manual resizing with aspect ratio preservation
- Sprite sheet support - extract multiple frames from one image
- Generates clean C header files with proper formatting
- Educational comments and usage examples in output

The script will:
1. Load the PNG or BMP image
2. Convert to grayscale if needed
3. Apply thresholding for 1-bit conversion
4. Resize to target dimensions (optional)
5. Extract sprite frames (if sprite sheet mode)
6. Pack pixels into bytes (8 pixels per byte)
7. Generate C array(s) in header file format

Author: Generated for Kywy development
License: GPL-3.0-or-later
"""

import sys
import os
from PIL import Image
import argparse
from datetime import datetime
import numpy as np

# Kywy display dimensions
KYWY_WIDTH = 144
KYWY_HEIGHT = 168


def calculate_optimal_threshold(img):
    """
    Calculate optimal threshold for 1-bit conversion using Otsu's method.

    Args:
        img: PIL Image in grayscale mode

    Returns:
        Optimal threshold value (0-255)
    """
    # Convert to numpy array for histogram analysis
    img_array = np.array(img)

    # Calculate histogram
    hist, bins = np.histogram(img_array.flatten(), bins=256, range=(0, 256))

    # Normalize histogram
    hist = hist.astype(float)
    hist /= hist.sum()

    # Find optimal threshold using Otsu's method
    bins = bins[:-1]  # Remove last bin edge

    # Initialize variables
    max_variance = 0
    optimal_threshold = 128  # Default fallback

    # Try all possible thresholds
    for t in range(1, 255):
        # Split into background and foreground
        w0 = hist[:t].sum()  # Background weight
        w1 = hist[t:].sum()  # Foreground weight

        if w0 == 0 or w1 == 0:
            continue

        # Calculate means
        mu0 = (hist[:t] * bins[:t]).sum() / w0  # Background mean
        mu1 = (hist[t:] * bins[t:]).sum() / w1  # Foreground mean

        # Calculate between-class variance
        variance_between = w0 * w1 * (mu0 - mu1) ** 2

        if variance_between > max_variance:
            max_variance = variance_between
            optimal_threshold = t

    return optimal_threshold


def load_and_prepare_image(
    input_path,
    target_width=KYWY_WIDTH,
    target_height=KYWY_HEIGHT,
    threshold=None,
    resize=True,
):
    """
    Load PNG or BMP image and prepare it for conversion to Kywy format.

    Args:
        input_path: Path to input PNG or BMP file
        target_width: Target width in pixels (default: 144)
        target_height: Target height in pixels (default: 168)
        threshold: Custom threshold for 1-bit conversion (0-255), None for auto
        resize: Whether to resize image to target dimensions

    Returns:
        PIL Image object in 1-bit black/white format
    """
    try:
        # Load the image
        img = Image.open(input_path)
        print(f"Loaded image: {img.size[0]}x{img.size[1]} pixels, mode: {img.mode}")

        # Convert to grayscale if needed
        if img.mode not in ["L", "1"]:
            print("Converting to grayscale...")
            img = img.convert("L")

        # Resize if needed and requested
        if resize and img.size != (target_width, target_height):
            print(
                f"Resizing from {img.size[0]}x{img.size[1]} to {target_width}x{target_height}..."
            )

            # Calculate aspect ratios
            img_ratio = img.size[0] / img.size[1]
            target_ratio = target_width / target_height

            if img_ratio > target_ratio:
                # Image is wider - fit to width
                new_width = target_width
                new_height = int(target_width / img_ratio)
            else:
                # Image is taller - fit to height
                new_height = target_height
                new_width = int(target_height * img_ratio)

            # Resize maintaining aspect ratio
            img = img.resize((new_width, new_height), Image.Resampling.LANCZOS)

            # Create new image with target size and paste resized image centered
            final_img = Image.new(
                "L", (target_width, target_height), 255
            )  # White background
            paste_x = (target_width - new_width) // 2
            paste_y = (target_height - new_height) // 2
            final_img.paste(img, (paste_x, paste_y))
            img = final_img

        # Convert to 1-bit black and white with custom or automatic threshold
        if threshold is not None:
            print(
                f"Converting to 1-bit black/white with custom threshold {threshold}..."
            )
            # Apply custom threshold
            img = img.point(lambda p: 255 if p > threshold else 0, mode="1")
        else:
            print("Converting to 1-bit black/white with automatic threshold...")
            # Calculate optimal threshold using Otsu's method
            optimal_threshold = calculate_optimal_threshold(img)
            print(f"Calculated optimal threshold: {optimal_threshold}")
            img = img.point(lambda p: 255 if p > optimal_threshold else 0, mode="1")

        return img

    except Exception as e:
        print(f"Error loading image: {e}")
        sys.exit(1)


def extract_sprite_frames(img, cols, rows):
    """
    Extract individual sprite frames from a sprite sheet image.

    Args:
        img: PIL Image containing the sprite sheet
        cols: Number of columns (frames horizontally)
        rows: Number of rows (frames vertically)

    Returns:
        List of PIL Image objects, one for each frame
    """
    width, height = img.size
    frame_width = width // cols
    frame_height = height // rows

    print(
        f"Extracting {cols}x{rows} sprite frames ({frame_width}x{frame_height} each)..."
    )

    frames = []
    for row in range(rows):
        for col in range(cols):
            left = col * frame_width
            top = row * frame_height
            right = left + frame_width
            bottom = top + frame_height

            frame = img.crop((left, top, right, bottom))
            frames.append(frame)

    print(f"Extracted {len(frames)} frames")
    return frames


def image_to_byte_array(img):
    """
    Convert 1-bit PIL Image to byte array for Kywy display.

    The Kywy display uses a packed bitmap format where:
    - Each byte represents 8 horizontal pixels
    - Bit 7 (MSB) is the leftmost pixel
    - 0 = black pixel, 1 = white pixel

    Args:
        img: PIL Image in 1-bit mode

    Returns:
        List of integers representing the byte array
    """
    width, height = img.size
    bytes_per_row = (width + 7) // 8  # Round up to nearest byte
    byte_array = []

    print(f"Converting {width}x{height} image to byte array...")
    print(f"Bytes per row: {bytes_per_row}")

    for y in range(height):
        for byte_col in range(bytes_per_row):
            byte_value = 0

            # Pack 8 pixels into one byte
            for bit in range(8):
                x = byte_col * 8 + bit

                if x < width:
                    # Get pixel value (0 = black, 255 = white in 1-bit mode)
                    pixel = img.getpixel((x, y))

                    # Convert to bit (0 = black, 1 = white)
                    # Note: PIL returns 0 for black, 255 for white in 1-bit mode
                    bit_value = 1 if pixel > 0 else 0

                    # Set bit in byte (MSB is leftmost pixel)
                    byte_value |= bit_value << (7 - bit)

            byte_array.append(byte_value)

    print(f"Generated {len(byte_array)} bytes")
    return byte_array


def generate_header_file(
    byte_arrays,
    output_path,
    array_names,
    widths,
    heights,
    input_filename,
    is_sprite_sheet=False,
    sprite_cols=1,
    sprite_rows=1,
):
    """
    Generate C header file with the byte array(s).

    Args:
        byte_arrays: List of byte value lists (one per frame/image)
        output_path: Output file path
        array_names: List of names for the C arrays
        widths: List of image widths
        heights: List of image heights
        input_filename: Original input filename for documentation
        is_sprite_sheet: Whether this is a sprite sheet with multiple frames
        sprite_cols: Number of sprite columns (for documentation)
        sprite_rows: Number of sprite rows (for documentation)
    """
    try:
        with open(output_path, "w") as f:
            # Write header
            f.write(
                f"// SPDX-FileCopyrightText: {datetime.now().year} KOINSLOT, Inc.\n"
            )
            f.write("// SPDX-License-Identifier: GPL-3.0-or-later\n\n")
            f.write(f"/*\n")
            f.write(f" * Auto-generated header file from {input_filename}\n")

            if is_sprite_sheet:
                f.write(f" * Sprite sheet: {sprite_cols}x{sprite_rows} frames\n")
                f.write(f" * Frame size: {widths[0]}x{heights[0]} pixels\n")
                f.write(f" * Total frames: {len(byte_arrays)}\n")
            else:
                f.write(f" * Image size: {widths[0]}x{heights[0]} pixels\n")

            f.write(f" * Format: 1-bit packed bitmap (0=black, 1=white)\n")
            f.write(f" * Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n")
            f.write(f" *\n")
            f.write(f" * Usage in Kywy projects:\n")

            if is_sprite_sheet:
                base_name = array_names[0].replace("_frame_0", "")
                f.write(f' *   #include "{os.path.basename(output_path)}"\n')
                f.write(f" *   \n")
                f.write(f" *   // Draw individual frames:\n")
                f.write(
                    f" *   engine.display.drawBitmap(x, y, {base_name.upper()}_FRAME_WIDTH, \n"
                )
                f.write(
                    f" *                            {base_name.upper()}_FRAME_HEIGHT, {array_names[0]});\n"
                )
                f.write(f" *   \n")
                f.write(f" *   // Animate through frames:\n")
                f.write(
                    f" *   const uint8_t* frames[] = {{{', '.join(array_names)}}};\n"
                )
                f.write(
                    f" *   engine.display.drawBitmap(x, y, {base_name.upper()}_FRAME_WIDTH,\n"
                )
                f.write(
                    f" *                            {base_name.upper()}_FRAME_HEIGHT, frames[currentFrame]);\n"
                )
            else:
                f.write(f' *   #include "{os.path.basename(output_path)}"\n')
                f.write(
                    f" *   engine.display.drawBitmap(x, y, {array_names[0].upper()}_WIDTH, \n"
                )
                f.write(
                    f" *                            {array_names[0].upper()}_HEIGHT, {array_names[0]});\n"
                )

            f.write(f" */\n\n")

            # Generate include guard
            guard_name = (
                os.path.splitext(os.path.basename(output_path))[0].upper() + "_H"
            )
            f.write(f"#ifndef {guard_name}\n")
            f.write(f"#define {guard_name}\n\n")
            f.write(f"#include <stdint.h>\n\n")

            # Write dimensions
            if is_sprite_sheet:
                base_name = array_names[0].replace("_frame_0", "")
                f.write(f"// Sprite sheet information\n")
                f.write(f"#define {base_name.upper()}_FRAME_WIDTH {widths[0]}\n")
                f.write(f"#define {base_name.upper()}_FRAME_HEIGHT {heights[0]}\n")
                f.write(f"#define {base_name.upper()}_FRAME_COUNT {len(byte_arrays)}\n")
                f.write(f"#define {base_name.upper()}_SPRITE_COLS {sprite_cols}\n")
                f.write(f"#define {base_name.upper()}_SPRITE_ROWS {sprite_rows}\n\n")
            else:
                f.write(f"// Image dimensions\n")
                f.write(f"#define {array_names[0].upper()}_WIDTH {widths[0]}\n")
                f.write(f"#define {array_names[0].upper()}_HEIGHT {heights[0]}\n\n")

            # Write arrays
            for i, (byte_array, array_name, width, height) in enumerate(
                zip(byte_arrays, array_names, widths, heights)
            ):
                if is_sprite_sheet:
                    f.write(f"// Frame {i} bitmap data ({len(byte_array)} bytes)\n")
                else:
                    f.write(f"// Bitmap data ({len(byte_array)} bytes)\n")

                f.write(f"const uint8_t {array_name}[] = {{\n")

                # Write byte array with proper formatting
                bytes_per_line = 12  # Same as original 2DTennis format
                for j, byte_val in enumerate(byte_array):
                    if j % bytes_per_line == 0:
                        f.write("  ")

                    f.write(f"0x{byte_val:02x}")

                    if j < len(byte_array) - 1:
                        f.write(", ")

                    if (j + 1) % bytes_per_line == 0 or j == len(byte_array) - 1:
                        f.write("\n")

                f.write("};\n\n")

            f.write(f"#endif // {guard_name}\n")

        print(f"Header file generated: {output_path}")
        if is_sprite_sheet:
            print(f"Sprite frames: {len(byte_arrays)}")
            print(f"Frame size: {widths[0]}x{heights[0]} pixels")
        else:
            print(f"Array name: {array_names[0]}")
            print(f"Image size: {widths[0]}x{heights[0]} pixels")

        total_bytes = sum(len(arr) for arr in byte_arrays)
        print(f"Total size: {total_bytes} bytes")

    except Exception as e:
        print(f"Error writing header file: {e}")
        sys.exit(1)


def format_header_file(output_path):
    """
    Apply clang-format to the generated header file if clang-format is available.
    
    Args:
        output_path: Path to the header file to format
    """
    import subprocess
    import shutil
    
    # Check if clang-format is available
    clang_format_cmd = None
    for cmd in ['clang-format', 'clang-format-14', 'clang-format-15', 'clang-format-16']:
        if shutil.which(cmd):
            clang_format_cmd = cmd
            break
    
    if clang_format_cmd is None:
        print("clang-format not found - skipping code formatting")
        return False
    
    try:
        print(f"Formatting header file with {clang_format_cmd}...")
        
        # Run clang-format in-place on the generated file
        result = subprocess.run(
            [clang_format_cmd, '-i', output_path],
            capture_output=True,
            text=True,
            timeout=30
        )
        
        if result.returncode == 0:
            print("Header file formatted successfully")
            return True
        else:
            print(f"clang-format warning: {result.stderr}")
            return False
            
    except subprocess.TimeoutExpired:
        print("clang-format timed out - skipping formatting")
        return False
    except Exception as e:
        print(f"Error running clang-format: {e}")
        return False


def main():
    """Main function with command line argument parsing."""
    parser = argparse.ArgumentParser(
        description="Convert PNG or BMP image to Kywy-compatible C header file",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  # Convert splash.png to splash.h with default array name
  python png_to_header.py splash.png splash.h
  
  # Convert BMP image with custom array name and threshold
  python png_to_header.py monster.bmp monster_sprite.h myMonster --threshold 128
  
  # Convert with custom dimensions (no resizing)
  python png_to_header.py image.png output.h --width 100 --height 50 --no-resize
  
  # Extract sprite sheet with 4 columns, 2 rows
  python png_to_header.py spritesheet.png sprites.h --sprite-cols 4 --sprite-rows 2
  
  # Preview the converted image before generating header
  python png_to_header.py image.bmp output.h --preview

The output header file can be included in your Kywy project:
  #include "splash.h"
  engine.display.drawBitmap(0, 0, SPLASH_WIDTH, SPLASH_HEIGHT, splash);

For sprite sheets:
  #include "sprites.h"
  const uint8_t* frames[] = {sprite_frame_0, sprite_frame_1, sprite_frame_2, sprite_frame_3};
  engine.display.drawBitmap(x, y, SPRITE_FRAME_WIDTH, SPRITE_FRAME_HEIGHT, frames[currentFrame]);
        """,
    )

    parser.add_argument("input", help="Input PNG or BMP file path")
    parser.add_argument("output", help="Output header file path")
    parser.add_argument(
        "array_name",
        nargs="?",
        help="C array name (default: derived from output filename)",
    )

    # Image processing options
    parser.add_argument(
        "--width",
        type=int,
        default=KYWY_WIDTH,
        help=f"Target width in pixels (default: {KYWY_WIDTH})",
    )
    parser.add_argument(
        "--height",
        type=int,
        default=KYWY_HEIGHT,
        help=f"Target height in pixels (default: {KYWY_HEIGHT})",
    )
    parser.add_argument(
        "--threshold",
        type=int,
        metavar="0-255",
        help="Custom threshold for black/white conversion (0-255). "
        "Pixels above this value become white, below become black. "
        "Default: automatic threshold",
    )
    parser.add_argument(
        "--no-resize",
        action="store_true",
        help="Don't resize image (use original dimensions)",
    )

    # Sprite sheet options
    parser.add_argument(
        "--sprite-cols",
        type=int,
        metavar="N",
        help="Number of sprite columns (creates sprite sheet)",
    )
    parser.add_argument(
        "--sprite-rows",
        type=int,
        metavar="N",
        help="Number of sprite rows (creates sprite sheet)",
    )

    # Preview option
    parser.add_argument(
        "--preview", action="store_true", help="Show preview of converted image"
    )

    args = parser.parse_args()

    # Validate input file
    if not os.path.exists(args.input):
        print(f"Error: Input file '{args.input}' not found")
        sys.exit(1)

    # Check for supported file extensions
    input_lower = args.input.lower()
    if not (input_lower.endswith(".png") or input_lower.endswith(".bmp")):
        print("Warning: Input file doesn't have .png or .bmp extension")

    # Validate threshold
    if args.threshold is not None and (args.threshold < 0 or args.threshold > 255):
        print("Error: Threshold must be between 0 and 255")
        sys.exit(1)

    # Validate sprite sheet options
    is_sprite_sheet = args.sprite_cols is not None or args.sprite_rows is not None
    if is_sprite_sheet:
        if args.sprite_cols is None or args.sprite_rows is None:
            print(
                "Error: Both --sprite-cols and --sprite-rows must be specified for sprite sheets"
            )
            sys.exit(1)
        if args.sprite_cols < 1 or args.sprite_rows < 1:
            print("Error: Sprite columns and rows must be at least 1")
            sys.exit(1)

    # Generate array name if not provided
    if args.array_name:
        base_array_name = args.array_name
    else:
        # Derive from output filename
        base_name = os.path.splitext(os.path.basename(args.output))[0]
        # Convert to valid C identifier
        base_array_name = "".join(c if c.isalnum() else "_" for c in base_name)
        if base_array_name[0].isdigit():
            base_array_name = "img_" + base_array_name

    print(f"Image to Kywy Header Converter")
    print(f"Input: {args.input}")
    print(f"Output: {args.output}")
    print(f"Base array name: {base_array_name}")

    if is_sprite_sheet:
        print(f"Sprite sheet mode: {args.sprite_cols}x{args.sprite_rows} frames")

    if args.threshold is not None:
        print(f"Custom threshold: {args.threshold}")

    if args.no_resize:
        print("No resizing (using original dimensions)")
    else:
        print(f"Target size: {args.width}x{args.height}")

    print("-" * 50)

    # Process the image
    if is_sprite_sheet:
        # Load image without resizing for sprite sheet processing
        img = load_and_prepare_image(
            args.input, args.width, args.height, args.threshold, resize=False
        )

        # Extract sprite frames
        frames = extract_sprite_frames(img, args.sprite_cols, args.sprite_rows)

        # Resize frames if needed
        if not args.no_resize:
            frame_width = (
                args.width // args.sprite_cols if args.width else frames[0].size[0]
            )
            frame_height = (
                args.height // args.sprite_rows if args.height else frames[0].size[1]
            )

            resized_frames = []
            for i, frame in enumerate(frames):
                if frame.size != (frame_width, frame_height):
                    print(
                        f"Resizing frame {i} from {frame.size[0]}x{frame.size[1]} to {frame_width}x{frame_height}"
                    )
                    frame = frame.resize(
                        (frame_width, frame_height), Image.Resampling.LANCZOS
                    )
                resized_frames.append(frame)
            frames = resized_frames

        # Show preview if requested
        if args.preview:
            print("Showing preview of first frame... (close the window to continue)")
            frames[0].show()

        # Convert frames to byte arrays
        byte_arrays = []
        array_names = []
        widths = []
        heights = []

        for i, frame in enumerate(frames):
            byte_array = image_to_byte_array(frame)
            byte_arrays.append(byte_array)
            array_names.append(f"{base_array_name}_frame_{i}")
            widths.append(frame.size[0])
            heights.append(frame.size[1])

        # Generate header file
        input_filename = os.path.basename(args.input)
        generate_header_file(
            byte_arrays,
            args.output,
            array_names,
            widths,
            heights,
            input_filename,
            is_sprite_sheet=True,
            sprite_cols=args.sprite_cols,
            sprite_rows=args.sprite_rows,
        )

        # Format the generated header file with clang-format
        format_header_file(args.output)

        print("-" * 50)
        print("Sprite sheet conversion complete!")
        print(f"\nTo use in your Kywy project:")
        print(f'  #include "{os.path.basename(args.output)}"')
        print(f'  const uint8_t* frames[] = {{{", ".join(array_names)}}};')
        print(
            f"  engine.display.drawBitmap(x, y, {base_array_name.upper()}_FRAME_WIDTH,"
        )
        print(
            f"                           {base_array_name.upper()}_FRAME_HEIGHT, frames[currentFrame]);"
        )

    else:
        # Single image mode
        img = load_and_prepare_image(
            args.input,
            args.width,
            args.height,
            args.threshold,
            resize=not args.no_resize,
        )

        # Show preview if requested
        if args.preview:
            print("Showing preview... (close the window to continue)")
            img.show()

        # Convert to byte array
        byte_array = image_to_byte_array(img)

        # Generate header file
        input_filename = os.path.basename(args.input)
        generate_header_file(
            [byte_array],
            args.output,
            [base_array_name],
            [img.size[0]],
            [img.size[1]],
            input_filename,
        )

        # Format the generated header file with clang-format
        format_header_file(args.output)

        print("-" * 50)
        print("Conversion complete!")
        print(f"\nTo use in your Kywy project:")
        print(f'  #include "{os.path.basename(args.output)}"')
        print(
            f"  engine.display.drawBitmap(x, y, {base_array_name.upper()}_WIDTH, {base_array_name.upper()}_HEIGHT, {base_array_name});"
        )


if __name__ == "__main__":
    main()
