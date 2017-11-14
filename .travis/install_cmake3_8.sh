#!/bin/sh

wget http://www.cmake.org/files/v3.8/cmake-3.8.0.tar.gz
tar zxf cmake-3.8.0.tar.gz
cd cmake-3.8.0
./configure
make
sudo make install
