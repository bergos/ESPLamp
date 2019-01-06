#ifndef MODE_SOLID_H
#define MODE_SOLID_H

#include <ESPLamp.h>

class ModeSolid : public ESPLampMode {
  public:
    ModeSolid();

    virtual void begin(ESPLamp* lamp);

    virtual void loop();

  protected:
    ESPLamp* lamp;
};

#endif // _CLOUD_MODE_SOLID_H
