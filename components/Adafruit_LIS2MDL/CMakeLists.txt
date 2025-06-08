cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "Adafruit_LIS2MDL/Adafruit_LIS2MDL.cpp"
                    INCLUDE_DIRS "Adafruit_LIS2MDL/."
                    REQUIRES "arduino-esp32" "Adafruit_BusIO" "Adafruit_Sensor"
                    )
project(Adafruit_LIS2MDL)
