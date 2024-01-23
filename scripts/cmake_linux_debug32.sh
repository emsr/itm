#! /bin/bash

abi=32
build_dir=build/debug$abi

cmake -DABI=$abi -DCMAKE_BUILD_TYPE=Debug -G Ninja -B $build_dir
cmake --build $build_dir
