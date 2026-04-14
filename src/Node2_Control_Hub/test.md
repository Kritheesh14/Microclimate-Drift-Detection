# ESP32 ESP-NOW Receiver & Actuator (Node 2)

This repository contains the source code for the **Receiver Node** (Node 2). This node acts as the visual dashboard and decision-maker for the wireless sensor network, interpreting data from Node 1 to drive physical indicators.

## Project Gist
Node 2 receives environmental telemetry via the **ESP-NOW protocol**. It processes the data through a logic engine to determine if the microclimate is "drifting" and decides which actuators (simulated by RGB LEDs) should be engaged.

---

## System Architecture

### 1. RGB LED Configuration
The system uses **Common Anode** RGB LEDs. 
- **Inverted Logic:** Pins are driven `LOW` to activate a color and `HIGH` to deactivate.
- **Visual Feedback:** - **LED 1 (Drift):** Monitoring environmental stability.
    - **LED 2 (Actuation):** Monitoring system response (Cooling/Humidifying).

### 2. Hardware Mapping
| Component | Function | Pins (R, G, B) |
| :--- | :--- | :--- |
| **RGB LED 1** | Drift Status | GPIO 25, 26, 27 |
| **RGB LED 2** | Actuator Status | GPIO 18, 5, 23 |

---

## Logic Breakdown

### Data Reception (`OnDataRecv`)
The node utilizes a non-blocking callback mechanism. Upon receiving a packet:
1. Data is mapped to the `struct_message` format.
2. **Environmental Drift Check:** Evaluates humidity and light/wind ratios to detect anomalies.
3. **Actuation Trigger:** Evaluates temperature and humidity thresholds to determine the required mechanical response.

### Status Indicators
| State | LED 1 (Drift) | LED 2 (Actuation) |
| :--- | :--- | :--- |
| **Normal** | 🟢 Green (Stable) | 🟢 Green (Idle) |
| **High Temp** | - | 🔴 Red (Cooling On) |
| **High Hum** | 🔴 Red (Alert) | 🔵 Blue (Ventilate) |
| **Stagnant Heat**| 🔵 Blue (Check Wind) | - |

---

## 🚀 Deployment Instructions
1. Ensure the **Sender Node (Node 1)** is configured with the MAC address of this ESP32.
2. Connect RGB LEDs using appropriate current-limiting resistors (220Ω recommended).
3. Set the Upload Speed to `115200` and the Board to **ESP32 Dev Module**.
