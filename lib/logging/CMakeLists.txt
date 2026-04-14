add_library(logging INTERFACE)
target_compile_features(logging INTERFACE cxx_std_23)
target_include_directories(logging INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(tests)