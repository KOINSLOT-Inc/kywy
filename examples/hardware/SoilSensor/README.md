# Soil Sensor with Plant Monster Sprite

This example demonstrates how to read data from an I2C capacitive soil moisture sensor while displaying an animated plant monster sprite that reacts to the moisture levels.

## Hardware Requirements

- Kywy hardware platform
- STEMMA Soil Sensor (I2C capacitive moisture sensor)
- Connection wires

## Wiring

- **VCC** → 3.3V
- **GND** → GND  
- **SDA** → GP4
- **SCL** → GP5

## Features

### Sensor Capabilities
- Reads soil moisture using capacitive sensing technology
- Reads temperature from onboard sensor
- Displays real-time sensor data with visual indicators
- Implements robust I2C communication with error handling

### Interactive Controls
- **Right Button**: Toggle debug information display
  - Shows raw sensor reading (capacitive touch value)
  - Shows I2C device address
  - Press again to hide debug information

### Sprite Animations
The plant monster sprite has three different animation states based on sensor readings:

1. **Normal State (Happy)** - Row 1 frames (0-2)
   - Plays when moisture level is adequate (≥30%)
   - Continuous 3-frame loop animation

2. **Thirsty State** - Row 3 frames (6-8)  
   - Plays when moisture level is low (<30%)
   - Shows the plant monster looking dried out and sad

3. **Watering State (Excited)** - Row 2 frames (3-5)
   - Triggered when a sudden moisture increase is detected (>100 raw units)
   - Plays for exactly 2 complete loops then returns to normal/thirsty state
   - Shows the plant monster excited about being watered

### Display Layout
- **Top**: Moisture level bar (health bar style) with percentage on the right
- **Center**: Animated 64x64 pixel plant monster sprite
- **Middle**: Plant state description ("Happy", "Thirsty", "Drinking!")
- **Debug mode** (when right button pressed): Raw sensor value and I2C address
- **Bottom corners**: Temperature (left) and runtime (right)

## Educational Value

This example teaches:
- **I2C Communication**: Register-based sensor protocols
- **Data Conversion**: Big-endian byte handling, sensor calibration
- **Sprite Animation**: Frame-based animation systems
- **State Machines**: Behavior changes based on sensor input
- **Real-time Programming**: Non-blocking animation updates
- **User Interface**: Interactive debug mode with button input
- **Display Management**: Dynamic layout adjustments

## Sprite Sheet Information

The plant monster uses a 3x4 sprite sheet (192x256 pixels total):
- Each frame is 64x64 pixels
- Frame rate: 500ms per frame (2 FPS)
- Generated from `assets/monsters/plant.bmp` using the `img_to_header.py` script

## Customization

You can modify the sprite animations by:
1. Adjusting `FRAME_DURATION` for faster/slower animation
2. Changing moisture thresholds in `updatePlantState()`
3. Modifying the watering detection sensitivity
4. Adding new animation states or frames

## Calibration

The moisture sensor readings may need calibration for your specific soil type:
- Measure readings in completely dry soil
- Measure readings in saturated soil  
- Update `DRY_THRESHOLD` and `WET_THRESHOLD` in `convertToMoisturePercentage()`

Default values work well for most potting soils but may need adjustment for different soil compositions.
