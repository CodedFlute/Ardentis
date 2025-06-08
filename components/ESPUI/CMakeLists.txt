cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "ESPUI/src/ESPUI.cpp"
                    INCLUDE_DIRS "ESPUI/src/." 
                    REQUIRES 
                        "arduino-esp32" 
                        "arduinojson"
                        "espasyncwebserver"
                        "esp_timer"
                    )
project(ESPUI)