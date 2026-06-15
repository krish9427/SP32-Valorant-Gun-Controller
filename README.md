# ESP32 Valorant Gun Controller


 

A DIY, ultra-low latency hardware aim controller for Valorant (and other FPS games). It uses an ESP32 and an MPU6050 gyroscope to act as a hardware-level Bluetooth mouse, bypassing software anti-cheat restrictions like Vanguard.

## Features
* **Hardware-Level Emulation:** Bypasses software mouse-mover blocks.
* **Auto-Calibration:** Cancels out natural gyroscope drift on startup.
* **Fractional Accumulators:** Ensures 1:1 aiming accuracy with zero decimal-dropping (left and right turning are perfectly matched).
* **Anti-Jitter Deadzone:** Filters out natural hand tremors for smooth crosshair placement.

## Hardware Required
* 1x ESP32 Development Board
* 1x MPU6050 Gyroscope/Accelerometer module
* 1x Push Button (Trigger)
* Jumper wires

## Wiring Diagram
| Component | ESP32 Pin |
| :--- | :--- |
| **Trigger Button** | Pin 4 -> Button -> GND |
| **MPU6050 SDA** | Pin 21 |
| **MPU6050 SCL** | Pin 22 |
| **MPU6050 VCC** | 3.3V (or 5V/VIN depending on module) |
| **MPU6050 GND** | GND |

## How to Install & Use
1. Install the `ESP32-BLE-Mouse` library in your Arduino IDE.
2. Upload the `.ino` sketch to your ESP32.
3. **CRITICAL:** When powering on or resetting the ESP32, keep the device **perfectly still on your desk for 2 seconds**. It needs this time to calibrate the drift.
4. Connect to your PC via Bluetooth (Look for the device named **ValoGun**).
5. Load into the Valorant Practice Range and test your aim!



https://github.com/user-attachments/assets/c87e98a0-c447-4042-89cb-1d2fb04f27d4




