# Rove Control Board Firmware

## Introduction
This repository contains the firmware for the Rove Control Board, a key component of Rove's hardware suite. The board controls various elements such as individually addressable LED strips, fog lights, servo motors (for camera movement), a yellow indicator LED, and motor control for a winch. Development is ongoing with testing on bluepill mcu, aiming for final deployment on a STM32 microcontroller.

## Features
- **Individually Addressable LED Strip**: Control color and patterns using the NeoPixel library.
- **Fog Lights**: Manage two external fog lights with simple on/off controls.
- **Servo Motors**: Adjust camera angles with precision.
- **Indicator LED**: Use for status signaling.
- **Winch Control**: Operate a motorized winch.

## Development Setup
This project is built using PlatformIO, an ecosystem compatible with Arduino and STM32. Follow these steps:
1. Install PlatformIO Core or IDE.
2. Clone this repository.
3. Open the project in PlatformIO IDE.
4. Configure `platformio.ini` for your hardware.
5. Install required libraries, including Adafruit NeoPixel.
6. Change the environment to env:due and build (this will download the capra_micro_comm library). The build **will fail**. This is expected behavior.
7. Change the environment back to env:nucleo_f446re and build. This **will also fail**.
8. Locate the `.pio` folder, copy `.pio/libdeps/due/capra_micro_comm` and paste it in `.pio/libdeps/nucleo_f446re/`. This is a workaround to get the capra_micro_comm library for the nucleo. It may be due to the stm32cube framework.

## Building and Uploading
- **Build**: Run `platformio run` to compile.
- **Upload**: Connect your board and run `platformio run --target upload`.

## Testing
The current firmware is tested on an STM32 [more specifically, a bluepill mcu, therefore a generic f103c8]. Ensure all components are correctly connected to your desired pins if you want to test this specific code.

## Components
1. `LEDStrip`: Manages the NeoPixel LED strip.
2. `FogLight`: Controls on/off state of fog lights.
3. `main.cpp`: Main program entry, handling initialization and serial commands.

## Usage
- Modify `main.cpp` to handle custom serial commands for device control.
- Use `LEDStrip` and `FogLight` classes for managing respective hardware components.

---

Made by CAPRA |  Date: 2024-03-02