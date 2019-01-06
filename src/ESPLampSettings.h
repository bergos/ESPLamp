#ifndef ESP_LAMP_SETTINGS_H
#define ESP_LAMP_SETTINGS_H

#include <Arduino.h>
#include <StringArray.h>

class AsyncClient;
class AsyncServer;
class AsyncWebServerRequest;
class ESPLamp;
class ESPLampMode;
class WiFiClient;

class ESPLampSettings {
  public:
    uint32_t primaryColor = 0xffffff;
    uint32_t secondaryColor = 0x000000;
    String modeName;

    ESPLampSettings();

    void addMode(ESPLampMode* mode);

    void begin(ESPLamp* lamp);
    void beginHttpServer();
    void beginSocketServer();
    void beginSocketClient();

    void loop();

    bool fromJson(const char* content, size_t len);

    char* toJson();

    void get(AsyncWebServerRequest* request);

    void put(AsyncWebServerRequest* request, uint8_t* data, size_t len);

  protected:
    ESPLamp* lamp;
    LinkedList<ESPLampMode*> modes;

    // server
    AsyncServer* server;
    LinkedList<AsyncClient*> clients;

    // client
    WiFiClient* client;
    uint32_t clientConnected;

    void loopSocketClient();

    void updateMode();
};

#endif // ESP_LAMP_SETTINGS_H
