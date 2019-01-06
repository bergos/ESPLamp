#include "ESPLampColor.h"

uint8_t rFromColor(uint32_t color) {
  return (color >> 16) & 0xff;
}

uint8_t gFromColor(uint32_t color) {
  return (color >> 8) & 0xff;
}

uint8_t bFromColor(uint32_t color) {
  return color & 0xff;
}

uint32_t rgbToColor(uint8_t r, uint8_t g, uint8_t b) {
  return (r << 16) | (g << 8) | b;
}

uint32_t colorFromCss(String cssColor) {
  uint8_t r = strtol(cssColor.substring(1, 3).c_str(), 0, 16);
  uint8_t g = strtol(cssColor.substring(3, 5).c_str(), 0, 16);
  uint8_t b = strtol(cssColor.substring(5, 7).c_str(), 0, 16);

  return rgbToColor(r, g, b);
}

String byteToHex(uint8_t byte) {
  String hex = String(byte, HEX);

  if (hex.length() == 1) {
    hex = String('0') + hex;
  }

  return hex;
}

String colorToCss(uint32_t color) {
  uint8_t r = rFromColor(color);
  uint8_t g = gFromColor(color);
  uint8_t b = bFromColor(color);

  String cssColor = "#000000";

  return String('#') + byteToHex(r) + byteToHex(g) + byteToHex(b);
}
