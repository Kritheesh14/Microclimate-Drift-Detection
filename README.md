# Microclimate-Drift-Detection  

## The Problem
Traditional agricultural monitoring systems often assume **field uniformity**. In reality, different zones within the same field behave differently due to topography, soil density, and surrounding structures. This oversight leads to uneven crop growth and inefficient resource usage.

---

## The Solution
This system focuses on **Detecting Micro-zone Variation** rather than enforcing environmental correction. It identifies "unstable zones" where environmental parameters diverge from expected correlations. The system provides **decision-level insight**, not physical intervention.

---

## Sensor Array
To map environmental drift, the system uses:

* **Temperature & Humidity:** via DHT22  
* **Light Intensity:** via LDR  
* **Wind Flow (Simulated):** via potentiometer  

---

## Sensor Fusion Logic
The system identifies instability by cross-referencing variables that *should* correlate but do not.

| Scenario | Data Divergence | Identified Impact |
| :--- | :--- | :--- |
| **Zone Variation** | Same Temp + Different Humidity | Localized moisture imbalance |
| **Evapotranspiration Shift** | Same Light + Different Wind | High dehydration risk |
| **Unstable Patches** | Multi-variable divergence | Microclimate inconsistency |

---

## System Architecture (2-Node)

The system is divided into two distributed nodes using ESP32 communication.

### Node 1 — Sensor Station (Remote Node)
- Collects environmental data  
- Components:
  - DHT22 (temperature & humidity)  
  - LDR (light intensity)  
  - Potentiometer (wind simulation)  
- Sends processed sensor data periodically  

---

### Node 2 — Control Hub (Processing Node)
- Receives data from Node 1  
- Performs comparison and drift detection  
- Drives outputs (fan + LED indicators)  
- Acts as central decision unit  

---

## Communication Framework

- ESP32-to-ESP32 communication using **ESP-NOW**  
- Node 1 transmits structured data packets  
- Node 2 receives and updates system state in real time  

---

## Workflow

1. **Initialization**  
   Both ESP32 nodes power on and establish communication.

2. **Data Acquisition (Node 1)**  
   - Reads temperature, humidity, light, and wind input  
   - Formats data into a structured packet  

3. **Transmission**  
   - Sends data wirelessly to Node 2  

4. **Data Processing (Node 2)**  
   - Receives incoming data  
   - Compares against predefined thresholds  
   - Evaluates correlations between variables  

5. **Drift Detection**  
   - Identifies anomalies where expected relationships fail  
   - Flags unstable environmental zones  

6. **Output Indication**  
   - Fan ON → high temperature condition  
   - LED ON → low temperature (simulated heating state)  

7. **Continuous Loop**  
   - System repeats sensing → transmission → analysis  

---

## Key Outputs & Benefits

1. **Identifies Unstable Zones**  
   Detects environmental inconsistencies using sensor fusion  

2. **Drift Awareness**  
   Highlights deviations from expected environmental behavior  

3. **Low-Cost Monitoring System**  
   Uses minimal hardware for distributed sensing  

4. **Scalable Architecture**  
   Additional sensor nodes can be integrated  

---

## System Limitations

- No real-world environmental correction (simulation only)  
- No irrigation or fertilization control  
- LED represents heating instead of actual thermal regulation  

---

## Final Classification

This system functions as a:

**Microclimate Drift Detection and Analysis System**

It performs:
- Multi-variable sensing  
- Data fusion  
- Anomaly detection  

It does not perform:
- Physical environmental control  
- Resource actuation  

---


## 🛠️ Current Hardware Implementation (Build v1.0)

### Node 2 Hub Configuration
The Control Hub utilizes two **Common Anode RGB LEDs** for status visualization. Unlike standard common cathode LEDs, these require a **HIGH** signal to turn **OFF** and a **LOW** signal to turn **ON**.

