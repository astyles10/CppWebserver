#!/bin/bash

mkdir -p build && cd build
cmake .. 
make
sudo cmake --install .