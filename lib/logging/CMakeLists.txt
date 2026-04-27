option(LOGGING_RAYLIB_ADAPTER "Enable logging raylib adapter" OFF)

#  standard version
add_library(logging INTERFACE
    include/logging/logging.h
    include/logging/styles.h
)

target_compile_features(logging INTERFACE cxx_std_23)
target_include_directories(logging INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# enable raylib adapter
if (LOGGING_RAYLIB_ADAPTER)
    add_library(logging_raylib INTERFACE
        include/logging/raylib_adapter.h
    )
    
    target_compile_features(logging_raylib INTERFACE cxx_std_23)
    target_include_directories(logging_raylib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    
    target_compile_definitions(logging_raylib INTERFACE LOGGING_RAYLIB_ADAPTER)
    target_link_libraries(logging_raylib INTERFACE logging raylib)
endif()


# logging tests
add_subdirectory(tests)