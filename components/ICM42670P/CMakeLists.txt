cmake_minimum_required(VERSION 3.5)
idf_component_register(SRCS "ICM42670P/src/ICM42670P.cpp" "ICM42670P/src/inv_time.c" "ICM42670P/src/imu/inv_imu_apex.c" "ICM42670P/src/imu/inv_imu_driver.c" "ICM42670P/src/imu/inv_imu_selftest.c" "ICM42670P/src/imu/inv_imu_transport.c"
                    INCLUDE_DIRS "ICM42670P/src/." 
                    REQUIRES "arduino-esp32" # Library requires Arduino
                    )
project(ICM42670P)