add_library(engine SHARED 
    src/core/Logger/Logger.cpp
    src/core/Logger/LogStream.cpp
)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")