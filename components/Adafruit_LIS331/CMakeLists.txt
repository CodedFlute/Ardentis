cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "Adafruit_LIS331/Adafruit_LIS331.cpp" "Adafruit_LIS331/Adafruit_H3LIS331.cpp" "Adafruit_LIS331/Adafruit_LIS331HH.cpp"
                    INCLUDE_DIRS "Adafruit_LIS331/."
                    REQUIRES "arduino-esp32" "Adafruit_BusIO" "Adafruit_Sensor"
                    )
project(Adafruit_LIS331)
