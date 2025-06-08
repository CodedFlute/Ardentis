#pragma once

#include <Arduino.h>

struct ArdDataStruct {
    uint16_t pressure;
    int32_t latitude;
    int32_t longitude;
    int16_t horizAccuracy;
    int16_t vertAccuracy;
    int16_t temperature;
    int16_t accelX;
    int16_t accelY;
    int16_t accelZ;
    int16_t highAccelX;
    int16_t highAccelY;
    int16_t highAccelZ;
    int16_t gyroX;
    int16_t gyroY;
    int16_t gyroZ;
    int16_t altitude;
    bool cont1;
    bool cont2;
    bool cont3;
    bool cont4;
    bool fired_1;
    bool fired_2;
    bool fired_3;
    bool fired_4;

    uint16_t getAltitude(int sea_pressure) const {
        // Example calculation for altitude based on pressure
        // Replace with your actual formula
        return ((pow((sea_pressure / pressure), 1/5.257) - 1.0) * (temperature + 273.15)) / 0.0065;
    }
};
