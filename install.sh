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
cd ../../

# install tgui
git clone https://github.com/texus/TGUI.git
cd TGUI/
cmake . && make -j $cores
sudo make install
cd ../

# move libs to PATH
sudo cp /usr/local/lib/libtmx-loader.so /usr/lib/
sudo cp /usr/local/lib/libpugi.so /usr/lib/
sudo cp /usr/local/lib/libtgui.so* /usr/lib/

# cleanup
rm -rf sfml-tmxloader
rm -rf TGUI