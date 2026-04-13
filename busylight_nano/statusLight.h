#ifndef STATUS_LIGHT_H
#define STATUS_LIGHT_H

#include <Adafruit_NeoPixel.h>

enum Status
{
  Off = 0,
  Available = 1,
  Caution = 2,
  Busy = 3,
  SuperBusy = 4,
  Fun = 5
};

class StatusLight
{
public:
  StatusLight(
      uint16_t ledCount,
      uint16_t hardwarePin)
  {
    strip = Adafruit_NeoPixel(ledCount, hardwarePin, NEO_GRB + NEO_KHZ800);
    currentMode = Available;
  }

  void initialize()
  {
    strip.begin();
    strip.setBrightness(50);
    strip.show();
  }

  void setMode(
      char cmd)
  {
    if (cmd == '0' || cmd == 0x30) // Off
      setOff();
    if (cmd == '1' || cmd == 0x31) // Available
      setAvailable();
    if (cmd == '2' || cmd == 0x32) // Caution
      setCaution();
    if (cmd == '3' || cmd == 0x33) // Busy
      setBusy();
    if (cmd == '4' || cmd == 0x34) // Klaxon Busy
      setSuperBusy();
    if (cmd == '5' || cmd == 0x35) // Disco Fun
      setFun();
  }

  void setOff()
  {
    currentMode = Off;
  }

  void setAvailable()
  {
    currentMode = Available;
  }

  void setCaution()
  {
    currentMode = Caution;
  }

  void setBusy()
  {
    currentMode = Busy;
  }

  void setSuperBusy()
  {
    currentMode = SuperBusy;
  }

  void setFun()
  {
    currentMode = Fun;
  }

  void Service()
  {
    unsigned long now = millis();
    switch (currentMode)
    {
    case Off:
      offLoop(now);
      break;
    case Available:
      availableLoop(now);
      break;
    case Caution:
      cautionLoop(now);
      break;
    case Busy:
      busyLoop(now);
      break;
    case SuperBusy:
      superBusyLoop(now);
      break;
    case Fun:
    default:
      funLoop(now);
      break;
    }
  }

private:
  void offLoop(
      unsigned long now)
  {
    if (now - lastUpdate >= 500)
    {
      strip.fill(strip.Color(0, 0, 0));
      strip.show();
      lastUpdate = now;
    }
  }

  void availableLoop(
      unsigned long now)
  {
    singleColorLoop(now, strip.Color(0, 255, 0));
  }

  void cautionLoop(
      unsigned long now)
  {
    singleColorLoop(now, strip.Color(255, 255, 0));
  }

  void busyLoop(
      unsigned long now)
  {
    singleColorLoop(now, strip.Color(255, 0, 0));
  }

  void singleColorLoop(
      unsigned long now,
      uint32_t color)
  {
    if (now - lastUpdate >= 500)
    {
      strip.fill(color);
      strip.show();
      lastUpdate = now;
    }
  }

  void superBusyLoop(
      unsigned long now)
  {
    if (now - lastUpdate >= 100)
    {
      ++currentLed;
      if (currentLed >= strip.numPixels())
        currentLed = 1;

      // The circular LED has a middle light and we want to keep that lit
      // the entire time.
      strip.fill(strip.Color(0, 0, 0));
      strip.setPixelColor(currentLed, strip.Color(255, 0, 0));
      strip.setPixelColor(0, strip.Color(255, 0, 0));

      strip.show();
      delay(50);
      lastUpdate = now;
    }
  }

  void funLoop(
      unsigned long now)
  {
    if (now - lastUpdate >= 10)
    {
      for (uint16_t i = 0; i < strip.numPixels(); i++)
      {
        strip.setPixelColor(i, getWheel((i + rainbowStep) & 255));
      }
      strip.show();
      rainbowStep = (rainbowStep + 1) & 255;
      lastUpdate = now;
    }
  }

  uint32_t getWheel(
      byte WheelPos)
  {
    if (WheelPos < 85)
    {
      return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if (WheelPos < 170)
    {
      WheelPos -= 85;
      return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
      WheelPos -= 170;
      return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
  }

  Adafruit_NeoPixel strip;
  Status currentMode;
  uint16_t currentLed = 0;
  unsigned long lastUpdate = 0;
  uint16_t rainbowStep = 0;
};

#endif // STATUS_LIGHT_H