| Component | Channel | ESP32 GPIO | Logic Purpose |
| :--- | :--- | :--- | :--- |
| **Environmental LED (LED 1)** | Red | **25** | Moisture Drift Alert |
| | Green | **26** | System Healthy/Stable |
| | Blue | **27** | Evapotranspiration Risk |
| **Actuation LED (LED 2)** | Red | **18** | Simulated Cooling (Fan) |
| | Green | **5** | System Idle/Standby |
| | Blue | **23** | Simulated Humidifier |

### Technical Wiring Requirements
- **Current Limiting:** Every color channel (6 total) is equipped with a **220Ω resistor** to protect the ESP32 GPIO pins from overcurrent.
- **Power Rail:** Common Anode pins are connected to the **3.3V rail** of the ESP32.
- **Pull-up Resistor:** The DHT22 on Node 1 utilizes a **10kΩ pull-up resistor** on the data line for signal stability.

---

## 📡 Wireless Communication Protocol

The system utilizes **ESP-NOW**, a connectionless Wi-Fi communication protocol developed by Espressif. This allows for:
- **Low Latency:** Immediate transmission of sensor packets without the overhead of a traditional Wi-Fi handshake.
- **Unicast Transmission:** Node 1 is hardcoded with the specific MAC address of Node 2 (`D4:E9:F4:B2:59:C0`), ensuring data integrity within the distributed network.

### Data Packet Structure
```cpp
typedef struct struct_message {
    float temp;   // Celsius
    float hum;    // Percentage
    int light;    // 0-100% (Mapped from 12-bit ADC)
    int wind;     // 0-100% (Mapped from Potentiometer)
} struct_message;
```

---

## Sensor Fusion & Comparison Logic (Build v2.0)
Unlike basic monitoring, this system identifies instability by cross-referencing **Live Node** data against a **Virtual Testbench Node**. Drift is detected when variables that should correlate between the two environments actually diverge.

| Scenario | Data Divergence (Live vs. Testbench) | Identified Impact | LED 1 Color |
| :--- | :--- | :--- | :--- |
| **Zone Variation** | Similar Temp + Different Humidity ($>15\%$) | Localized moisture imbalance | **Red** |
| **Evapotranspiration**| Similar Light + Different Wind ($>20$) | High dehydration risk | **Blue** |
| **System Healthy** | Multi-variable correlation/Stability | Microclimate consistency | **Green** |

---

## 🛠️ Updated Hardware Implementation

### Node 2 Hub Configuration (Common Anode)
*Logic: HIGH signal = OFF, LOW signal = ON.*

| Component | Channel | GPIO | Logic Purpose |
| :--- | :--- | :--- | :--- |
| **Environmental LED (LED 1)** | Red | **25** | Moisture Drift Alert (Fusion Logic) |
| | Green | **26** | System Healthy/Stable |
| | Blue | **27** | Evapotranspiration Risk (Fusion Logic) |
| **Actuation LED (LED 2)** | Red | **18** | Simulated Cooling Active ($T > 30^\circ C$) |
| | Green | **5** | System Idle/Standby |
| | Blue | **23** | Simulated Humidifier Active ($H < 40\%$) |

---

## Automated Demonstration Workflow

The Control Hub (Node 2) now features an automated **15-second loop** to demonstrate every system state using the software-defined Testbench:

1. **State 0 (0-15s): Healthy Mode** - Testbench values are synced to Live data. 
   - **Result:** LED 1 turns **Green**.
2. **State 1 (15-30s): Moisture Drift Demo** - Testbench humidity is offset by $+20\%$ relative to Live. 
   - **Result:** LED 1 turns **Red**.
3. **State 2 (30-45s): Dehydration Risk Demo** - Testbench wind speed is offset by $+30$ relative to Live. 
   - **Result:** LED 1 turns **Blue**.

*Note: LED 2 continues to operate independently, reacting in real-time to the physical environment of Node 1 regardless of the demo state.*
