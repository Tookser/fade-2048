#!/bin/bash

cd $(dirname $0)
mkdir -p build
cd build
cmake -Dgtest_disable_pthreads=OFF ../src 
make -j4
cp ./bin/2048 ../2048
cp ./ut/2048_ut ../2048_ut