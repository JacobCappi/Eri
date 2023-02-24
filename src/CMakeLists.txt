
cmake_minimum_required(VERSION 3.25.0)
project(Eri VERSION 0.0.01)

function(printVar)
    foreach(var ${ARGN})
        message("${var} = ${${var}}")
    endforeach()
endfunction()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Eri)
add_subdirectory(Game)