add_library(llm STATIC
    include/llm/llm.h
    include/llm/exception.h
    include/llm/llama_wrapper.h

    src/llama_wrapper.cpp
)

target_compile_features(llm PUBLIC cxx_std_23)
target_compile_strict(llm)

target_include_directories(llm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(llm PUBLIC
    llama
    nlohmann_json::nlohmann_json
)

target_link_libraries(llm PRIVATE
    logging_llama
    llama-common
)

target_precompile_headers(llm PRIVATE
    # std
    <atomic>
    <exception>
    <filesystem>
    <mutex>
    <string>
    <vector>

    # Dependencies
    <json-schema-to-grammar.h>
    <llama.h>
    <logging/adapters/llama.h>
    <logging/logging.h>
    <nlohmann/json.hpp>
)

add_subdirectory(tests)
