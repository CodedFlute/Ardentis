# Ardentis

> **⚠ WARNING:** This is currently a work in progress and untested. Only fly if you are willing and able to take full responsibility for any damage or property loss that may result from failure of this flight computer to operate as designed.

Ardentis is the best value high-performance flight computer designed specifically for high-power rocketry at all levels from 38mm min diameter to 132mm dual stage space shots.

This repository contains both the hardware and software required for Ardentis and its upgrades.

## Features

Ardentis comes in one default configurations, although any combination of features could be created by due to its open source design philosophy.

| Feature | Capability |
|-|-|
| Max Altitude | Baro: 25 km (85k ft) GPS: 80 km (250k ft) |
| GPS Tracking | Yes |
| Max Radio Range | ~15 km |
| Max Acceleration | 400G |
| Outputs | 4 (Servo + Air Start Capable) |
| Data Logging | MicroSD |
| Wi-Fi | Configuration and Control |

### Ardentis

<details>
<summary>Hardware Details</summary>

- **MCU**: ESP32-S3-MINI-1-N4R2
  - 4 MB Flash
  - 384 KB ROM
  - 512 KB SRAM
  - Dual core Xtense 32 bit LX7 at up to 240 MHz
- **Radio**: seeed studio SX126x Breakout
  - 22 dBm TX Power
  - 915 MHz ISM Band
  - u.Fl Antenna Connector
- **IMU**: ICM
  - +/- 16G Accelerometer
  - +/- 2000 Deg/s Gyroscope
- **High G Accelerometer**:
  - 3 Axis
  - +/- 100, 200, 400G Ranges
- **Barometer**: MS5611
  - 10 cm Resolution
  - 10 mBar to 1 200 mBar Operating Range
- **GPS**: uBlox SAM-M10Q
  - 80 km (250k ft) Max Altitude
  - 500 m/s (1.45 Mach) Max Velocity
  - 4G Max Acceleration
- **Ignition Channels**4
- **Connectivity**: Bluetooth 5.0, Wi-Fi, Micro USB
- **Power**: 1S LiPo battery support (or anything between 2V and 5V)

</details>
