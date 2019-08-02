cmake_minimum_required(VERSION 3.13.1)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/Arduino-Toolchain.cmake)
set(ARDUINO_CMAKE_SKETCHBOOK_PATH ${CMAKE_SOURCE_DIR}/../..)

project(DcsBiosLibrary)

get_board_id(uno_board_id uno)
get_board_id(leonardo_board_id leonardo)

set(dcsbios_sources
        src/DcsBios.h

        src/controls/ActionButton.cpp
        src/controls/DimmableLed.cpp
        src/controls/Led.cpp
        src/controls/PollingSwitch.cpp
        src/controls/Potentiometer.cpp
        src/controls/Stepper.cpp
        src/controls/Switch2Pos.cpp
        src/controls/Switch3Pos.cpp
        src/controls/SwitchMultiPos.cpp

        src/dcs/ExportStreamListener.cpp
        src/dcs/ExportStreamParser.cpp
        src/dcs/Input.cpp
        src/dcs/IntegerListener.cpp
        src/dcs/PollingInput.cpp

        src/hal/AcceleratedStepperOutput.cpp
        src/hal/DirectAnalogInput.cpp
        src/hal/DirectAnalogOuptut.cpp
        src/hal/DirectInputPin.cpp
        src/hal/DirectOutputPin.cpp
        src/hal/DirectStepperDriver.cpp
        src/hal/StepperOutput.cpp

        src/pc/DcsBiosRs485BusParser.cpp
        src/pc/DcsBiosRs485Controller.cpp
        src/pc/DcsBiosRs485Device.cpp
        src/pc/DcsBiosRs485PcParser.cpp
        src/pc/DcsBiosSerialDevice.cpp
)

add_arduino_library(dcsbios_uno_lib ${uno_board_id} ${dcsbios_sources})
add_arduino_core_lib(dcsbios_uno_lib ${uno_board_id})

add_arduino_library(dcsbios_leonardo_lib ${leonardo_board_id} ${dcsbios_sources})
add_arduino_core_lib(dcsbios_leonardo_lib ${leonardo_board_id})
target_compile_options(leonardo_core_lib PUBLIC -DUSB_VID=0x2341 -DUSB_PID=0x8036 -DUSB_MANUFACTURER="Unknown" -DUSB_PRODUCT="Arduino Leonardo")

add_subdirectory(extras)
add_subdirectory(examples)