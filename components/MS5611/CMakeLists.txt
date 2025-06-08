cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "MS5611/MS5611.cpp"
                    INCLUDE_DIRS "MS5611/." 
                    REQUIRES "arduino-esp32" # Library requires Arduino
                    )
project(MS5611)