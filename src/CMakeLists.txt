# Project Executable
add_executable(${PROJECT_NAME})

# Sources
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_sources(${PROJECT_NAME}
PRIVATE
    Graphics/Entity.h
    Graphics/Entity.cpp
    Interface/Button.h
    Interface/Button.cpp
    Interface/Menu.h
    Interface/MainMenu.h
    Interface/MainMenu.cpp
    Interface/GameOverMenu.h
    Interface/GameOverMenu.cpp
    Interface/GridView.h
    Interface/GridView.cpp
    Interface/PlayerBuilderMenu.h
    Interface/PlayerBuilderMenu.cpp
    Player.h
    Player.cpp
    GridHelpers.h
    PlayerBuilder.h
    PlayerBuilder.cpp
    Enemy.h
    Enemy.cpp
    Serialization.h
    Serialization.cpp
    Game.h
    Game.cpp
    Application.h
    Application.cpp
    main.cpp
)

# copy logos etc.
file(COPY ${CMAKE_SOURCE_DIR}/res/img DESTINATION ${CMAKE_BINARY_DIR}/res)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)


target_link_libraries(${PROJECT_NAME} PRIVATE
    httplib::httplib
    logging_raylib
    nlohmann_json::nlohmann_json
    raylib
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    # Use logging only with Debug builds
    $<$<NOT:$<CONFIG:Debug>>:LOGGING_DISABLE>
)

target_compile_strict(${PROJECT_NAME})
target_sanitize(${PROJECT_NAME} "address")
target_sanitize(${PROJECT_NAME} "undefined")
target_sanitize(${PROJECT_NAME} "leak")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)


# Platform Specific configurations

# Web Configurations
if (DEFINED PLATFORM AND PLATFORM STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

if (APPLE)
    # Checks if OSX and links appropriate frameworks (Only required on MacOS)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")

    # macOS: build as .app bundle in all configs
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()

if (WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/res/Battleships.rc
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        # Windows: GUI for Release, MinSizeRel and RelWithDebInfo builds, console in Debug
        WIN32_EXECUTABLE $<$<NOT:$<CONFIG:Debug>>:TRUE>
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        # compatibility fix (Windows.h, raylib.h and httplib.h)
        WIN32_LEAN_AND_MEAN
        NOGDI
        NOUSER
        NOMINMAX
    )
endif()

# Precompiled headers
target_precompile_headers(${PROJECT_NAME} PRIVATE
    # std
    <algorithm>
    <array>
    <atomic>
    <cassert>
    <chrono>
    <concepts>
    <cstddef>
    <cstdint>
    <exception>
    <filesystem>
    <fstream>
    <functional>
    <future>
    <limits>
    <memory>
    <mutex>
    <random>
    <stdexcept>
    <string_view>
    <string>
    <thread>
    <type_traits>
    <utility>
    <vector>

    # Dependencies
    <httplib.h>
    <logging/adapters/raylib.h>
    <logging/logging.h>
    <nlohmann/json.hpp>
    <raylib.h>
)

# download gemma4:e2b
set(MODELS_DIR ${CMAKE_BINARY_DIR}/models)

set(MODELS_GEMMA gemma-4-E2B-it-Q8_0.gguf)
set(MODELS_GEMMA_HASH e049411c01fb7a81161768c52e38828970e55a64e22738957adcbe51d20f1c8e)
set(MODELS_GEMMA_COMPUTED_HASH "")

if (IS_READABLE ${MODELS_DIR}/${MODELS_GEMMA})
    message(STATUS "found ia model ${MODELS_GEMMA}")
    message(STATUS "evaluating ${MODELS_GEMMA} hash")
    file(SHA256 ${MODELS_DIR}/${MODELS_GEMMA} MODELS_GEMMA_COMPUTED_HASH)
else()
    message(STATUS "ai model ${MODELS_GEMMA} not found")
endif()

if (MODELS_GEMMA_HASH STREQUAL MODELS_GEMMA_COMPUTED_HASH)
    message(STATUS "${MODELS_GEMMA} hash correct")
else()
    message(STATUS "downloading ai model: ${MODELS_GEMMA}")
    file(
        DOWNLOAD
        https://huggingface.co/ggml-org/gemma-4-E2B-it-GGUF/resolve/main/gemma-4-E2B-it-Q8_0.gguf
        ${MODELS_DIR}/${MODELS_GEMMA}
        SHOW_PROGRESS
        EXPECTED_HASH SHA256=${MODELS_GEMMA_HASH}
    )
endif()