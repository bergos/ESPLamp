#include <ESPLamp.h>
#include <ESPLampColor.h>
#include <ESPLampLEDs.h>
#include <ESPLampSettings.h>

#include "ModeSolid.h"

ModeSolid::ModeSolid() {
  name = "solid";
}

void ModeSolid::begin(ESPLamp* lamp) {
  this->lamp = lamp;
}

void ModeSolid::loop() {
  for (uint16_t i=0; i<lamp->leds->ledCount; i++) {
    lamp->leds->setPixel(i, lamp->settings->primaryColor);
  }

  lamp->leds->show();
}
