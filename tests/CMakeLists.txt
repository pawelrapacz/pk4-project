add_executable(battleships-tests tests_main.cpp)
target_link_libraries(battleships-tests doctest)
target_compile_strict(battleships-tests)
add_test(NAME BattleshipsTests COMMAND battleships-tests)
