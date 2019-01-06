#include <ESPAsyncWebServer.h>

#include "ESPLamp.h"
#include "ESPLampLEDs.h"
#include "ESPLampMode.h"
#include "ESPLampSettings.h"

ESPLamp::ESPLamp(uint16_t ledCount, uint8_t rowCount, uint8_t lampCount, uint8_t lampId) : id(lampId), count(lampCount) {
  server = 0;
  settings = new ESPLampSettings();
  leds = new ESPLampLEDs(ledCount, rowCount);
}

ESPLamp::~ESPLamp() {
  delete settings;
  delete leds;

  if (server != 0) {
    delete server;
  }
}

void ESPLamp::addMode(ESPLampMode* mode) {
  settings->addMode(mode);
}

void ESPLamp::begin() {
  Serial.print("init LEDs...");
  leds->begin(this);
  Serial.println("done");

  Serial.print("init settings...");
  settings->begin(this);
  Serial.println("done");
}

void ESPLamp::beginHttpServer() {
  if (server != 0) {
    delete server;
  }

  server = new AsyncWebServer(80);

  Serial.print("init SPIFFS...");
  SPIFFS.begin();
  Serial.println("done");

  Serial.print("init HTTP server...");
  server->begin();
  server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");
  Serial.println("done");

  Serial.print("init settings HTTP routings...");
  settings->beginHttpServer();
  Serial.println("done");
}

void ESPLamp::beginSocketServer() {
  Serial.print("init settings socket server...");
  settings->beginSocketServer();
  Serial.println("done");
}

void ESPLamp::beginSocketClient() {
  Serial.print("init settings socket client...");
  settings->beginSocketClient();
  Serial.println("done");
}

void ESPLamp::loop() {
  settings->loop();

  if (mode) {
    mode->loop();
  }
}
