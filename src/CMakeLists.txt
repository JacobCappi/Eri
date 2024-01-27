
cmake_minimum_required(VERSION 3.25.0)
project(Eri VERSION 0.0.01)
set(ENGINE_NAME "${PROJECT_NAME}-lib")
set(EXE_NAME "${PROJECT_NAME}-tester")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message("${CMAKE_INSTALL_PREFIX}")

add_library(${ENGINE_NAME} SHARED 
    Engine/src/App.cpp
    Engine/src/Subsystems/Logger/BasicLogger/BasicLogger.cpp
    Engine/src/Subsystems/Logger/QuietLogger/QuietLogger.cpp
    Engine/src/Subsystems/EventSystem/VectorEvents/VectorEvents.cpp
    Engine/src/Subsystems/Platform/Windows/PlatformWindows.cpp
    Engine/src/Subsystems/Platform/Windows/WindowKeys.cpp
    Engine/src/Subsystems/Factory.cpp
)

message("${VULKAN_SDK}")

target_include_directories(${ENGINE_NAME} PUBLIC "Engine/src")
target_compile_options(${ENGINE_NAME} PRIVATE -g -Wall -Werror)

# TODO: This doesn't pull the vulkan lib for some reason
target_link_libraries(${ENGINE_NAME} PRIVATE -luser32 -Isrc -I$VULKAN_SDK/include -L$VULKAN_SDK/lib)
target_compile_definitions(${ENGINE_NAME} PRIVATE ERI_EXPORT _DEBUG)

add_executable(${EXE_NAME}
    Game/src/GameWorld.cpp
)
target_link_libraries(${EXE_NAME} PRIVATE ${ENGINE_NAME})


# install(TARGET "${PROJECT_NAME}" DESTINATION ${})