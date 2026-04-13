// Packages
// "Adafruit_NeoPixel" 1.15.4
// BTstackLib is built into the Earle Philhower RP2040 core (no separate install)

// Board:
// For the Pico 2 W:
//   Board managers -> https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
//   Install the RP2040 from "Earle Philhower"
//
// IMPORTANT — Arduino IDE Tools menu settings:
//   IP/Bluetooth Stack: "IPv4 + Bluetooth"   (enables BTstack BLE)

// Deployment script for POP_OS! (Execute from inside git folder)
// cp $(find . -name "*.uf2" 2>/dev/null | head -1) /media/$USER/RP2350/

// =============================================================================
// Device Testing (BLE UART / Nordic UART Service)
// =============================================================================
//
// iOS: Use "nRF Toolbox" app -> UART -> connect to "BusyLight" -> send 0/1/2/3
//
// Linux — send via gatttool:
//   sudo apt install bluez-tools
//   hcitool lescan                         # find MAC
//   gatttool -b <MAC> -I
//     > connect
//     > primary                            # find NUS service handle
//     > characteristics                    # find RX characteristic handle
//     > char-write-req <handle> 31         # 0x31 = '1' (Available)
//
// Commands: '0'=Off  '1'=Available (green)  '2'=Busy (red)  '3'=Fun (rainbow)
// =============================================================================

#include <BTstackLib.h>

#include "hardwareConfig.h"
#include "statusLight.h"

// Nordic UART Service (NUS) UUIDs
static const char* NUS_SERVICE_UUID = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
static const char* NUS_RX_UUID      = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";  // phone writes here
static const char* NUS_TX_UUID      = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";  // device notifies here

StatusLight statusLight = StatusLight(LED_COUNT, PIN);

// Callback signature required by BTstackLib: int fn(uint16_t id, uint8_t* buf, uint16_t len)
// BTstack passes the ATT handle, not a custom ID — only one writable characteristic exists so no check needed.
int gattWriteCallback(uint16_t characteristic_id, uint8_t* buffer, uint16_t buffer_size) {
  if (buffer_size > 0) {
    statusLight.setMode((char)buffer[0]);
  }
  return 0;
}

void setup() {
  Serial.begin(9600);

  statusLight.initialize();

  BTstack.setGATTCharacteristicWrite(gattWriteCallback);
  BTstack.setup("BusyLight");

  BTstack.addGATTService(new UUID(NUS_SERVICE_UUID));
  BTstack.addGATTCharacteristicDynamic(new UUID(NUS_RX_UUID), ATT_PROPERTY_WRITE_WITHOUT_RESPONSE | ATT_PROPERTY_WRITE, 0);
  BTstack.addGATTCharacteristicDynamic(new UUID(NUS_TX_UUID), ATT_PROPERTY_READ | ATT_PROPERTY_NOTIFY, 0);

  BTstack.startAdvertising();
  Serial.println("BLE advertising...");
}

void loop() {
  BTstack.loop();
  statusLight.Service();
}
