add_executable(logging-tests 
    tests_main.cpp
    logging_test.cpp
    styles_test.cpp
)

target_link_libraries(logging-tests doctest::doctest logging)

target_compile_definitions(logging-tests PRIVATE LOGGING_ALWAYS_USE_DEBUG)
target_compile_strict(logging-tests)
doctest_discover_tests(logging-tests)