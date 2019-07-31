cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(PROJECT_NAME DcsBios)
project(${PROJECT_NAME})

set(DcsBios_RECURSE True)
#link_directories(..)

add_subdirectory(src)
add_subdirectory(examples)