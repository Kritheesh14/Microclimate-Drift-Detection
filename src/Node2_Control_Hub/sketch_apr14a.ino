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
struct_message testbench;  // Simulated Node values for comparison

unsigned long lastSwitchTime = 0;
int demoState = 0;

// Common ANODE → invert logic (LOW = ON, HIGH = OFF)
void setColor(int r, int g, int b, int Rpin, int Gpin, int Bpin) {
  digitalWrite(Rpin, r ? LOW : HIGH);
  digitalWrite(Gpin, g ? LOW : HIGH);
  digitalWrite(Bpin, b ? LOW : HIGH);
}

// Function to update LED states based on Fusion and Actuation logic
void updateLEDs() {
  // --- SENSOR FUSION & COMPARISON LOGIC (LED 1) ---
  // 2. Zone Variation (Moisture Drift)
  if (abs(incoming.temp - testbench.temp) < 1.0 && abs(incoming.hum - testbench.hum) > 15.0) {
    setColor(1, 0, 0, E_RED, E_GREEN, E_BLUE); // RED
  } 
/*
 else if (incoming.temp > 10.0 && incoming.light > 80) {
    setColor(1, 0,  1, E_RED, E_GREEN, E_BLUE); // PURPLE (Red + Blue)
    Serial.println("ALERT: High Temp & Light! Temperature regulation taking place.");
  } */
  // 3. Evapotranspiration Shift
  else if (abs(incoming.light - testbench.light) < 10 && abs(incoming.wind - testbench.wind) > 20) {
    setColor(0, 0, 1, E_RED, E_GREEN, E_BLUE); // BLUE
  } 
  // 4. System Healthy
  else {
    setColor(0, 1, 0, E_RED, E_GREEN, E_BLUE); // GREEN
  }

  // --- ACTUATION LOGIC (LED 2) ---
  if (incoming.temp > 30.0) {
    setColor(1, 0, 0, A_RED, A_GREEN, A_BLUE); // RED: Cooling
  } 
  else if (incoming.hum < 40.0) { 
    setColor(0, 0, 1, A_RED, A_GREEN, A_BLUE); // BLUE: Humidifier
  } 
  else {
    setColor(0, 1, 0, A_RED, A_GREEN, A_BLUE); // GREEN: Standby
  }
}

// Callback when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&incoming, incomingData, sizeof(incoming));

  Serial.printf("RCVD Data -> T:%.1f H:%.1f L:%d W:%d\n", 
                incoming.temp, incoming.hum, incoming.light, incoming.wind);

  updateLEDs();
}

void setup() {
  Serial.begin(115200);

  // Initialize LEDs
  int pins[] = {E_RED, E_GREEN, E_BLUE, A_RED, A_GREEN, A_BLUE};
  for (int i : pins) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH); // Start OFF for common anode
  }

  // Set initial Testbench values
  testbench.temp = 25.0;
  testbench.hum = 50.0;
  testbench.light = 70;
  testbench.wind = 10;

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Control Hub Node 2 Ready");
}

void loop() {
  // Demo cycle: Change testbench values every 15 seconds
  if (millis() - lastSwitchTime >= 15000) {
    lastSwitchTime = millis();
    demoState++;
    if (demoState > 2) demoState = 0;

    switch (demoState) {
      case 0: // SYSTEM HEALTHY
        Serial.println("\n[DEMO] Mode: SYSTEM HEALTHY");
        testbench.temp = incoming.temp; 
        testbench.hum = incoming.hum;
        testbench.light = incoming.light;
        testbench.wind = incoming.wind;
        break;

      case 1: // MOISTURE DRIFT
        Serial.println("\n[DEMO] Mode: MOISTURE DRIFT");
        testbench.temp = incoming.temp; 
        testbench.hum = incoming.hum + 20.0; 
        testbench.light = incoming.light;
        testbench.wind = incoming.wind;
        break;

      case 2: // EVAPOTRANSPIRATION RISK
        Serial.println("\n[DEMO] Mode: EVAPOTRANSPIRATION");
        testbench.temp = incoming.temp;
        testbench.hum = incoming.hum;
        testbench.light = incoming.light;
        testbench.wind = incoming.wind + 30; 
        break;
    }
    updateLEDs();
  }
}
