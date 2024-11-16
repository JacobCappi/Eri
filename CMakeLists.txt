
cmake_minimum_required(VERSION 3.25.0)
project(Eri VERSION 0.0.01)
set(ENGINE_NAME "${PROJECT_NAME}-LIB")
set(EXE_NAME "${PROJECT_NAME}-TESTER")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

message("${CMAKE_INSTALL_PREFIX}")
add_subdirectory(Engine)
add_subdirectory(Game)