#!/usr/bin/env bash

echo "Configuring and building tree ..."

mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j1
