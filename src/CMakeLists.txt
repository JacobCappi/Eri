
cmake_minimum_required(VERSION 3.25.0)
project(Eri VERSION 0.0.01)
set(ENGINE_NAME "${PROJECT_NAME}-lib")
set(EXE_NAME "${PROJECT_NAME}-tester")

function(printVar)
    foreach(var ${ARGN})
        message("${var} = ${${var}}")
    endforeach()
endfunction()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

printVar(${CMAKE_INSTALL_PREFIX})

add_library(${ENGINE_NAME} SHARED 
    Engine/src/App.cpp
    Engine/src/Subsystems/Logger/BasicLogger/BasicLogger.cpp
)

target_include_directories(${ENGINE_NAME} PUBLIC "Engine/src")
target_compile_options(${ENGINE_NAME} PRIVATE -g -Wall -Werror)
target_link_libraries(${ENGINE_NAME} PRIVATE -luser32 -Isrc -I$VULKAN_SDK/include -L$VULKAN_SDK/lib)
target_compile_definitions(${ENGINE_NAME} PRIVATE ERI_EXPORT _DEBUG)

add_executable(${EXE_NAME}
    Game/src/GameWorld.cpp
)
target_link_libraries(${EXE_NAME} PRIVATE ${ENGINE_NAME})


# install(TARGET "${PROJECT_NAME}" DESTINATION ${})