#pragma once
/**
 * @file ard_deployment.h
 * @author Roland Neill
 * @brief Header file for the ard_deployment class.
 * @version 0.1
 * @date 2025-04-25
 * 
 * @copyright Copyright (c) 2025
 * 
 * @details This file contains the definition of the ard_deployment class, which is used to manage GPIO pins for deployment purposes.
 * 
 */

#include <driver/gpio.h>

#include <esp_err.h>
#include <esp_log.h>

/**
 * @struct ardDeployGpioConfig
 * @brief Configuration structure for GPIO pins used in deployment.
 * 
 * @var fire Array of 4 GPIO numbers for fire outputs.
 * @var cont Array of 4 GPIO numbers for continuity detection inputs.
 */
struct ardDeployGpioConfig {
    gpio_num_t fire[4];
    gpio_num_t cont[4];
};

/**
 * @enum FirePins
 * @brief Enum containing the valid fire indices.
 * 
 */
enum class FirePins : uint8_t {
    Zero = 0,
    One = 1,
    Two = 2,
    Three = 3
};

/**
 * @class ardOutputGPIO
 * @brief Class to manage GPIO pins for deployment.
 * @details This class provides methods to initialize GPIO pins, set their modes, and read/write values.
 */
class ardOutputGPIO {
public:
    /**
     * @brief Construct a new ardOutputGPIO object
     * 
     * @param config Configuration for GPIO pins used in deployment
     */
    ardOutputGPIO(ardDeployGpioConfig config) {
        config = config;
        for (int i = 0; i < 4; i++) {
            gpio_set_direction(config.fire[i], GPIO_MODE_OUTPUT); // Set fire pins as output
            gpio_set_level(config.fire[i], 0); // Initialize fire pins to low
            gpio_set_direction(config.cont[i], GPIO_MODE_INPUT);  // Set continuity pins as input
            gpio_set_pull_mode(config.cont[i], GPIO_PULLUP_ONLY); // Enable pull-up resistors on continuity pins
        }
    }
    /**
     * @brief Fire the specified pin.
     * 
     * @param pin The output number of the pin to fire.
     * @return esp_err_t Whether the operation succeeded or failed.
     */
    esp_err_t fire(FirePins pin);

    /**
     * @brief Check the continuity of the outputs
     * 
     * @return std::array<bool, 4> A array with the continuity status of each output
     * @details The array will contain true if there is continuity at the output, and false if there is not.
     */
    std::array<bool, 4> checkContinuity();

    /**
     * @brief Check which pins have been fired.
     * 
     * @return std::array<bool, 4> A array with the staus of each output
     * @details The array will contain true if the pin has been fired since the instance was initialized, and false if it has not.
     */
    std::array<bool, 4> fireStatus();
private:
    ardDeployGpioConfig config;
};