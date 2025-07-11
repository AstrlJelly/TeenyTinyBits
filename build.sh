#!/bin/bash

mkdir -p ./build/
cd ./build/

cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DGLFW_BUILD_WAYLAND=OFF

cmake --build . $@
