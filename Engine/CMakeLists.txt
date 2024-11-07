
set (ERI_SRC_FILES
  src/Eri/App.cpp
  src/Eri/Utils/Logger/BasicLogger/BasicLogger.cpp
  src/Eri/Utils/Logger/QuietLogger/QuietLogger.cpp
  src/Eri/Utils/Events/VectorEvents/VectorEvents.cpp
  src/Eri/Platform/EriPlatform.cpp
  src/Eri/Utils/EriUtils.cpp
)

if (WIN32)
  list (APPEND ERI_SRC_FILES
    src/Eri/Platform/Windows/PlatformWindows.cpp
    src/Eri/Platform/Windows/WindowKeys.cpp
  )
elseif (UNIX AND NOT APPLE)
  list (APPEND ERI_SRC_FILES
    src/Eri/Platform/Linux/PlatformLinux.cpp
    src/Eri/Platform/Linux/LinuxMouse.cpp
    src/Eri/Platform/Linux/LinuxKeys.cpp
  )
endif()

message("Full list of Sources ${ERI_SRC_FILES}")
add_library(${ENGINE_NAME} SHARED ${ERI_SRC_FILES})


find_package(Vulkan REQUIRED)
find_package(X11 REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE 
  Vulkan::Vulkan
  X11)

target_include_directories(${ENGINE_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/Engine/src")
target_compile_options(${ENGINE_NAME} PRIVATE -g -Wall -Werror)
target_compile_definitions(${ENGINE_NAME} PRIVATE ERI_EXPORT _DEBUG)
