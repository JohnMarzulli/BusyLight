#include <BTstackLib.h>

// Packages
// "Adafruit_NeoPixel" 1.15.4
// "ArduinoBLE" 2.0.0

// Board:
// For the Pico 2 W:
//   Board managers -> https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
//   Install the RP2040 from "Earle Philhower"

// NOTE:
// For the upload to work on a Nano board (Pop_OS!)

// rules need to be setup to allow access.
// https://support.arduino.cc/hc/en-us/articles/9005041052444-Fix-udev-rules-on-Linux#renesas
//
// Need to install udisks
// sudo apt install udisks2

// Deployment script for POP_OS! (Execute from inside git folder)
// cp $(find . -name "*.uf2" 2>/dev/null | head -1) /media/$USER/RP2350/

// Test device is 2C:CF:67:E0:77:3F

// =============================================================================
// Device Testing (Classic Bluetooth SPP)
// Test device MAC: 2C:CF:67:E0:77:3F
// =============================================================================
//
// Step 1 — Cycle Bluetooth (clears stuck adapter state)
//   bluetoothctl power off
//   bluetoothctl power on
//
// Step 2 — Pair and trust (first time only)
//   bluetoothctl
//     scan on
//     # Wait for "BusyLight" to appear
//     scan off
//     pair 2C:CF:67:E0:77:3F
//     trust 2C:CF:67:E0:77:3F
//     quit
//
// Step 3 — Bind the RFCOMM serial port (channel 1 = SPP default)
//   sudo rfcomm unbind /dev/rfcomm0
//   sudo rfcomm bind /dev/rfcomm0 2C:CF:67:E0:77:3F 1
//
// Step 4 — Connect via bluetoothctl to establish the link
//   bluetoothctl connect 2C:CF:67:E0:77:3F
//
// Step 5 — Send a command
//   sudo bash -c 'echo "1" > /dev/rfcomm0'
//
// Troubleshooting:
//   "Cannot allocate memory" on echo:
//     The rfcomm node exists but the connection was never established.
//     Run Step 4 (bluetoothctl connect) before Step 5.
//     Or verify the correct channel: sdptool browse 2C:CF:67:E0:77:3F
//       Look for "Serial Port" / RFCOMM entry and use that channel number.
//   Release a stuck binding:  sudo rfcomm release /dev/rfcomm0
//   Restart BlueZ:            sudo systemctl restart bluetooth
//   Verify the port is open:  sudo screen /dev/rfcomm0 9600  (Ctrl+A then K to exit)
// =============================================================================

#include <SerialBT.h>

#include "hardwareConfig.h"
#include "statusLight.h"

StatusLight statusLight = StatusLight(LED_COUNT, PIN);

uint8_t currentMode = 0;

void setup() {
  Serial.begin(9600);

  statusLight.initialize();

  SerialBT.setName("BusyLight");
  SerialBT.begin();
  Serial.println("BT advertising...");
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    statusLight.setMode(cmd);
  }

  statusLight.Service();
}