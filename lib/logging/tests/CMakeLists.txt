add_executable(logging-tests 
    tests_main.cpp
    logging_test.cpp
    styles_test.cpp
)

target_link_libraries(logging-tests doctest logging)
# target_compile_strict(logging-tests) # currently doctest fails
add_test(NAME LoggingTest COMMAND logging-tests)
