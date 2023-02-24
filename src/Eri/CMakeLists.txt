add_library(EriEngine SHARED 
    src/App.cpp
)

target_compile_options(EriEngine PRIVATE -g -Wall -Werror)
target_link_libraries(EriEngine PRIVATE -luser32 -Isrc -I$VULKAN_SDK/include -L$VULKAN_SDK/lib)
add_compile_definitions(ERI_EXPORT _DEBUG)

printVar(${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(EriEngine PUBLIC "src")