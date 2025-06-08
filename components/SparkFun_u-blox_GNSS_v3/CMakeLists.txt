cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "SparkFun_u-blox_GNSS_v3/src/u-blox_GNSS.cpp" "SparkFun_u-blox_GNSS_v3/src/sfe_bus.cpp"
                    INCLUDE_DIRS "SparkFun_u-blox_GNSS_v3/src/." 
                    PRIV_REQUIRES "arduino-esp32" # Library requires Arduino
                    )
project(SparkFun_u-blox_GNSS_v3)