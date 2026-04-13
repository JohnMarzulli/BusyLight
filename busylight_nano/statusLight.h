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
    currentMode = Available;
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
    unsigned long now = millis();
    switch (currentMode) {
      case Busy:      busyLoop(now);      break;
      case Available: availableLoop(now); break;
      case Off:       offLoop(now);       break;
      case Fun:
      default:        funLoop(now);       break;
    }
  }

private:
  void offLoop(unsigned long now) {
    if (now - lastUpdate >= 500) {
      strip.fill(strip.Color(0, 0, 0));
      strip.show();
      lastUpdate = now;
    }
  }

  void busyLoop(unsigned long now) {
    if (now - lastUpdate >= 500) {
      strip.fill(strip.Color(255, 0, 0));
      strip.show();
      lastUpdate = now;
    }
  }

  void availableLoop(unsigned long now) {
    if (now - lastUpdate >= 500) {
      strip.fill(strip.Color(0, 255, 0));
      strip.show();
      lastUpdate = now;
    }
  }

  void funLoop(unsigned long now) {
    if (now - lastUpdate >= 10) {
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, getWheel((i + rainbowStep) & 255));
      }
      strip.show();
      rainbowStep = (rainbowStep + 1) & 255;
      lastUpdate = now;
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
  unsigned long lastUpdate = 0;
  uint16_t rainbowStep = 0;
};

#endif  // STATUS_LIGHT_H
