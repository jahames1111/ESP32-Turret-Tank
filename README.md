### ESP32 Turret Tank
## Overview
This is a tank that I built in my free time that has a live camera view, full 360 degree turn steering, and a turret that shoots small disks.
<img width="3060" height="4080" alt="1000012497" src="https://github.com/user-attachments/assets/eba9e319-1267-4b9e-b6bb-d58a306b422c" />

## Features

- Two motors on both sides with variable speed control
- Pan and tilt camera movement
- Live camera stream on a hosted webpage
- Launch button for a disc-shooting auto-reloading turret built with rubber bands
- Built-in flashlight on the ESP32-CAM
- Saved pan/tilt servo positions using Preferences

## Hardware Used
<img width="859" height="446" alt="image" src="https://github.com/user-attachments/assets/e114cb6c-3f39-4d60-ba17-78d171886373" />


## Wiring
<img width="960" height="720" alt="Untitled drawing (5)" src="https://github.com/user-attachments/assets/baddd7a0-a656-4893-8582-c1d19ade5809" />



The esp32 hosts a page that lets you control:

- track speed sliders
- camera pan slider
- camera tilt slider
- launch button
- flashlight toggle
- recenter camera button


The ESP32 then sends serial messages to the Mega to drive the tracks and launcher.

<img width="3060" height="4080" alt="1000012499" src="https://github.com/user-attachments/assets/e88d21b8-2d3c-4fe9-ad29-652bc979b330" />

