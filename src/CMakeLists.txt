
cmake_minimum_required(VERSION 3.25.0)
project(Eri VERSION 0.0.01)
set(ENGINE_NAME "${PROJECT_NAME}-LIB")
set(EXE_NAME "${PROJECT_NAME}-TESTER")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

message("${CMAKE_INSTALL_PREFIX}")

add_library(${ENGINE_NAME} SHARED 
    Engine/src/App.cpp
    Engine/src/Subsystems/Logger/BasicLogger/BasicLogger.cpp
    Engine/src/Subsystems/Logger/QuietLogger/QuietLogger.cpp
    Engine/src/Subsystems/Events/VectorEvents/VectorEvents.cpp
    Engine/src/Subsystems/Platform/Windows/PlatformWindows.cpp
    Engine/src/Subsystems/Platform/Windows/WindowKeys.cpp
    Engine/src/Subsystems/Factory.cpp
)

find_package(Vulkan REQUIRED)
target_link_libraries(${ENGINE_NAME} PRIVATE Vulkan::Vulkan) 

target_include_directories(${ENGINE_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/Engine/src")
target_compile_options(${ENGINE_NAME} PRIVATE -g -Wall -Werror)
target_compile_definitions(${ENGINE_NAME} PRIVATE ERI_EXPORT _DEBUG)

add_executable(${EXE_NAME}
    Game/src/GameWorld.cpp
)
target_link_libraries(${EXE_NAME} PRIVATE ${ENGINE_NAME})
