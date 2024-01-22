#! /bin/bash

abi=64
build_dir=build/itm$abi

cmake -DABI=$abi -DCMAKE_BUILD_TYPE=Release -G Ninja -B $build_dir
cmake --build build/itm$abi
