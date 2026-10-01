# ESP32 Turret Tank

A custom remote-controlled tank built around an ESP32-CAM and an Arduino Mega. It includes dual tracked movement, a turret with pan/tilt control, a disc launcher, a live camera stream, and a hosted web interface for control.

## Overview

This project combines two microcontrollers:

- ESP32-CAM: hosts the web UI, reads camera feed, controls the pan/tilt servos, and communicates with the motor controller.
- Arduino Mega: drives the left and right tank tracks and activates the disc launcher.

The result is a small, camera-equipped turret tank that can be controlled over Wi-Fi from a browser.

## Features

- Two tracked drive motors with variable speed control
- Pan and tilt camera movement
- Live camera stream on a hosted webpage
- Launch button for a disc-shooting turret mechanism
- Flashlight toggle for the ESP32-CAM
- Saved pan/tilt servo positions using Preferences storage
- Browser-based control panel with sliders and buttons

## Repository Files

- `duotank-esp32cam.ino` — ESP32-CAM firmware, web server, camera stream, and servo controls
- `duotank-mega.ino` — Mega firmware for track drive and disc launcher logic
- `README.md` — project overview and setup instructions

## Hardware Used

- ESP32-CAM module
- Arduino Mega
- 2 DC motors or tracked drive motors
- Motor driver board (the project uses direct pins for motor control and PWM)
- 2 servos for pan/tilt turret movement
- Camera module integrated with ESP32-CAM
- Disc launcher mechanism driven by a motor or actuator
- Power supply suitable for both the ESP32 and the motors

## Wiring Notes

The ESP32-CAM file defines the following key signals:

- `PAN_PIN = 12`
- `TILT_PIN = 13`
- `FLASH_LED = 4`

The Mega sketch uses:

- `ENA` / `IN1` / `IN2` for the right-side track
- `ENB` / `IN3` / `IN4` for the left-side track
- `LAUNCH_IN1` / `LAUNCH_IN2` for the launcher

The ESP32 communicates with the Mega over Serial2 at 115200 baud using commands shaped like:

- `touno:left,right`
- `touno:FIRE_ON`
- `touno:FIRE_OFF`

## Required Libraries

For the ESP32-CAM firmware, install the following in the Arduino IDE:

- ESP32 board support
- `ESP32Servo`
- `Preferences`
- Camera support for the ESP32-CAM board

For the Mega firmware, the standard Arduino core is sufficient.

## Setup Instructions

### 1. Update Wi-Fi credentials

In `duotank-esp32cam.ino`, edit:

```cpp
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
```

This is currently hardcoded and must be changed before use.

### 2. Upload code to the boards

- Upload `duotank-esp32cam.ino` to the ESP32-CAM
- Upload `duotank-mega.ino` to the Arduino Mega

### 3. Power everything on

After the ESP32 connects to Wi-Fi, it prints the assigned IP address over Serial.

### 4. Open the control page

Open the ESP32 address in a browser, typically:

```text
http://<ESP32_IP_ADDRESS>
```

The page provides:

- track speed sliders
- camera pan slider
- camera tilt slider
- launch button
- flashlight toggle
- recenter camera button

## How the Control Page Works

The web page sends commands to the ESP32 server via HTTP endpoints:

- `/control` — updates track, pan, and tilt values
- `/launch` — turns the launcher on or off
- `/flash` — toggles the flashlight
- `/getangles` — returns saved pan/tilt values

The ESP32 then sends serial messages to the Mega to drive the tracks and launcher.

## Example Operation

- Move the left/right track sliders to drive the tank
- Use the pan and tilt sliders to aim the turret
- Press and hold the launch button to fire
- Use the flashlight toggle to illuminate the area
- Recenter the camera with the dedicated button

## Notes and Safety

- The project currently includes a Wi-Fi SSID/password directly in source code. Replace this before deploying outside a private test setup.
- Motor and launcher power requirements should be matched to the hardware used.
- Check servo travel ranges and mechanical fit before extended operation.
- Use a proper battery or regulated power source for the motors; ESP32 and Mega power rails should be managed carefully.

## License

This project is shared for personal and educational use. No explicit license file is included in the repository, so use it at your own discretion and respect the hardware and code ownership of the original author.

## Project Status

This repository is a working proof-of-concept for a Wi-Fi controlled turret tank with camera streaming and disc-launch functionality. It is intended as a hobby project and can be expanded with improved controls, better driver logic, or a more robust mechanical design.

## Quick Summary

The tank is controlled from a browser over Wi-Fi, the ESP32-CAM handles video and turret control, and the Mega manages locomotion and firing. It is a compact and fun build for experimenting with robotics, embedded control, and web interfaces.

## Original Code Intent

The code demonstrates a practical approach to:

- controlling multiple motors from a microcontroller
- streaming a live camera over Wi-Fi
- exposing a simple browser UI
- communicating between ESP32 and Mega over serial
- saving camera orientation settings to persistent memory

This repository is a good starting point for a custom RC tank or turret project.

---

If you want, I can also generate a more polished version tailored specifically for GitHub, including a project gallery, wiring diagram notes, and a cleaner hardware bill of materials.
