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
|Item                    |Qty|Supplier  |Cost/item|Total Cost|link                                                |
|------------------------|---|----------|---------|----------|----------------------------------------------------|
|ESP32-CAM with ov3660   |1  |Amazon    |$10.50   |$10.50    |https://www.amazon.com/dp/B07RXPHYNM                |
|Arduino Mega            |1  |Amazon    |$17.00   |$17.00    |https://www.amazon.com/dp/B0GTPSYSTZ                |
|3x TT motors            |1  |Amazon    |$6.00    |$6.00     |https://www.amazon.com/dp/B098JS2ZLF                |
|4 MG90 or similar servos|1  |Amazon    |$8.00    |$8.00     |https://www.amazon.com/dp/B07MLR1498                |
|L298N                   |2  |Amazon    |$7.50    |$15.00    |https://www.amazon.com/dp/B07PFC4RRB                |
|2s Lipo                 |1  |Aliexpress|$4.66    |$4.66     |https://www.aliexpress.us/item/3256809942884416.html|
|Capacitor               |1  |Aliexpress|$0.23    |$0.23     |https://www.aliexpress.us/item/3256809487562748.html|
|Estimated total shipping|1  |          |         |$0.00     |                                                    |



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


## Basic Assembly
This project features a 3d printed frame with the components either glued and screwed into the lower sides of the frame (the drive motors) or glued onto the top of the frame (the electronics and servos).

There is a 2 inch high bridge constructed of popsicle sticks that holds the 3d printed turret and mechanisims on the top of it. It is shaped like a trapezoid with an additional support on the back.
<img width="3060" height="4080" alt="1000012499" src="https://github.com/user-attachments/assets/e88d21b8-2d3c-4fe9-ad29-652bc979b330" />

