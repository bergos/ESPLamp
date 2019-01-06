#ifndef ESP_LAMP_COLOR_H_
#define ESP_LAMP_COLOR_H_

#include <Arduino.h>

uint8_t rFromColor(uint32_t color);
uint8_t gFromColor(uint32_t color);
uint8_t bFromColor(uint32_t color);

uint32_t rgbToColor(uint8_t r, uint8_t g, uint8_t b);

uint32_t colorFromCss(String cssColor);

String byteToHex(uint8_t byte);

String colorToCss(uint32_t color);

#endif // ESP_LAMP_COLOR_H_
