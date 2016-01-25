cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME DcsBios)
project(${PROJECT_NAME})

set(DcsBios_RECURSE True)
link_directories(..)

set(MasterCaution_SKETCH examples/MasterCaution/MasterCaution.ino)
generate_arduino_firmware(
        MasterCaution
)

set(MasterCautionRs485_SKETCH examples/MasterCautionRs485/MasterCautionRs485.ino)
generate_arduino_firmware(
        MasterCautionRs485
)

set(Rs485BusController_SKETCH examples/Rs485BusController/Rs485BusController.ino)
generate_arduino_firmware(
        Rs485BusController
        BOARD leonardo
)