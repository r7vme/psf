#!/bin/bash
set -e
[ ! -d build ] && mkdir build
cmake -B build -GNinja .
cmake --build build
#cmake --build build --target test
