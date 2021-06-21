#!/bin/bash

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/build
cd build && cmake .. && make && cd ..
g++ main.cc -o main -std=c++11 -lpthread -L./build -laudio_extract -lavformat -lavdevice -lavcodec -lavutil -lswresample -lavfilter -lpostproc -lswscale
