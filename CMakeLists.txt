cmake_minimum_required(VERSION 2.8)
project(starcraft)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -O3")

include_directories(src/simu)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable( TestApp ${SOURCES})
