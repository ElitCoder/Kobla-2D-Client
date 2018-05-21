#!/bin/bash

# get number of cores
cores=`grep --count ^processor /proc/cpuinfo`

# install dependencies
sudo pacman -Syy && sudo pacman -S --needed gcc sfml

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

# temp fix for cmake copying error
sudo cp lib/libtgui.so* /usr/local/lib/

cd ../
echo "TGUI was installed, with a workaround"

# move libs to PATH
sudo cp /usr/local/lib/libtmx-loader.so /usr/lib/
sudo cp /usr/local/lib/libpugi.so /usr/lib/
sudo cp /usr/local/lib/libtgui.so* /usr/lib/

# cleanup
rm -rf sfml-tmxloader
rm -rf TGUI