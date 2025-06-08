cmake_minimum_required(VERSION 3.5)

file(GLOB_RECURSE RADIOLIB_ESP_SOURCES
    "src/*.*"
  )

idf_component_register(SRCS "RadioLib/src/Hal.cpp" "RadioLib/src/Module.cpp" "RadioLib/src/hal/Arduino/ArduinoHal.cpp" "RadioLib/src/modules/SX126x/SX126x.cpp" "RadioLib/src/modules/SX126x/SX1262.cpp" "RadioLib/src/modules/SX126x/SX126x_LR_FHSS.cpp" "RadioLib/src/protocols/PhysicalLayer/PhysicalLayer.cpp" "RadioLib/src/utils/FEC.cpp" "RadioLib/src/utils/CRC.cpp" "RadioLib/src/utils/Utils.cpp"
                    INCLUDE_DIRS "RadioLib/src/." 
                    REQUIRES "arduino-esp32" # Library requires Arduino
                    )
project(RadioLib)