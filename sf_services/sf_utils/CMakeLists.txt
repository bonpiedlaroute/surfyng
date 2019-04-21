cmake_minimum_required(VERSION 3.8.2)

set (CMAKE_CXX_STANDARD 17)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")

project(sf_utils)

include_directories(inc)

SET(SRCS src/Logger.cpp src/Config.cpp src/Str.cpp
	src/Math.cpp src/Algorithm.cpp)

add_library(sf_utils SHARED	${SRCS})
