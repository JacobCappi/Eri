add_library(engine SHARED src/core/testing.cpp)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")