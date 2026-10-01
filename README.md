# ESP32 Turret Tank

A custom remote-controlled tank built around an ESP32-CAM and an Arduino Mega. It includes dual tracked movement, a turret with pan/tilt control, a disc launcher, a live camera stream, and a hosted web interface for control.
<img width="3060" height="4080" alt="1000012497" src="https://github.com/user-attachments/assets/eba9e319-1267-4b9e-b6bb-d58a306b422c" />

## Overview

This project combines two microcontrollers:

- ESP32-CAM: hosts the web UI, reads camera feed, controls the pan/tilt servos, and communicates with the motor controller via 1-wire UART.
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

## Hardware Used

- ESP32-CAM module
- Arduino Mega
- 3 of the yellow TT motors you can find all over Amazon (2 for driving, one for disk luncher)
- L2398N x2
- 2 servos for pan/tilt turret movement
- Camera module integrated with ESP32-CAM
- Disc launcher mechanism driven by a motor or actuator
- 7.4v LiPo, charger, and 5v regulator/buck converter
## Wiring Notes

The ESP32-CAM file defines the following key signals:

- `PAN_PIN = 12`
- `TILT_PIN = 13`
- `FLASH_LED = 4`

The Mega sketch uses:

- `ENA` / `IN1` / `IN2` for the right-side track
- `ENB` / `IN3` / `IN4` for the left-side track
- `LAUNCH_IN1` / `LAUNCH_IN2` for the launcher
<img width="3060" height="4080" alt="1000012504" src="https://github.com/user-attachments/assets/81057490-f833-462b-bfd2-5f0e19b8b4f9" />

The ESP32 communicates with the Mega over Serial2 at 115200 baud using commands shaped like:

- `touno:left,right`
- `touno:FIRE_ON`
- `touno:FIRE_OFF`

## Required Libraries

For the ESP32-CAM firmware, install the following in the Arduino IDE:

- ESP32 board support library
- `ESP32Servo`
- `Preferences`
- Camera support for the ESP32-CAM board (comes with esp32 board library)

For the Mega firmware, the standard Arduino core is sufficient.

## Setup

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

The page lets you control:

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
- Use the flashlight toggle to illuminate the area with the IO4 built-in ESP32 camera flash led
- Recenter the camera with the dedicated button

<img width="3060" height="4080" alt="1000012499" src="https://github.com/user-attachments/assets/e88d21b8-2d3c-4fe9-ad29-652bc979b330" />

This project is shared for personal and educational use. No explicit license file is included in the repository, so use it at your own discretion and respect the hardware and code ownership of the original author.
