#!/bin/sh

echo "Installing CMake 3.8 ..."
wget http://www.cmake.org/files/v3.8/cmake-3.8.0.tar.gz
tar zxf cmake-3.8.0.tar.gz
cd cmake-3.8.0
./configure > /dev/null 2>&1
make > /dev/null 2>&1
sudo make install > /dev/null 2>&1
echo "Done installing CMake 3.8"
