add_arduino_executable(led_test ${uno_board_id})
target_sketches(led_test ${uno_board_id} LedTests.ino)
target_link_libraries(led_test PRIVATE arduino_unit_lib dcsbios_uno_lib)
