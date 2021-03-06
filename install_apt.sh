#!/bin/bash

# get number of cores
cores=`grep --count ^processor /proc/cpuinfo`

# install dependencies
sudo apt-get update && sudo apt-get install g++ libsfml-dev cmake zlib1g-dev

# install sfml-tmxloader (own fork with map animations)
git clone https://github.com/ElitCoder/sfml-tmxloader.git
cd sfml-tmxloader/
./install.sh
cd ../

# install tgui
git clone https://github.com/texus/TGUI.git
cd TGUI/
cmake . && make -j $cores
sudo make install
cd ../

# copy links to /usr/lib/ to ensure that they're on PATH
sudo cp /usr/local/lib/libtgui.so* /usr/lib/

# cleanup
rm -rf sfml-tmxloader
rm -rf TGUI

# warn about zapcc
echo "WARNING: zapcc needs to be manually installed to build, or change the Makefile to use g++"