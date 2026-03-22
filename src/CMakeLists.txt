# Project Executable
add_executable(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Sources
target_sources(${PROJECT_NAME}
PRIVATE
    Graphics/Entity.h
    Graphics/Entity.cpp
    Interface/Button.h
    Interface/Button.cpp
    Application.h
    Application.cpp
    main.cpp
)

set(raylib_VERBOSE 1)
target_link_libraries(${PROJECT_NAME} raylib)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_compile_strict(${PROJECT_NAME})

# Web Configurations
if (DEFINED PLATFORM AND PLATFORM STREQUAL "Web")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html") # Tell Emscripten to build an example.html file.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s GL_ENABLE_GET_PROC_ADDRESS=1")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
