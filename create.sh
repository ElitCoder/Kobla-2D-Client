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