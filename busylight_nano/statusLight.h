#ifndef STATUS_LIGHT_H
#define STATUS_LIGHT_H

#include <Adafruit_NeoPixel.h>

enum Status {
  Off = 0,
  Available,
  Busy,
  Fun
};

class StatusLight {
public:
  StatusLight(
    uint16_t ledCount,
    uint16_t hardwarePin) {
    strip = Adafruit_NeoPixel(ledCount, hardwarePin, NEO_GRB + NEO_KHZ800);
    currentMode = Fun;
  }

  void initialize() {
    strip.begin();
    strip.setBrightness(50);
    strip.show();
  }

  void setMode(
    char cmd) {
    if (cmd == '0') setOff();
    if (cmd == '1') setAvailable();
    if (cmd == '2') setBusy();
    if (cmd == '3') setFun();
  }

  void setOff() {
    currentMode = Off;
  }

  void setBusy() {
    currentMode = Busy;
  }

  void setAvailable() {
    currentMode = Available;
  }

  void setFun() {
    currentMode = Fun;
  }

  void Service() {
    switch (currentMode) {
      case Busy: busyLoop(); break;
      case Available: availableLoop(); break;
      case Off: offLoop(); break;
      case Fun:
      default: funLoop(); break;
    }
  }

private:
  void offLoop() {
    strip.fill(strip.Color(0, 0, 0));
    strip.show();
    delay(500);
  }

  void busyLoop() {
    strip.fill(strip.Color(255, 0, 0));
    strip.show();
    delay(500);
  }

  void availableLoop() {
    strip.fill(strip.Color(0, 255, 0));
    strip.show();
    delay(500);
  }

  void funLoop() {
    rainbow(10);
  }

  void rainbow(uint8_t wait) {
    uint16_t i, j;
    for (j = 0; j < 256; j++) {
      for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, getWheel((i + j) & 255));
      }
      strip.show();
      delay(wait);
    }
  }

  void rainbowCycle(uint8_t wait) {
    uint16_t i, j;
    for (j = 0; j < 256 * 5; j++) {
      for (i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, getWheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();
      delay(wait);
    }
  }

  uint32_t getWheel(byte WheelPos) {
    if (WheelPos < 85) {
      return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if (WheelPos < 170) {
      WheelPos -= 85;
      return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
      WheelPos -= 170;
      return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
  }


  Adafruit_NeoPixel strip;
  Status currentMode;
};

#endif  // STATUS_LIGHT_H