#!/bin/bash

# get number of cores
cores=`grep --count ^processor /proc/cpuinfo`

# install dependencies
sudo pacman -Syy && sudo pacman -S --needed gcc sfml cmake zapcc yaourt

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