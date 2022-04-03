add_library(engine SHARED 
    src/entry.h
    src/core/AppState.cpp
    src/core/Application.cpp
    src/core/Logger/Logger.cpp
    src/core/Logger/LogStream.cpp
    src/core/Subsystems/Platform_Windows.cpp
    src/core/Subsystems/Platform_Linux.cpp
)

target_compile_options(engine PRIVATE -g -fdeclspec -fPIC )
target_link_libraries(engine PRIVATE -D_DEBUG -DERI_EXPORT -Isrc -I$VULKAN_SDK/include -lxcb -lX11 -lX11-xcb -lxkbcommon -L$VULKAN_SDK/lib -L/usr/X11R6/lib)

target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")