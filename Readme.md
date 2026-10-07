# RP2040 Dual Joystick USB HID Keyboard

A lightweight embedded C/C++ project for the Raspberry Pi Pico (RP2040) that presents the board as a native USB HID keyboard. It translates joystick movement into arrow key presses and a physical button into a `SPACE` keypress.

## Project Repository

- GitHub: https://github.com/charan9390/joystick

## Overview

This project is designed for USB keyboard input control using two analog joysticks and a push button. It is useful for custom controllers, game input emulation, or keyboard-based interfaces driven by a Raspberry Pi Pico.

## Features

- Plug-and-play USB HID support on Windows, Linux, and macOS
- Dual analog joystick input using RP2040 ADC channels
- Deadzone threshold logic to avoid jitter and noise
- Support for diagonal movement using combined key states
- Edge-triggered key event handling to reduce USB traffic
- Space button mapped to a keyboard key press

## Hardware Requirements

- Raspberry Pi Pico (RP2040)
- 2x analog joystick modules or one dual-axis joystick
- 1x push button or tactile switch
- Breadboard and jumper wires
- USB-A to Micro-USB cable

## Wiring and Pinout

Connect the hardware to the Pico as follows:

| Component | Signal | Pico GPIO | Pico Pin | Function |
| --- | --- | --- | --- | --- |
| Joystick 1 horizontal | VRx / VRy | GPIO 26 | 31 | ADC0 axis |
| Joystick 2 vertical | VRx / VRy | GPIO 27 | 32 | ADC1 axis |
| Space switch | SW / button | GPIO 10 | 14 | Digital input with pull-up |
| Power | VCC | 3.3V OUT | 36 | Supply |
| Ground | GND | GND | 13 / 18 / 38 | Common ground |

> The space button is wired in active-low mode. One side of the button connects to GPIO 10, and the other side connects to ground.

## Repository Structure
.
├── CMakeLists.txt        # Pico SDK build configuration
├── main.c                # ADC sampling and HID keyboard logic
├── tusb_config.h         # TinyUSB configuration
├── usb_descriptors.c     # USB HID descriptors and strings
├── Readme.md             # Project documentation
└── build/                # Generated build output

## Build Instructions

### Prerequisites

1. Install the Raspberry Pi Pico SDK
2. Install CMake
3. Install the ARM GNU toolchain (`arm-none-eabi-gcc`)

### Steps
git clone https://github.com/charan9390/joystick.git
cd joystick
mkdir build
cd build
cmake ..
make -j4

### Flashing to the Pico

1. Hold the BOOTSEL button on the Pico.
2. Connect the Pico to your computer with a micro-USB cable.
3. Copy the generated `.uf2` file to the mounted `RPI-RP2` drive.

## How It Works

1. The Pico starts up and initializes the USB HID stack and ADC inputs.
2. The joystick positions are read continuously from ADC channels.
3. Each reading is compared against threshold values to determine direction.
4. The program detects changes in state and sends keyboard reports only when needed.
5. Pressed keys are released automatically when the joystick returns to neutral.

## Input Mapping

- Left: `LEFT` arrow
- Right: `RIGHT` arrow
- Up: `UP` arrow
- Down: `DOWN` arrow
- Space button: `SPACE`

### Threshold Behavior

- ADC value below `1500`: movement toward the negative direction
- ADC value above `2500`: movement toward the positive direction
- ADC value between `1500` and `2500`: neutral deadzone

## Notes

- The code is designed for low-latency keyboard input.
- Reports are sent only on state changes to avoid key chatter and unnecessary USB traffic.
- This project is intended for embedded and hardware experimentation with the RP2040.

## License

This project is provided for educational and personal use. Add a license file if you want to publish it more formally for external use.
