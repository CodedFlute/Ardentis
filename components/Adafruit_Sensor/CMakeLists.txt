cmake_minimum_required(VERSION 3.16)
idf_component_register(SRCS "Adafruit_Sensor/Adafruit_Sensor.cpp"
                    INCLUDE_DIRS "Adafruit_Sensor/."
                    REQUIRES "arduino-esp32"
                    )
project(Adafruit_Sensor)