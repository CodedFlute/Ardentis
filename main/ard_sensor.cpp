#include "ard_sensor.h"

#include "ard_log.h"

const char* sensor_TAG = "Sensors";

ardLog sensor_log(sensor_TAG);

#include <esp_err.h>

#include <esp_timer.h>

#include <Wire.h>

#include <Arduino.h>
#include <stdint.h>

#include <ICM42670P.h>

#include <MS5611.h>

#include <Adafruit_Sensor.h> //Needed for adafruit sensor libraries
#include <Adafruit_H3LIS331.h> // For H3LIS331 High G Accelerometer

static const char* TAG = "Sensors";

void updateArdDataField(ardDataField* field, int16_t val) {
    field->val = val;
    field->timestamp = esp_timer_get_time(); // Timestamp in microseconds
    field->updated = true;
}

ardSensors::ardSensors(gpio_num_t sda, gpio_num_t scl, int i2c, int gps_uart_num, gpio_num_t gps_tx, gpio_num_t gps_rx) {
    gps_uart = HardwareSerial(gps_uart_num); // Initialize the GPS UART serial port

    sda = sda; // Assign the SDA pin
    scl = scl; // Assign the SCL pin

    uart_rx = gps_rx; // Assign the GPS RX pin
    uart_tx = gps_tx; // Assign the GPS TX pin

    const i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = i2c,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .clk_source = I2C_CLK_SRC_DEFAULT,
    };

    esp_err_t ret = i2c_new_master_bus(&i2c_bus_config, &i2c_handle);

    ard_sensor_i2c = TwoWire(i2c); // Initialize I2C with the specified I2C number
}

ardSensors::~ardSensors() {
    icm42670_delete(imu_handle); // Delete the ICM42670 handle
}

esp_err_t ardSensors::initSensors(int LIS331_addr, int imu_adr, int baro_addr, int gps_baud) {
    esp_err_t ret = ESP_OK;

    ard_sensor_i2c.begin(sda, scl);

    gps_uart.begin(gps_baud, SERIAL_8N1, uart_rx, uart_tx);

    if (icm42670_create(i2c_handle, imu_adr, &imu_handle) != 0) {
        sensor_log.warn("ICM42670 initialization failed.");
        ret = ESP_FAIL;
    } else {
        sensor_log.info("ICM42670 initialized successfully.");
    }

    const icm42670_cfg_t imu_cfg = {
        .acce_fs = ACCE_FS_16G,
        .acce_odr = ACCE_ODR_100HZ,
        .gyro_fs = GYRO_FS_2000DPS,
        .gyro_odr = GYRO_ODR_100HZ,
    };

    if (icm42670_config(imu_handle, &imu_cfg) != 0) {
        sensor_log.warn("ICM42670 configuration failed.");
        ret = ESP_FAIL;
    } else {
        sensor_log.info("ICM42670 configured successfully.");
    }

    if (icm42670_acce_set_pwr(imu_handle, ACCE_PWR_LOWNOISE) != 0) {
        sensor_log.warn("Error initializing ICM42670 accelerometer.");
        ret = ESP_FAIL;
    } else {
        sensor_log.info("ICM42670 accelerometer initialized successfully.");
    }

    if (icm42670_gyro_set_pwr(imu_handle, GYRO_PWR_LOWNOISE) != 0) {
        sensor_log.warn("Error initializing ICM42670 gyro.");
        ret = ESP_FAIL;
    } else {
        sensor_log.info("ICM42670 gyro initialized successfully.");
    }

    if (accel.begin_I2C(LIS331_addr, &ard_sensor_i2c) != 0) {
        sensor_log.warn("Could not start H3LIS331 accelerometer.");
        ret = ESP_FAIL;
    } else {
        sensor_log.info("H3LIS331 accelerometer started successfully.");
    }

    accel.setRange(H3LIS331_RANGE_400_G);

    accel.setDataRate(LIS331_DATARATE_100_HZ);

    barometer = MS5611(baro_addr, &ard_sensor_i2c); // MS5611 address
    barometer.begin();

    if (gps.begin(gps_uart) != 0) {
        sensor_log.warn("GPS initialization failed.");
        ret = ESP_FAIL;
    } else {
        sensor_log.info("GPS initialized successfully.");
        gps.setNavigationFrequency(25);
    }

    return ret;
}
// Function to update the sensor data
esp_err_t ardSensors::update() {
    esp_err_t ret = ESP_OK;

    if (esp_timer_get_time() - imu_update_time >= imu_update_interval) {

        icm42670_value_t accel_value;
        if (icm42670_get_acce_value(imu_handle, &accel_value) != 0) {
            sensor_log.warn("Error reading ICM42670 accelerometer.");
            ret = ESP_FAIL;
        } else {
            sensor_log.verb("ICM42670 accelerometer read successfully.");

            updateArdDataField(&accelX, accel_value.x); // Check units when have hardware
            updateArdDataField(&accelY, accel_value.y); // Check units when have hardware
            updateArdDataField(&accelZ, accel_value.z); // Check units when have hardware
        }

        icm42670_value_t gyro_value;
        if (icm42670_get_gyro_value(imu_handle, &gyro_value) != 0) {
            sensor_log.warn("Error reading ICM42670 gyro.");
            ret = ESP_FAIL;
        } else {
            sensor_log.verb("ICM42670 gyro read successfully.");

            updateArdDataField(&gyroX, gyro_value.x); // Check units when have hardware
            updateArdDataField(&gyroY, gyro_value.y); // Check units when have hardware
            updateArdDataField(&gyroZ, gyro_value.z); // Check units when have hardware
        }

        imu_update_time = esp_timer_get_time(); // Timestamp in microseconds
    }

    // Read High G Accelerometer Data
    if (esp_timer_get_time() - high_g_accel_update_time >= high_g_accel_update_interval) {
        sensors_event_t accel_event;
        accel.getEvent(&accel_event);

        updateArdDataField(&highAccelX, accel_event.acceleration.x); // In m/s^2
        updateArdDataField(&highAccelY, accel_event.acceleration.y); // In m/s^2
        updateArdDataField(&highAccelZ, accel_event.acceleration.z); // In m/s^2

        high_g_accel_update_time = esp_timer_get_time(); // Timestamp in microseconds
    }

    if (esp_timer_get_time() - barometer_update_time >= barometer_update_interval) {
        barometer.read(); // Read the barometer data

        updateArdDataField(&pressure, barometer.getPressure() * 100); // Convert to Pa
        updateArdDataField(&temperature, barometer.getTemperature()); // In degrees Celsius

        barometer_update_time = esp_timer_get_time(); // Timestamp in microseconds
    }

    if (esp_timer_get_time() - gps_update_time >= gps_update_interval) {
        if (gps.getPVT() == true) {
            updateArdDataField(&latitude, gps.getLatitude() * 10e7); // Convert to degrees
            updateArdDataField(&longitude, gps.getLongitude() * 10e7); // Convert to degrees
            updateArdDataField(&horizAccuracy, gps.getHorizontalAccuracy() * 1000); // Convert to meters
            updateArdDataField(&vertAccuracy, gps.getVerticalAccuracy() * 1000); // Convert to meters
            updateArdDataField(&gps_altitude, gps.getAltitude() * 1000); // Convert to meters

            gps_update_time = esp_timer_get_time(); // Timestamp in microseconds
        }
    }

    return ret;
} 