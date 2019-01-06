#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <NeoPixelBus.h>
#include "ESPLamp.h"
#include "ESPLampLEDs.h"
#include "ESPLampSettings.h"
#include "ESPLampColor.h"

ESPLampLEDs::ESPLampLEDs(uint16_t ledCount, uint8_t rowCount) : ledCount(ledCount), rowCount(rowCount) {
  columnCount = floor((float)ledCount / (float)rowCount);

  strip = new NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod>(ledCount, 0);
}

ESPLampLEDs::~ESPLampLEDs() {
  delete strip;
}

void ESPLampLEDs::begin(ESPLamp* lamp) {
  this->lamp = lamp;

  strip->Begin();

  if (lamp->server != 0) {
    lamp->server->on("/leds", HTTP_GET, [this](AsyncWebServerRequest* request) {
      Serial.println("GET /leds");

      get(request);
    });

    lamp->server->on("/leds", HTTP_PUT, [](AsyncWebServerRequest* request) {
    }, [](AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
    }, [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
      Serial.println("PUT /leds");

      put(request, data, len);
    });
  }
}

char* ESPLampLEDs::toJson() {
  DynamicJsonBuffer buffer;

  JsonObject& json = buffer.createObject();

  json["rows"] = rowCount;
  json["columns"] = columnCount;

  JsonArray& color = json.createNestedArray("color");

  for(uint16_t i=0; i<ledCount; i++) {
    color.add(colorToCss(getPixel(i)));
  }

  size_t length = json.measureLength() + 1;
  char* content = new char[length];

  json.printTo(content, length);

  return content;
}

uint32_t ESPLampLEDs::getPixel(uint16_t index) {
  HtmlColor htmlColor = strip->GetPixelColor(index);

  return htmlColor.Color;
}

void ESPLampLEDs::setPixel(uint16_t index, uint32_t color) {
  strip->SetPixelColor(index, HtmlColor(color));
}

void ESPLampLEDs::setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue) {
  uint32_t color = (red << 16) | (green << 8) | blue;

  setPixel(index, color);
}

void ESPLampLEDs::show() {
  strip->Show();
}

void ESPLampLEDs::get(AsyncWebServerRequest* request) {
  char* content = toJson();

  AsyncWebServerResponse* response = request->beginResponse(200, "application/json", content);
  request->send(response);

  delete[] content;
}

void ESPLampLEDs::put(AsyncWebServerRequest* request, uint8_t* data, size_t len) {
  get(request);
}
