#include <ESP8266mDNS.h>
#include <ESP8266WiFi.h>
#include <ESPLamp.h>
#include <ESPLampSettings.h>
#include <WiFiManager.h>

#include "ModeSolid.h"

#define LED_COUNT 144
#define LED_ROW_COUNT 1

ESPLamp lamp(LED_COUNT, LED_ROW_COUNT);

void setup() {
  Serial.begin(115200);

  // init lamp and run first loop, so we don't need to wait for WiFi
  lamp.settings->primaryColor = 0xffffffff;
  lamp.addMode(new ModeSolid());
  lamp.begin();
  lamp.loop();

  delay(10);

  // init WiFi
  WiFiManager wifiManager;

  wifiManager.autoConnect("esp-lamp");
}

void loop() {
  lamp.loop();
  delay(10);
}
