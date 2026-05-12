add_executable(llm-tests
    tests_main.cpp
    remote_test.cpp
    api_formatters_test.cpp
    llama_wrapper_test.cpp
)

target_compile_definitions(llm-tests PRIVATE
    MODELS_PATH="${CMAKE_BINARY_DIR}/models"
)

target_link_libraries(llm-tests doctest::doctest llm logging)

target_compile_strict(llm-tests)
doctest_discover_tests(llm-tests)
