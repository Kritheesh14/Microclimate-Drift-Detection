#include <esp_now.h>
#include <WiFi.h>
#include "DHT.h"

// Pin Assignments
#define DHTPIN 4
#define DHTTYPE DHT22
#define LDRPIN 34
#define POTPIN 35

// MAC Address of Node 2 (Verified from your earlier log)
uint8_t broadcastAddress[] = {0xD4, 0xE9, 0xF4, 0xB2, 0x59, 0xC0};

typedef struct struct_message {
    float temp;
    float hum;
    int light;
    int wind;
} struct_message;

struct_message myData;
DHT dht(DHTPIN, DHTTYPE);
esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  
  analogReadResolution(12); 
  Serial.println("Node 1 Sender Active.");
}

void loop() {
  myData.temp = dht.readTemperature();
  myData.hum = dht.readHumidity();
  myData.light = map(analogRead(LDRPIN), 0, 4095, 100, 0); 
  myData.wind = map(analogRead(POTPIN), 0, 4095, 0, 100);

  // Send the data
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  Serial.print("Send status: ");
  if (result == ESP_OK) {
    Serial.println("Success");
  } else {
    Serial.println("Error");
  }
  
  delay(2000); 
}