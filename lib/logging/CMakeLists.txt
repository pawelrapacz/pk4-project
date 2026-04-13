add_library(logging)

target_sources(logging PRIVATE
    src/logging.cpp
    src/styles.cpp
)

target_compile_features(logging PRIVATE cxx_std_23)

target_include_directories(logging PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)