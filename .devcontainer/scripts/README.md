# Kywy Development Scripts

This directory contains helpful scripts for Kywy development.

## img_to_header.py

A comprehensive image-to-header converter that supports multiple formats and advanced features.

### Features

- **Multi-format support**: PNG, BMP, and other PIL-supported formats
- **Automatic thresholding**: Smart conversion to 1-bit black/white with multiple algorithms
- **Custom thresholding**: User-defined threshold values (0-255)
- **Sprite sheet support**: Extract multiple animation frames from a single image
- **Flexible resizing**: Optional resizing with aspect ratio preservation
- **Educational output**: Clean C headers with usage examples and comments

### Usage Examples

```bash
# Convert single image with automatic thresholding
python3 img_to_header.py logo.png logo.h

# Convert with custom threshold
python3 img_to_header.py image.bmp output.h --threshold 128

# Create sprite sheet (3 columns, 4 rows like monster sprites)
python3 img_to_header.py monster.bmp monster_sprites.h --sprite-cols 3 --sprite-rows 4

# Convert without resizing (preserve original dimensions)
python3 img_to_header.py sprite.png sprite.h --no-resize

# Preview the conversion before generating header
python3 img_to_header.py image.png output.h --preview
```

### Sprite Sheet Support

Perfect for game development! The script can extract individual frames from sprite sheets:

- Specify grid dimensions with `--sprite-cols` and `--sprite-rows`
- Generates separate arrays for each frame
- Includes helpful macros for frame dimensions and counts
- Provides usage examples for animation loops

### Generated Output

The script generates clean, well-documented C header files with:

- SPDX license headers
- Detailed comments explaining usage
- Frame dimension macros
- Educational code examples
- Properly formatted byte arrays

### Requirements

```bash
pip install Pillow
```

## Example: Monster Sprites

The Kywy project includes monster sprite sheets in `assets/monsters/`. These are 192x256 pixel BMP files arranged as 3x4 grids (3 columns, 4 rows) with 64x64 pixel frames.

Convert them like this:
```bash
python3 img_to_header.py ../../assets/monsters/plant.bmp plant_monster.h plant_monster --sprite-cols 3 --sprite-rows 4 --no-resize
```

This creates individual arrays for each of the 12 frames, perfect for character animations in games.
