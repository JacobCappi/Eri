#!/bin/bash
# Eri Build Script for Linux
set echo on

mkdir -p ../bin

# Get a list of all the .c files.
cFilenames=$(find . -type f -name "*.c")

echo "Engine Files:" $cFilenames 

assembly="engine"
compilerFlags="-g -shared -fdeclspec -fPIC"

# -fms-extensions 
# -Wall -Werror

includeFlags="-Isrc -I$VULKAN_SDK/include"
linkerFlags="-lvulkan -lxcb -lX11 -lX11-xcb -lxkbcommon -L$VULKAN_SDK/lib -L/usr/X11R6/lib"
defines="-D_DEBUG -DERI_EXPORT"

echo "Building Eri's $assembly..."
clang $cFilenames $compilerFlags -o ../bin/lib$assembly.so $defines $includeFlags $linkerFlags 
