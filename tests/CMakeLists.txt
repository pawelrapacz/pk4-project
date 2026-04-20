add_executable(battleships-tests
    tests_main.cpp
)

target_link_libraries(battleships-tests doctest::doctest)

target_compile_strict(battleships-tests)
doctest_discover_tests(battleships-tests)