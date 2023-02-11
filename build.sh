#!/bin/bash

echo "cleaning"
rm -rf build


conan install . --install-folder cmake-build-release --build=missing

mkdir build
cd build



echo "running cmake"

cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake-build-release/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release



