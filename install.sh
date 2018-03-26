#!/bin/bash

# install dependencies
sudo apt-get install libsfml-dev

# install sfml-tmxloader
git clone https://github.com/fallahn/sfml-tmxloader.git
cd sfml-tmxloader/
mkdir build; cd build
cmake .. && make -j 9
sudo make install

# move libs to PATH
sudo cp /usr/local/lib/libtmx-loader.so /usr/lib/x86_64-linux-gnu/libtmx-loader.so
sudo cp /usr/local/lib/libpugi.so /usr/lib/x86_64-linux-gnu/libpugi.so

# cleanup
cd ../../
rm -rf sfml-tmxloader