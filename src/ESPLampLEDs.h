#ifndef ESP_LAMP_LEDS_H
#define ESP_LAMP_LEDS_H

#include <NeoPixelBus.h>

class AsyncWebServerRequest;
class ESPLamp;

class ESPLampLEDs {
  public:
    uint16_t ledCount;
    uint8_t rowCount;
    uint8_t columnCount;

    ESPLampLEDs(uint16_t ledCount, uint8_t rowCount);
    virtual ~ESPLampLEDs();

    void begin(ESPLamp* lamp);

    char* toJson();

    uint32_t getPixel(uint16_t index);

    void setPixel(uint16_t index, uint32_t color);
    void setPixel(uint16_t index, uint8_t red, uint8_t green, uint8_t blue);

    void show();

    void get(AsyncWebServerRequest* request);

    void put(AsyncWebServerRequest* request, uint8_t* data, size_t len);

  protected:
    ESPLamp* lamp;
    NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod>* strip;
};

#endif // ESP_LAMP_LEDS_H
