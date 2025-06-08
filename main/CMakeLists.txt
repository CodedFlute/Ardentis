
idf_component_register(SRCS "ard_ui.cpp" "ard_deployment.cpp" "ard_radio.cpp" "main.cpp" "ard_sensor.cpp" "ard_log.cpp"
                    INCLUDE_DIRS "." "include/."
                    REQUIRES "ICM42670P" "MS5611" "ESPUI" "Adafruit_LIS331" "Adafruit_LIS2MDL" "SX126x-Arduino" "SparkFun_u-blox_GNSS_v3" "Adafruit_Sensor" "RadioLib" "icm42670"
                    )
