#!/bin/bash

# clean build
make clean

# clean install
rm -rf sfml-tmxloader
rm -rf TGUI

# clean unix cmake
rm -rf bin/CMakeFiles
rm -rf build

# clean cross platform binaries
rm -f bin/Kobla-2D-Client*
rm -f bin/*.dll.a

# clean rel_bin
rm -rf rel_bin/windows