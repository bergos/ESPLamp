#include <ArduinoJson.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "ESPLamp.h"
#include "ESPLampSettings.h"
#include "ESPLampColor.h"

ESPLampSettings::ESPLampSettings() :
  modes(LinkedList<ESPLampMode*>([](ESPLampMode* m){})),
  clients(LinkedList<AsyncClient*>([](AsyncClient* c){}))
{
  server = 0;
  client = 0;
}

void ESPLampSettings::addMode(ESPLampMode* mode) {
  modes.add(mode);
}

void ESPLampSettings::begin(ESPLamp* lamp) {
  this->lamp = lamp;

  ESPLampMode* const* mode = modes.nth(0);

  if (mode) {
    this->modeName = (*mode)->name;
  }

  updateMode();
}

void ESPLampSettings::beginHttpServer() {
  lamp->server->on("/settings", HTTP_GET, [this](AsyncWebServerRequest* request) {
    Serial.println("GET /settings");

    get(request);
  });

  lamp->server->on("/settings", HTTP_PUT, [](AsyncWebServerRequest* request) {
  }, [](AsyncWebServerRequest* request, String filename, size_t index, uint8_t* data, size_t len, bool final) {
  }, [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
    Serial.println("PUT /settings");

    put(request, data, len);
  });
}

void ESPLampSettings::beginSocketServer() {
  server = new AsyncServer(30000);

  server->onClient([this](void* s, AsyncClient* client) {
    if(client == 0) {
      return;
    }

    clients.add(client);

    char* content = toJson();

    client->write(content, strlen(content) + 1);

    delete[] content;

    client->onDisconnect([this](void* s, AsyncClient* client) {
      Serial.println("disconnect");

      clients.remove(client);
    });
  }, this);

  server->begin();
}

void ESPLampSettings::beginSocketClient() {
  client = new WiFiClient();
}

void ESPLampSettings::loopSocketClient() {
  if (!client->connected()) {
    if (client->status() == CLOSED) {
      Serial.println("socket client: connecting");

      clientConnected = millis();
      client->connect(WiFi.gatewayIP(), 30000);
	} else {
      Serial.println(".");
	}
  } else if (client->available()) {
    String content = client->readStringUntil(0);

	fromJson(content.c_str(), content.length());
  }

  if (millis() - clientConnected > 10000) {
    Serial.println("socket client: auto reconnect");

	delete client;
	client = new WiFiClient();
  }
}

void ESPLampSettings::loop() {
  if (client != 0) {
    loopSocketClient();
  }
}

bool ESPLampSettings::fromJson(const char* content, size_t len) {
  Serial.print("settings: ");
  Serial.println(content);

  DynamicJsonBuffer buffer;
  JsonObject& json = buffer.parseObject(content);

  if (!json.success()) {
    Serial.println("settings: invalid JSON object");

    return false;
  }

  modeName = json.get<String>("mode");
  primaryColor = colorFromCss(json["primaryColor"]);
  secondaryColor = colorFromCss(json["secondaryColor"]);

  updateMode();

  return true;
}

char* ESPLampSettings::toJson() {
  DynamicJsonBuffer buffer;
  JsonObject& json = buffer.createObject();

  json["mode"] = modeName;
  json["primaryColor"] = colorToCss(primaryColor);
  json["secondaryColor"] = colorToCss(secondaryColor);

  size_t length = json.measureLength() + 1;
  char* content = new char[length];

  json.printTo(content, length);

  return content;
}

void ESPLampSettings::get(AsyncWebServerRequest* request) {
  char* content = toJson();

  AsyncWebServerResponse* response = request->beginResponse(200, "application/json", content);
  request->send(response);

  delete[] content;
}

void ESPLampSettings::put(AsyncWebServerRequest* request, uint8_t* data, size_t len) {
  if (fromJson((char*)data, len)) {
    if (request != 0) {
      get(request);
    }
  } else {
    if (request != 0) {
      AsyncWebServerResponse* response = request->beginResponse(400);
      request->send(response);
    }
  }
}

void ESPLampSettings::updateMode() {
  ESPLampMode* mode = 0;

  for(ESPLampMode* m : modes) {
	if (m->name == modeName) {
	  mode = m;
	  break;
	}
  }

  if (!mode) {
    return;
  }

  lamp->mode = mode;
  lamp->mode->begin(lamp);

  if (lamp->id == 0) {
    char* content = toJson();

    int i=0;

    for(AsyncClient* client : clients) {
      client->write(content, strlen(content) + 1);
      Serial.println(i);
      i++;
    }

    delete[] content;
  }
}
