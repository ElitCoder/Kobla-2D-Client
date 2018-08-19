cmake_minimum_required(VERSION 2.6)

# Project name
project(Kobla-2D-Client)

# Paths
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Include directories
include_directories("${PROJECT_SOURCE_DIR}")

# Find all source files
file(GLOB files ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Static build for Windows
if (WIN32)
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	set(BUILD_SHARED_LIBS OFF)
	set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
	
	set(CMAKE_MODULE_PATH "/usr/i686-w64-mingw32/cmake/Modules" ${CMAKE_MODULE_PATH})
endif (WIN32)

# Dependencies
find_package(Threads REQUIRED)
find_package(SFML 2 COMPONENTS graphics window system REQUIRED)

if (UNIX)
	find_package(TGUI REQUIRED)
endif (UNIX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic-errors -O3")

# Executable
add_executable(${PROJECT_NAME} ${files})

# Link
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} tgui)										# No FindTGUI.cmake
target_link_libraries(${PROJECT_NAME} tmx-loader)								# No FINDTMX-Loader.cmake

if (UNIX)
	target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
endif (UNIX)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} ws2_32.lib)
	target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif (WIN32)