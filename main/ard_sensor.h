#pragma once

#include <stdint.h>

#include <SparkFun_u-blox_GNSS_v3.h>

#include <Adafruit_Sensor.h> //Needed for adafruit sensor libraries
#include <Adafruit_H3LIS331.h> // For H3LIS331 High G Accelerometer

#include <ICM42670P.h> // For ICM42670P IMU

#include <icm42670.h>

#include <MS5611.h> // For MS5611 Barometer

#include <Wire.h> // For I2C communication

#include <HardwareSerial.h>

#include <driver/i2c_master.h>
/**
 * @brief ardDataField structure to hold sensor data and timestamp
 * @param val Sensor value
 * @param timestamp Timestamp of the last update measured in microseconds since boot
 * @param updated Flag to indicate if the value has been updated
 */
struct ardDataField {
    int16_t val;
    int64_t timestamp;
    bool updated;
};
/**
 * @brief Function to update the ardDataField structure with new sensor data
 * @param field Pointer to the ardDataField structure to be updated
 * @param val New sensor value to be assigned
 */
void updateArdDataField(ardDataField* field, int16_t val);

/**
 * @brief ardSensors class to manage sensor data acquisition and processing
 * @param sda GPIO pin for I2C SDA
 * @param scl GPIO pin for I2C SCL
 * @param i2c I2C bus number
 * @param gps_uart_num UART number for GPS
 * @param gps_tx GPIO pin for GPS TX
 * @param gps_rx GPIO pin for GPS RX
 */
class ardSensors {
public:
    ardSensors(gpio_num_t sda, gpio_num_t scl, int i2c, int gps_uart_num, gpio_num_t gps_tx, gpio_num_t gps_rx);
    ~ardSensors();

    /**
     * @brief Initialize the sensors
     * @param LIS331_addr I2C address for H3LIS331 accelerometer
     * @param imu_adr I2C address for ICM42670 IMU
     * @param baro_addr I2C address for MS5611 barometer
     * @param gps_baud Baud rate for GPS UART
     * @return esp_err_t Error code indicating success or failure of the initialization
     */
    esp_err_t initSensors(int LIS331_addr, int imu_adr, int baro_addr, int gps_baud);
    esp_err_t update(); // Function to check if new sensor data is available and update the data fields accordingly.
    ardDataField accelX; // Accelerometer X-axis data
    ardDataField accelY; // Accelerometer Y-axis data
    ardDataField accelZ; // Accelerometer Z-axis data
    ardDataField gyroX; // Gyroscope X-axis data
    ardDataField gyroY; // Gyroscope Y-axis data
    ardDataField gyroZ; // Gyroscope Y-axis data
    ardDataField highAccelX; // High G Accelerometer X-axis data
    ardDataField highAccelY; // High G Accelerometer Y-axis data
    ardDataField highAccelZ; // High G Accelerometer Z-axis data
    ardDataField pressure; // Pressure data from barometer
    ardDataField altitude; // Altitude data from barometer
    ardDataField temperature; // Temperature data from barometer
    ardDataField latitude; // Latitude data from GPS
    ardDataField longitude; // Longitude data from GPS
    ardDataField horizAccuracy; // Horizontal accuracy from GPS
    ardDataField vertAccuracy; // Vertical accuracy from GPS
    ardDataField gps_altitude; // Altitude data from GPS
private:
    TwoWire ard_sensor_i2c = NULL; // Arduino I2C bus object
    SFE_UBLOX_GNSS_SERIAL gps; // GPS object
    HardwareSerial gps_uart = NULL; // GPS UART bus object
    MS5611 barometer; // Barometer object
    Adafruit_H3LIS331 accel = Adafruit_H3LIS331(); // High G Accelerometer object
    icm42670_handle_t imu_handle = nullptr; // ICM42670 IMU object
    i2c_master_bus_handle_t i2c_handle = NULL; // ESP-IDF I2C bus object
    gpio_num_t sda; // SDA GPIO pin
    gpio_num_t scl; // SCL GPIO pin
    gpio_num_t uart_rx; // GPS RX GPIO pin
    gpio_num_t uart_tx; // GPS TX GPIO pin
    int gps_uart_num; // GPS UART bus number
    int i2c_num; // I2C bus number
    int16_t imu_update_time; // Timestamp of the last IMU update
    int16_t imu_update_interval; // Interval between IMU updates in microseconds
    int16_t high_g_accel_update_time; // Timestamp of the last High G Accelerometer update
    int16_t high_g_accel_update_interval; // Interval between High G Accelerometer updates in microseconds
    int16_t barometer_update_time; // Timestamp of the last Barometer update
    int16_t barometer_update_interval; // Interval between Barometer updates in microseconds
    int16_t gps_update_time; // Timestamp of the last GPS update
    int16_t gps_update_interval; // Interval between GPS updates in microseconds
};