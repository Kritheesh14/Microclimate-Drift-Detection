#include <esp_now.h>
#include <WiFi.h>

// --- LED 1 (Environmental - GPIO 25,26,27) ---
#define E_RED   25
#define E_GREEN 26
#define E_BLUE  27

// --- LED 2 (Actuation - GPIO 18,5,23) ---
#define A_RED   18  
#define A_GREEN 5   
#define A_BLUE  23  

typedef struct struct_message {
    float temp;
    float hum;
    int light;
    int wind;
} struct_message;

struct_message incoming;   // Data from the real Node
struct_message testbench;  // Simulated Node values

// Helper to handle Common Anode Inversion
void setColor(int r, int g, int b, int Rpin, int Gpin, int Bpin) {
  digitalWrite(Rpin, r ? LOW : HIGH);
  digitalWrite(Gpin, g ? LOW : HIGH);
  digitalWrite(Bpin, b ? LOW : HIGH);
}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&incoming, incomingData, sizeof(incoming));

  // --- SENSOR FUSION & COMPARISON LOGIC (LED 1) ---
  
  // 1. Zone Variation (Same Temp + Different Humidity)
  // Logic: If Temp is within 1 degree but Humidity differs by more than 15%
  if (abs(incoming.temp - testbench.temp) < 1.0 && abs(incoming.hum - testbench.hum) > 15.0) {
    setColor(1, 0, 0, E_RED, E_GREEN, E_BLUE); // RED: Moisture Drift Alert
  } 
  
  // 2. Evapotranspiration Shift (Same Light + Different Wind)
  // Logic: If Light is similar but Wind speed diverges significantly
  else if (abs(incoming.light - testbench.light) < 10 && abs(incoming.wind - testbench.wind) > 20) {
    setColor(0, 0, 1, E_RED, E_GREEN, E_BLUE); // BLUE: Evapotranspiration Risk
  } 
  
  // 3. System Healthy
  else {
    setColor(0, 1, 0, E_RED, E_GREEN, E_BLUE); // GREEN: Stable
  }

  // --- ACTUATION LOGIC (LED 2) ---
  // (Remains based on actual incoming environmental needs)
  if (incoming.temp > 30.0) {
    setColor(1, 0, 0, A_RED, A_GREEN, A_BLUE); // RED: Simulated Cooling
  } 
  else if (incoming.hum < 40.0) { // Using 40 as a threshold for humidifier
    setColor(0, 0, 1, A_RED, A_GREEN, A_BLUE); // BLUE: Simulated Humidifier
  } 
  else {
    setColor(0, 1, 0, A_RED, A_GREEN, A_BLUE); // GREEN: Standby
  }

  Serial.println("--- Comparison Report ---");
  Serial.printf("LIVE -> T:%.1f H:%.1f L:%d W:%d\n", incoming.temp, incoming.hum, incoming.light, incoming.wind);
  Serial.printf("TEST -> T:%.1f H:%.1f L:%d W:%d\n", testbench.temp, testbench.hum, testbench.light, testbench.wind);
}

void setup() {
  Serial.begin(115200);

  // Initialize Testbench Values (Adjust these to test your logic)
  testbench.temp = 25.0;
  testbench.hum = 50.0;
  testbench.light = 70;
  testbench.wind = 10;

  int pins[] = {E_RED, E_GREEN, E_BLUE, A_RED, A_GREEN, A_BLUE};
  for (int i : pins) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH); 
  }

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Control Hub v1.0 (Fusion Logic Enabled)");
}

unsigned long lastSwitchTime = 0;
int demoState = 0;

void loop() {
  // Check if 15 seconds have passed
  if (millis() - lastSwitchTime >= 15000) {
    lastSwitchTime = millis();
    demoState++;
    if (demoState > 2) demoState = 0; // Cycle through 3 Environmental states

    switch (demoState) {
      case 0: // STATE: SYSTEM HEALTHY
        Serial.println("\n[DEMO] Switching to: SYSTEM HEALTHY (LED 1 GREEN)");
        testbench.temp = incoming.temp; 
        testbench.hum = incoming.hum;
        testbench.light = incoming.light;
        testbench.wind = incoming.wind;
        break;

      case 1: // STATE: MOISTURE DRIFT
        Serial.println("\n[DEMO] Switching to: MOISTURE DRIFT (LED 1 RED)");
        // Force drift: Keep temp same, but make testbench humidity very different
        testbench.temp = incoming.temp; 
        testbench.hum = incoming.hum + 20.0; // Offset by 20%
        testbench.light = incoming.light;
        testbench.wind = incoming.wind;
        break;

      case 2: // STATE: EVAPOTRANSPIRATION RISK
        Serial.println("\n[DEMO] Switching to: EVAPOTRANSPIRATION RISK (LED 1 BLUE)");
        // Force risk: Keep light same, but make testbench wind very different
        testbench.temp = incoming.temp;
        testbench.hum = incoming.hum;
        testbench.light = incoming.light;
        testbench.wind = incoming.wind + 30; // Offset wind speed
        break;
    }
  }
}
