#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

// WiFi Credentials
#define WIFI_SSID     "YOUR_WIFI_SSID"
#define WIFI_PASS     "YOUR_WIFI_PASSWORD"

// Sinric Pro Credentials
#define APP_KEY       "YOUR_APP_KEY"
#define APP_SECRET    "YOUR_APP_SECRET"

// Your 3 device IDs from Sinric Pro
#define DEVICE_ID1    "YOUR_DEVICE_ID_LED1"
#define DEVICE_ID2    "YOUR_DEVICE_ID_LED2"
#define DEVICE_ID3    "YOUR_DEVICE_ID_LED3"

// LED pins
#define LED1 D1
#define LED2 D2
#define LED3 D3

// Callback function for LED control
bool onPowerState(const String &deviceId, bool &state) {
  if (deviceId == DEVICE_ID1) {
    digitalWrite(LED1, state ? HIGH : LOW);
    Serial.printf("LED1 %s\r\n", state ? "ON" : "OFF");
  }
  else if (deviceId == DEVICE_ID2) {
    digitalWrite(LED2, state ? HIGH : LOW);
    Serial.printf("LED2 %s\r\n", state ? "ON" : "OFF");
  }
  else if (deviceId == DEVICE_ID3) {
    digitalWrite(LED3, state ? HIGH : LOW);
    Serial.printf("LED3 %s\r\n", state ? "ON" : "OFF");
  }
  return true; // request handled properly
}

void setupWiFi() {
  Serial.printf("\r\n[WiFi] Connecting to %s", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(250);
  }
  Serial.printf("\r\n[WiFi] Connected! IP: %s\r\n", WiFi.localIP().toString().c_str());
}

void setupSinricPro() {
  SinricProSwitch &mySwitch1 = SinricPro[DEVICE_ID1];
  mySwitch1.onPowerState(onPowerState);

  SinricProSwitch &mySwitch2 = SinricPro[DEVICE_ID2];
  mySwitch2.onPowerState(onPowerState);

  SinricProSwitch &mySwitch3 = SinricPro[DEVICE_ID3];
  mySwitch3.onPowerState(onPowerState);

  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); });
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });

  SinricPro.begin(APP_KEY, APP_SECRET);
}

void setup() {
  Serial.begin(115200);
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);

  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}