# Microclimate-Drift-Detection

## Project Description
Traditional agricultural monitoring systems often assume **field uniformity**, ignoring the reality that topography, soil density, and local obstructions create distinct **Micro-zones**. This system is a distributed, low-latency diagnostic tool designed to detect **Environmental Drift**. 

Using **ESP-NOW** for wireless communication, the system transmits high-frequency sensor data from a remote zone (Node 1) to a central hub (Node 2). It performs **Sensor Fusion** to identify anomalies—such as high light intensity without corresponding wind flow—flagging dehydration risks (Evapotranspiration Shift) before they become visible to the eye.

---

## 🛠 Component Requirements & Selection Rationale

| Component | Why it was chosen | Role in System |
| :--- | :--- | :--- |
| **ESP32 (DevKit V1)** | Chosen for its integrated Wi-Fi stack and **ESP-NOW** protocol, allowing low-power, peer-to-peer communication without a router. | Central MCU / Wireless Transceiver |
| **DHT22** | Selected over the DHT11 for **higher precision** ($\pm 2\% \text{ humidity}$) and wider range, essential for detecting subtle micro-drifts. | Temp & Humidity Monitoring |
| **LDR (Photoresistor)**| Cost-effective way to measure **relative solar radiation**. Paired with wind data, it calculates evapotranspiration risks. | Light Intensity Sensing |
| **Potentiometer** | Provides a controllable analog input to **simulate wind speed**. Essential for testing system logic in a lab setting. | Wind Flow Simulation |
| **RGB LEDs (Common Anode)**| Chosen for a **Multi-variable Status Interface**, allowing color-coded "Heat Maps" of system health at a glance. | Status & Actuation Simulation |
| **220$\Omega$ Resistors** | Essential **Current Limiters** to protect the ESP32’s GPIO pins from thermal stress and prevent LED burnout. | Hardware Protection |

---

## Detailed Wiring Diagram Tables

### **Node 1: Remote Sensor Station**
*The "Eyes" of the system. Focuses on data acquisition.*

| Component | Pin Label | ESP32 Pin | Wiring Detail |
| :--- | :--- | :--- | :--- |
| **DHT22** | VCC | **3V3** | Add 10k$\Omega$ pull-up resistor to Data pin |
| | Data | **GPIO 4 (D4)** | Signal out |
| | GND | **GND** | Common Ground |
| **LDR** | Leg 1 | **3V3** | Power in |
| | Leg 2 | **GPIO 34 (D34)** | Connect 10k$\Omega$ resistor from here to GND |
| **Potentiometer** | Left Leg | **3V3** | Reference Voltage |
| | Middle Leg | **GPIO 35 (D35)** | Variable Analog Output |
| | Right Leg | **GND** | Ground |

---

### **Node 2: Processing & Actuator Hub**
*The "Brain" of the system. Focuses on logic and visual alerts.*

| Component | Pin Label | ESP32 Pin | Wiring Detail |
| :--- | :--- | :--- | :--- |
| **Common Anode RGB 1** | Longest Pin | **3V3** | **CRITICAL:** Anode connects to 3.3V Rail |
| (Environmental Status)| Red (R) | **GPIO 25** | Through 220$\Omega$ Resistor |
| | Green (G) | **GPIO 26** | Through 220$\Omega$ Resistor |
| | Blue (B) | **GPIO 27** | Through 220$\Omega$ Resistor |
| **Common Anode RGB 2** | Longest Pin | **3V3** | **CRITICAL:** Anode connects to 3.3V Rail |
| (Actuator Simulation) | Red (R) | **GPIO 18** | Through 220$\Omega$ Resistor |
| | Green (G) | **GPIO 5** | Through 220$\Omega$ Resistor |
| | Blue (B) | **GPIO 23** | Through 220$\Omega$ Resistor |
