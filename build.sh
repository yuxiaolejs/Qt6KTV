#!/bin/bash
rm -rf ./build
mkdir build
pushd build
cmake .. -DCMAKE_PREFIX_PATH=/home/unics/Qt/6.9.1/gcc_64
make -j$(nproc)
popd
./build/VideoPlayer