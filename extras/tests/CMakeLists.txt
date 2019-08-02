find_arduino_library(arduino_unit_lib ArduinoUnit ${uno_board_id} 3RD_PARTY)
add_arduino_core_lib(arduino_unit_lib ${uno_board_id})

add_subdirectory(LedTests)
add_subdirectory(Rs485ParserTests)
add_subdirectory(Rs485PcParserTests)