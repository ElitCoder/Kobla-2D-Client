#!/bin/bash

cores=`grep --count ^processor /proc/cpuinfo`

function build {
	if [ $# -ne 0 ]; then
		make clean
	fi
	
	make -j $cores
	
	if [ $? -ne 0 ]; then
		exit 1
	fi
}

if [ $# -eq 0 ]; then
	build
else
	if [ $1 = "clean" ]; then
		build clean
	elif [ $1 = "rel_bin" ]; then
		./clean.sh
		
		# create Windows client
		mkdir build; cd build
		cmake -DCMAKE_TOOLCHAIN_FILE=../toolchain-windows-mingw32.cmake ..
		make -j $cores
		
		if [ $? -ne 0 ]; then
			exit 1
		fi
		
		cd ..
		mkdir -p rel_bin/windows
		
		# copy data, executable, config and prebuilts to release
		cp -r bin/data rel_bin/windows
		cp bin/Kobla-2D-Client-Rebased.exe rel_bin/windows
		cp bin/config rel_bin/windows
		cp prebuilt/* rel_bin/windows
		
		cd rel_bin/windows
		zip -r ../kobla_2d_client_windows.zip *
		cd ../..
		rm -rf rel_bin/windows
		
		./clean.sh
	elif [ $1 = "run" ]; then
		build
		
		cd bin/
		# for debugging
		#valgrind --track-origins=yes ./Kobla-2D-Client-Rebased
		# helgrind
		#valgrind --tool=helgrind ./Kobla-2D-Client-Rebased
		# normal use
		./Kobla-2D-Client-Rebased
		cd ../
	fi
fi