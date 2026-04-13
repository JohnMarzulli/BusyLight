# BusyLight

## Summary

This is a project to provide a visible status light for home offices.

It uses standard light colors:

| Color        | Status             |
| ------------ | ------------------ |
| Green        | Available          |
| Yellow       | Caution            |
| Red          | Unavailable        |
| Rotating Red | REALLY unavailable |
| Rainbow      | Send Coffee        |

## Use

The device uses Bluetooth Low Energy (BLE) mode. You may use the provided Python script, Linux commands, OR use "NRF Toolbox" to set the color of the status light.

| Command | Result             |
| ------- | ------------------ |
| 0       | Light OFF          |
| 1       | Light GREEN        |
| 2       | Light YELLOW       |
| 3       | Light RED          |
| 4       | Light ROTATING RED |
| 5       | RAINBOW            |

## Components

A unit may be put together using the following components:

| Item                          | Link                                                                          |
| ----------------------------- | ----------------------------------------------------------------------------- |
| Raspberry Pi Pico 2 W (or WH) | <https://www.amazon.com/dp/B0F4W9J5CC/ref=dp_iou_view_item?ie=UTF8&th=1>      |
| 1100 mAh LiPo w/ JST 1.25     | <https://www.amazon.com/dp/B0867KDMY7?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1> |
| USB-C Charger Module          | <https://www.amazon.com/dp/B07PKND8KG?ref=ppx_yo2ov_dt_b_fed_asin_title>      |
| JST 1.25 Female Plugs         | <https://www.amazon.com/dp/B0B2DBTVJC?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1> |
| On/Off Switch                 | <https://www.amazon.com/dp/B07RTJDW27?ref=ppx_yo2ov_dt_b_fed_asin_title>      |
| Circular WS2812B LEDs         | <https://www.amazon.com/dp/B0B2D6JDVJ?ref=ppx_yo2ov_dt_b_fed_asin_title&th=1> |

Soldering is required, but it you purchase the "WH" version, jumper cables may be used for most of the wiring.

The housing may be obtained from MakerWorld. It was designed with PolyMaker PolyTerra Charcoal as the main color, and for "clear" PLA as the light difusser.

## Wiring

### LED

| Wire Color | LED     | Destination  |
| ---------- | ------- | ------------ |
| Red        | Power5V | Pi Pico VSYS |
| Orange     | DIN     | Pi Pico GP 4 |
| Brown      | GND     | Pi Pico GND  |

### Switch

| Wire Color | Start              | Destination        |
| ---------- | ------------------ | ------------------ |
| Red        | USB-C Out +        | Switch, Outter Pin |
| Red        | Switch, Middle Pin | Pi Pico VBUS       |

Use a multi-meter to determine the correct pins. With the pin in the "ON" position the middle pin and one of the outter pins will complete a circuit.

### USB-C Charger

| Wire Color                     | USB-C Charger | Destination                |
| ------------------------------ | ------------- | -------------------------- |
| Red                            | B+            | JST Female Plug Red Wire   |
| Black                          | B-            | JST Female Plug Black Wire |
| Brown                          | Out-          | Pi Pico GND                |
| (Covered in Switch Pinout) Red | USB-C Out +   | Switch, Outter Pin         |

### Final Assembly

Before placing all of the components into the printed housing, you will need to flash the board.

Please note that the Pi Pico 2 W is very picky about its USB-B plugs. You may need to try many cables before finding one.

[BusyLight Enclosure](https://makerworld.com/en/models/2654853-busylight#profileId-2935510)

To flash the Pi Pico W:

1. Plug the USB-A side of the cable into your computer.
1. Press and hold the "BOOTSEL" button.
1. Plug the USB-B side into the Raspberry Pi. Release the button.
1. You may now copy the .uf2 file to the RP2350 drive to flash the unit.

You will need Arduino IDE to perform the build. The .ino file will contain the list of libraries you will need to install to complete the build.

Once the unit is flashed, you will see the green status light turn on. After unplugging the USB cable, you may plug in the LiPo's JST cable to the female end.
