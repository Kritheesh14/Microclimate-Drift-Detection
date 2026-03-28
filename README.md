# Microclimate-Drift-Detection  

## The Problem
Traditional agricultural monitoring systems often assume **field uniformity**. In reality, different zones within the same field behave differently due to topography, soil density, and surrounding structures. This oversight leads to uneven crop growth and wasted resources.

## The Solution
This system moves beyond raw data collection to **Detecting Micro-zone Variation**. Instead of just reporting values, it identifies "unstable zones" where environmental factors diverge from the field average.

---

## Sensor Array
To map these instabilities, the system fuses data from four primary inputs:
* **Temperature:** Ambient heat levels.
* **Humidity:** Relative moisture in the air.
* **Light Intensity:** Solar radiation and shading.
* **Wind Flow:** Localized air velocity (Anemometry).

---

## Sensor Fusion Logic
The system identifies instability by cross-referencing variables that *should* correlate but don't.

| Scenario | Data Divergence | Identified Impact |
| :--- | :--- | :--- |
| **Zone Variation** | Same Temp + Different Humidity | Localized moisture trap or soil drainage issue. |
| **Evapotranspiration Shift** | Same Light + Different Wind | High-risk zone for rapid plant dehydration. |
| **Unstable Patches** | Combined Variable Variation | Micro-climates requiring immediate intervention. |

---

## Key Outputs & Benefits
Unlike standard systems that provide a dashboard of numbers, this system outputs **Actionable Intelligence**:

1.  **Identifies "Unstable Zones":** Pinpoints exactly where the microclimate is failing.
2.  **Targeted Irrigation:** Water only the zones showing high evapotranspiration.
3.  **Precision Fertilization:** Adjust nutrients based on localized growth conditions.
4.  **Resource Efficiency:** Prevents the "one-size-fits-all" approach to field management.
