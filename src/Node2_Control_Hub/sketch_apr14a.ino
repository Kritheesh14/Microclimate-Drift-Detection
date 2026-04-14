#include <esp_now.h>
#include <WiFi.h>

// --- LED 1 (existing: GPIO 25,26,27) ---
#define E_RED   25
#define E_GREEN 26
#define E_BLUE  27

// --- LED 2 (new mapping) ---
#define A_RED   18  
#define A_GREEN 5   
#define A_BLUE  23  

typedef struct struct_message {
    float temp;
    float hum;
    int light;
    int wind;
} struct_message;

struct_message incoming;

// Common ANODE → invert logic
void setColor(int r, int g, int b, int Rpin, int Gpin, int Bpin) {
  digitalWrite(Rpin, r ? LOW : HIGH);
  digitalWrite(Gpin, g ? LOW : HIGH);
  digitalWrite(Bpin, b ? LOW : HIGH);
}

void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  memcpy(&incoming, incomingData, sizeof(incoming));

  // --- DRIFT LOGIC (LED 1) ---
  if (incoming.hum > 75.0) {
    setColor(1,0,0, E_RED, E_GREEN, E_BLUE);   // Red
  } 
  else if (incoming.light > 80 && incoming.wind < 20) {
    setColor(0,0,1, E_RED, E_GREEN, E_BLUE);   // Blue
  } 
  else {
    setColor(0,1,0, E_RED, E_GREEN, E_BLUE);   // Green
  }

  // --- ACTUATION LOGIC (LED 2) ---
  if (incoming.temp > 30.0) {
    setColor(1,0,0, A_RED, A_GREEN, A_BLUE);   // Cooling
  } 
  else if (incoming.hum > 75.0) {
    setColor(0,0,1, A_RED, A_GREEN, A_BLUE);   // Humidifier
  } 
  else {
    setColor(0,1,0, A_RED, A_GREEN, A_BLUE);   // Idle
  }

  Serial.printf("RCVD Data -> T:%.1f H:%.1f L:%d W:%d\n",
                incoming.temp, incoming.hum, incoming.light, incoming.wind);
}

void setup() {
  Serial.begin(115200);

  int pins[] = {E_RED, E_GREEN, E_BLUE, A_RED, A_GREEN, A_BLUE};
  for (int i : pins) {
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH); // OFF for common anode
  }

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW Init Failed");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  Serial.println("Node 2 Ready");
}

void loop() {}