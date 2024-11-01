
add_library(${ENGINE_NAME} SHARED
  src/Eri/App.cpp
  src/Eri/Utils/Logger/BasicLogger/BasicLogger.cpp
  src/Eri/Utils/Logger/QuietLogger/QuietLogger.cpp
  src/Eri/Utils/Events/VectorEvents/VectorEvents.cpp
  src/Eri/Platform/EriPlatform.cpp
  src/Eri/Utils/EriUtils.cpp
)

find_package(Vulkan REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE Vulkan::Vulkan)

target_include_directories(${ENGINE_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/Engine/src")
target_compile_options(${ENGINE_NAME} PRIVATE -g -Wall -Werror)
target_compile_definitions(${ENGINE_NAME} PRIVATE ERI_EXPORT _DEBUG)
