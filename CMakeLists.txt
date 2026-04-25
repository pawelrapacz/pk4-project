cmake_minimum_required(VERSION 3.31)
project(Battleships DESCRIPTION "pk4-project" LANGUAGES CXX)

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
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Werror -pedantic-errors)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        target_compile_options(${target} PRIVATE /W4)
    endif() 
endfunction()

function(target_sanitize target type)
    message("target_sanitize is active only in debug builds (Debug RelWithDebInfo)")

    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        if (NOT type MATCHES "fuzzer")
            target_compile_options(${target} PRIVATE
                $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-fsanitize=${type}>
                $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-fno-omit-frame-pointer>
            )
            target_link_options(${target} PRIVATE
                $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-fsanitize=${type}>
            )
        endif()
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        message("MSVC supports only address, kernel-address and fuzzer sanitization")
        if (type MATHES "address|kernel-address|fuzzer")
            target_compile_options(${target} PRIVATE
                $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:/fsanitize=${type}>
            )
            target_link_options(${target} PRIVATE
                $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:/fsanitize=${type}>
            )
        endif()
    endif() 
endfunction()

# Dependencies
include(FetchContent)

# raylib
set(RAYLIB_VERSION 6.0)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG ${RAYLIB_VERSION}
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION}
)

# cpp-httplib
set(HTTP_VERSION 0.43.1)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v${HTTP_VERSION}
    FIND_PACKAGE_ARGS ${HTTP_VERSION}
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
set(DOCTEST_VERSION 2.5.2)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v${DOCTEST_VERSION}
    FIND_PACKAGE_ARGS ${DOCTEST_VERSION}
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
