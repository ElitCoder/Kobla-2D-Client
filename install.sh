#!/bin/bash

# get number of cores
cores=`grep --count ^processor /proc/cpuinfo`

# install dependencies
sudo apt-get update && sudo apt-get install g++ libsfml-dev cmake zlib1g-dev

# install sfml-tmxloader
git clone https://github.com/fallahn/sfml-tmxloader.git
cd sfml-tmxloader/
mkdir build; cd build
cmake .. && make -j $cores
sudo make install

# move libs to PATH
sudo cp /usr/local/lib/libtmx-loader.so /usr/lib/x86_64-linux-gnu/libtmx-loader.so
sudo cp /usr/local/lib/libpugi.so /usr/lib/x86_64-linux-gnu/libpugi.so

# cleanup
cd ../../
rm -rf sfml-tmxloader