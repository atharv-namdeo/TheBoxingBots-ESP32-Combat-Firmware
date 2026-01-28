# TheBoxingBots-ESP32-Combat-Firmware
High-performance combat firmware for TheBoxingBots. Features a fully custom control scheme, low-latency Bluepad32 gamepad integration, and pre-programmed combat macros including the signature "Ankle Breaker" and "Hip Twist" attacks.


# 🥊 TheBoxingBots: ESP32 Combat Firmware

![Status](https://img.shields.io/badge/Status-Battle%20Ready-red)
![Platform](https://img.shields.io/badge/Platform-ESP32-blue)
![Controller](https://img.shields.io/badge/Controller-Bluepad32-green)

This repository contains the custom firmware developed for **TheBoxingBots** combat robot. Unlike standard RC sketches, this project features a fully customized control loop designed for competitive agility and specific offensive strategies.

Powered by an ESP32, this system handles 5-axis servo kinematics (arms + waist) and differential drive movement simultaneously, controlled via a Bluetooth gamepad (PS4/DS4 recommended) using the Bluepad32 library.

## ⚡ Key Features

* **Custom Combo Engine:** Pre-programmed macros for complex moves like the "Ankle Breaker" and "Shoulder Bash."
* **Hybrid Control Scheme:** * **Direct Control:** Dual analog sticks map directly to arm positioning for granular blocking/striking.
    * **Digital Drive:** D-Pad handles movement to prevent drift during complex arm maneuvers.
* **Active Counter-Play:** Specific interrupt functions (Side Step + Jab) mapped to triggers for quick dodges.
* **Fail-Safe:** Built-in emergency disconnect sequence (System + Home).

## 🎮 Controls

The control scheme was designed by us to maximize tactical options during a fight.

### Movement & Basic Actions
| Input | Action | Description |
| :--- | :--- | :--- |
| **D-Pad** | **Movement** | Forward, Backward, Tank Turn Left/Right |
| **Left Stick (Y-Axis)** | **Left Arm** | Raises/Lowers Left Shoulder |
| **Left Stick (X-Axis)** | **Left Elbow** | Extends/Retracts Left Elbow |
| **Right Stick (Y-Axis)** | **Right Arm** | Raises/Lowers Right Shoulder |
| **Right Stick (X-Axis)** | **Right Elbow** | Extends/Retracts Right Elbow |
| **L2 / R2** | **Waist** | Rotates waist Left (L2) or Right (R2) |

### 💥 Special Moves (Macros)
| Button | Move Name | Logic |
| :--- | :--- | :--- |
| **Square (X)** | **The Ankle Breaker** | Drops right arm low, hooks, then rips upward to tip heavy opponents. |
| **L2 + Square** | **Feint Punch** | Rapidly extends and retracts to bait a reaction, then resets. |
| **R3 (Click)** | **Hip Twist Punch** | Winds up the waist and throws a full-body cross. |
| **L3 (Click)** | **Double Jab** | Two rapid-fire snaps of the left arm. |
| **Select** | **Shoulder Bash** | Tuck shoulder and ram forward at full speed. |
| **R1 + D-Pad** | **Side Step Counter** | Quick lateral dash followed immediately by a double jab. |

## 🛠️ Hardware Pinout

This firmware is configured for the following ESP32 wiring map:

### Motor Driver (L298N/TB6612)
* **Motor 1 (Left):** Enable: `13`, Fwd: `12`, Bwd: `14`
* **Motor 2 (Right):** Enable: `25`, Fwd: `27`, Bwd: `26`

### Servos (Arms & Body)
* **Left Shoulder:** GPIO `2`
* **Left Elbow:** GPIO `4`
* **Right Shoulder:** GPIO `5`
* **Right Elbow:** GPIO `18`
* **Waist:** GPIO `19`

## 📦 Dependencies

To build this project, install the following libraries in your Arduino IDE:

1.  **[Bluepad32](https://github.com/ricardoquesada/bluepad32)** - For Bluetooth Gamepad support.
2.  **[ESP32Servo](https://github.com/madhephaestus/ESP32Servo)** - For controlling the 5 servo motors.

## 🚀 Setup

1.  Clone this repository.
2.  Open the `.ino` file in Arduino IDE.
3.  Select your board (ESP32 Dev Module).
4.  Ensure your gamepad is in pairing mode (Share + PS button for DS4).
5.  Upload the code.
6.  Open Serial Monitor (`115200` baud) to confirm "WAR BOT: ANKLE BREAKER READY".

## ⚠️ Safety Note
The "Ankle Breaker" and "Shoulder Bash" moves utilize full motor torque. Ensure the robot is on the floor or a test stand before activating these macros to prevent damage to the servos or surrounding objects.

---
*Developed for TheBoxingBots competition.*
