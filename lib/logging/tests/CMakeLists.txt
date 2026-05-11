add_executable(logging-tests
    tests.h
    tests_main.cpp
    logging_test.cpp
    styles_test.cpp
    adapter_test.cpp
)

target_link_libraries(logging-tests doctest::doctest logging)

if (LOGGING_RAYLIB_ADAPTER)
    target_link_libraries(logging-tests logging_raylib)
endif()

if (LOGGING_LLAMA_ADAPTER)
    target_link_libraries(logging-tests logging_llama)
endif()

target_compile_strict(logging-tests)
doctest_discover_tests(logging-tests)