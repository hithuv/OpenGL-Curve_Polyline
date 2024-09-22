#!/bin/bash

# Change to the build directory
cd build

#cmake 
cmake -DMAKE_BUILD_TYPE=Release ..

# Run make
make

# Change back to the parent directory
cd ..

# run the built solution
./build/hw1