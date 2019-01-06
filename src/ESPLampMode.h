#ifndef ESP_LAMP_MODE_H
#define ESP_LAMP_MODE_H

class CloudLampMode {
  public:
    String name;

    virtual void begin(ESPLamp* lamp) = 0;

    virtual void loop() = 0;
};

#endif // ESP_LAMP_MODE_H
