# Smart Irrigation System

An Arduino Uno irrigation prototype that monitors soil moisture, rain, and tank water level to control a water pump. A DHT11 reports temperature and humidity, while a 16x2 LCD, status LEDs, and Serial Monitor show system conditions.

The sketch was reconstructed from the supplied **Smart_Irrigation_Code.pdf** explanation and code snippets. It is not a verbatim export of an original `.ino` file. See [source notes](docs/SOURCE_NOTES.md).

## Features

- Automatic watering when soil is dry, rain is absent, and tank water is available.
- Pump shutoff on low tank level, invalid DHT readings, or ultrasonic timeout.
- Temperature and humidity monitoring.
- LCD status messages and detailed Serial Monitor output at 9600 baud.
- Red LED for pump running, blue for normal pump-off states, green for sensor errors.
- Configurable soil threshold and tank dimensions.

## Components and wiring

Use an Arduino Uno, DHT11, analog soil moisture sensor, digital rain sensor, HC-SR04, 16x2 I2C LCD, active-LOW relay module, appropriately powered DC pump, three LEDs with series resistors, jumper wires, and a breadboard.

| Component signal | Arduino Uno pin |
| --- | --- |
| DHT11 DATA | D2 |
| Rain sensor DO | D4 |
| Relay IN | D7 |
| Red LED | D8 |
| Green LED | D9 |
| Blue LED | D10 |
| HC-SR04 ECHO | D12 |
| HC-SR04 TRIG | D13 |
| Soil sensor AO | A0 |
| LCD SDA | A4 |
| LCD SCL | A5 |

Connect sensor/module power and grounds according to each module's ratings. Use a current-limiting resistor for each LED. Power the pump from its own suitable supply through the relay's normally-open contacts, not from an Arduino I/O pin. Verify relay polarity with the pump disconnected first. These signal assignments are not a complete electrical schematic.

## Setup and upload

1. Open `SmartIrrigation/SmartIrrigation.ino` in Arduino IDE. Keep `ControlLogic.h` beside it.
2. Install [DHT sensor library by Adafruit](https://github.com/adafruit/DHT-sensor-library) and its Adafruit Unified Sensor dependency through Library Manager.
3. Install a compatible [LiquidCrystal_I2C library](https://github.com/johnrickman/LiquidCrystal_I2C) exposing `init()`, `backlight()`, and the `(address, columns, rows)` constructor. Libraries with similar names can have different APIs. `Wire` comes with the Arduino board core.
4. Select **Arduino Uno** and the connected serial port.
5. Review the wiring and calibration settings below. Verify the sketch, then upload.
6. Open Serial Monitor at **9600 baud** and run the checks in [TESTING.md](docs/TESTING.md) before connecting the pump.

## Calibration

Edit `SmartIrrigation/ControlLogic.h`:

| Setting | PDF default | Meaning |
| --- | --- | --- |
| `DRY_THRESHOLD` | 500 | Raw analog values above this are dry; 500 is wet |
| `TANK_HEIGHT_CM` | 6.3 | Distance from sensor reference plane to tank bottom |
| `LOW_WATER_CM` | 2.3 | Water depth at or below this stops the pump |

Water depth is `tank height - measured distance`, clamped to zero for distances beyond the tank bottom. Calibrate tank dimensions for your installation and leave adequate sensor clearance above the highest water surface. Confirm the soil sensor's dry/wet direction and choose a threshold from actual readings. Rain detection and relay activation both use LOW. Change the LCD address in the sketch if your module does not use `0x27`.

## Control decisions

| Priority | Condition | Pump | LED |
| --- | --- | --- | --- |
| 1 | DHT invalid or ultrasonic timeout | OFF | Green |
| 2 | Tank low | OFF | Blue |
| 3 | Dry soil and no rain | ON | Red |
| 4 | Rain detected or wet soil | OFF | Blue |

Readings update approximately every two seconds, plus sensor-read time. Valid temperature/humidity values do not set watering thresholds; invalid readings stop the pump.

## Repository structure

```text
SmartIrrigation/
  SmartIrrigation.ino    Arduino hardware integration and display output
  ControlLogic.h        Calibration settings and control decisions
docs/
  SOURCE_NOTES.md       Reconstruction details and scope
  TESTING.md            Local test commands and hardware checklist
tests/
  control_logic_test.cpp
README.md
.gitignore
```

## Validation and limitations

Host C++ tests cover all 16 combinations of error, tank, soil, and rain states, plus threshold and sensor-failure cases. An Arduino board build and physical hardware validation have not been performed in this packaging environment.

This is a prototype: soil/rain disconnection is not detected, readings have no hysteresis, there is no maximum pump runtime, and updates use a blocking delay. Relay startup before `setup()` depends on hardware. Test with the pump disconnected before supervised operation. The original PDF's unused temperature-limit variables were omitted; no temperature-based watering feature is claimed.

## Upload to GitHub

Suggested repository name: **smart-irrigation-system**

Suggested description: **Arduino-based irrigation with soil moisture, rain detection, tank monitoring, and automatic pump control.**

Extract the prepared ZIP and upload its contents to your repository root, including `.gitignore`. Keep `SmartIrrigation` as a folder so the Arduino sketch and header remain together. GitHub displays this README automatically. Upload source files, not only the ZIP.

No credentials, personal details, or original PDF metadata are included. The supplied PDF remains unchanged outside the repository. A software license has not been selected; add one only if you are authorized to license the source.
