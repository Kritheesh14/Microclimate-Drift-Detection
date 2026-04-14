# ESP32 [ESP-NOW] Sensor Node (Node 1)

This repository contains the source code for a **Sender Node** (Node 1) in a wireless sensor network. It utilizes the **ESP-NOW** protocol to transmit environmental data from an ESP32 to a designated receiver without the need for a Wi-Fi gateway.

## Project Gist
The system reads environmental data (Temperature, Humidity, Light intensity, and Wind speed simulation) and transmits it over the 2.4GHz band to a specific MAC address. This is ideal for low-power agricultural or industrial monitoring where low latency is required.

---

## System Architecture

### 1. Data Structure (`struct_message`)
To ensure data integrity between the sender and receiver, a custom structure is used. This acts as a "data packet" definition:
- `temp` (float): Temperature in Celsius.
- `hum` (float): Humidity percentage.
- `light` (int): Light intensity mapped to 0-100.
- `wind` (int): Potentiometer-simulated wind speed mapped to 0-100.

### 2. Hardware Mapping
| Component | ESP32 Pin | Description |
| :--- | :--- | :--- |
| **DHT22** | GPIO 4 | Digital Temperature & Humidity |
| **LDR** | GPIO 34 | Analog Light Intensity |
| **Potentiometer** | GPIO 35 | Analog Wind Simulation |

---

## Logic Breakdown

### Initialization (`setup`)
- **Wi-Fi Mode:** Set to `WIFI_STA` (Station Mode) to activate the radio for ESP-NOW.
- **ESP-NOW Init:** Initializes the protocol. If it fails, the system halts to prevent ghost transmissions.
- **Peer Registration:** Node 2 is added as a "peer" using its unique MAC address (`D4:E9:F4:B2:59:C0`). Encryption is disabled for this implementation to prioritize speed.

### Operation Loop (`loop`)
1. **Sensing:** The system polls the DHT22 and the analog pins.
2. **Data Normalization:** Raw 12-bit analog values (0-4095) are mapped to a 0-100 range for standardized reporting.
3. **Transmission:** The `esp_now_send()` function packages the struct and transmits it.
4. **Status Feedback:** The Serial Monitor prints the delivery status (`ESP_OK`) every 2 seconds.

---

## Deployment Instructions
1. Open the code in the **Arduino IDE**.
2. Install the **DHT Sensor Library** by Adafruit.
3. Ensure the `broadcastAddress[]` matches the MAC address of your receiver node.
4. Set the Upload Speed to `115200` and the Board to **ESP32 Dev Module**.
