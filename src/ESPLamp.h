#ifndef ESP_LAMP_H
#define ESP_LAMP_H

#include <Arduino.h>

class AsyncWebServer;
class ESPLamp;
class ESPLampLEDs;
class ESPLampMode;
class ESPLampSettings;

class ESPLampMode {
  public:
    String name;

    virtual void begin(ESPLamp* lamp) = 0;

    virtual void loop() = 0;
};

class ESPLamp {
  public:
    uint8_t id;
    uint8_t count;
    AsyncWebServer* server;

    ESPLampSettings* settings;
    ESPLampLEDs* leds;
    ESPLampMode* mode;

    ESPLamp(uint16_t ledCount, uint8_t rowCount, uint8_t lampCount=0, uint8_t lampId=0);
    virtual ~ESPLamp();

    void addMode(ESPLampMode* mode);

    void begin();
	void beginHttpServer();
	void beginSocketServer();
	void beginSocketClient();

    void loop();
};

#endif // ESP_LAMP_H
