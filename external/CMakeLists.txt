# doctest
add_library(doctest INTERFACE)
target_compile_features(doctest INTERFACE cxx_std_11)
target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
