cmake_minimum_required(VERSION 3.31)
project(battleships DESCRIPTION "pk4-project" LANGUAGES CXX)

set(PROJECT_AUTHOR "Paweł Rapacz")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CONFIGURATION_TYPES Debug Release)

set(
    CMAKE_CXX_STANDARD 23
    CMAKE_CXX_STANDARD_REQUIRED ON
    CMAKE_CXX_EXTENSIONS OFF
)

function(target_compile_strict target)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Werror -pedantic-errors)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE /W4)
    endif() 
endfunction()

# Dependencies
include(FetchContent)

# raylib
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    FIND_PACKAGE_ARGS 5.5
)

# cpp-httplib
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.43.1
    FIND_PACKAGE_ARGS 0.43.1
)

# json
# using URL not GIT_REPOSITORY because the repo is quite hevy (236 MiB)
set(NLOHMANN_JSON_VERSION 3.12.0)
FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/download/v${NLOHMANN_JSON_VERSION}/json.tar.xz
    URL_HASH SHA256=42f6e95cad6ec532fd372391373363b62a14af6d771056dbfc86160e6dfff7aa
    FIND_PACKAGE_ARGS ${NLOHMANN_JSON_VERSION}
)

# doctest
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.5.2
    FIND_PACKAGE_ARGS 2.5.2
)

FetchContent_MakeAvailable(raylib nlohmann_json httplib doctest)

enable_testing()
include("${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake")

# libs
add_subdirectory(lib/logging)

# Project
add_subdirectory(src)

# Tests
add_subdirectory(tests)
