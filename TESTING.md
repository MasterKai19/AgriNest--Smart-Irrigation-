# Testing

## Host logic tests

Run from the repository root with a C++11-compatible compiler:

```sh
g++ -std=c++11 -Wall -Wextra -pedantic tests/control_logic_test.cpp -o control_logic_test
./control_logic_test
```

On Windows use `-o control_logic_test.exe`, then run `.\control_logic_test.exe`.

Tests exercise the same control function used by the sketch. They cover 16 decision combinations, a missing ultrasonic echo, invalid humidity, distances beyond the tank bottom, dry/wet boundary values, and echo durations on either side of the low-water cutoff. These tests do not simulate the Arduino hardware or validate library compatibility.

## Hardware acceptance checks

First verify and upload with Arduino IDE using the actual libraries. Start with the pump disconnected and monitor relay output, LCD, and LEDs.

| Check | Expected outcome |
| --- | --- |
| Startup | Relay commanded OFF before sensor initialization |
| Dry soil, no rain, water available | Pump command ON, red LED |
| Wet soil with water available | Pump command OFF, blue LED |
| Rain with dry soil | Pump command OFF, blue LED, Dry + Rain |
| Low tank regardless of soil | Pump command OFF, blue LED, TANK LOW |
| DHT reading failure | Pump command OFF, green LED, SYSTEM ERROR |
| No ultrasonic echo | Pump command OFF, green LED, SYSTEM ERROR |
| Sensor readings recover | Control resumes from current conditions |

Validate water depth against a physical measurement, calibrate dry/wet readings, and verify LCD address and relay polarity. Only connect a suitably powered pump after these checks pass. Pin-state reporting confirms software output, not physical pump operation.
