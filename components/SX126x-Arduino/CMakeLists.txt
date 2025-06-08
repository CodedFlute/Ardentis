cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "SX126x-Arduino/src/radio/sx126x/sx126x.cpp" "SX126x-Arduino/src/radio/sx126x/radio.cpp" "SX126x-Arduino/src/boards/sx126x/sx126x-board.cpp" "SX126x-Arduino/src/boards/mcu/board.cpp" "SX126x-Arduino/src/boards/mcu/espressif/board.cpp" "SX126x-Arduino/src/boards/mcu/espressif/spi_board.cpp" "SX126x-Arduino/src/boards/mcu/espressif/timer.cpp"
                    INCLUDE_DIRS "SX126x-Arduino/src/." 
                    REQUIRES "arduino-esp32" # Library requires Arduino
                    )
project(SX126x-Arduino)