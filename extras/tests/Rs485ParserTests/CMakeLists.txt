add_arduino_executable(rs485_parser_test ${uno_board_id})
target_sketches(rs485_parser_test ${uno_board_id} Rs485ParserTests.ino)
target_link_libraries(rs485_parser_test PRIVATE arduino_unit_lib dcsbios_uno_lib)
