cmake_minimum_required(VERSION 2.6)

# Project name
project(Kobla-2D-Client-Rebased)

# Paths
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Include directories
include_directories("${PROJECT_SOURCE_DIR}")

# Find all source files
file(GLOB files ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Static build for Windows (disable for now)
if (WIN32)
	#set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	#set(BUILD_SHARED_LIBS OFF)
	#set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
	
	# Not needed since we're not searching for dependencies
	#set(CMAKE_MODULE_PATH "/usr/i686-w64-mingw32/" ${CMAKE_MODULE_PATH})
endif (WIN32)

# Dependencies
find_package(Threads REQUIRED)

# Don't search for libraries in mingw32 Windows compile since they don't have
# a Find<lib>.cmake

if (UNIX)
	find_package(SFML 2 COMPONENTS graphics window system REQUIRED)
	find_package(TGUI REQUIRED)
endif (UNIX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic-errors -O3")

# Executable
add_executable(${PROJECT_NAME} ${files})

# Link
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} tgui)
target_link_libraries(${PROJECT_NAME} tmx-loader)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} ws2_32.lib)
endif (WIN32)