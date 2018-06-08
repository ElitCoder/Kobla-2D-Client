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

# cleanup
rm -rf sfml-tmxloader
rm -rf TGUI