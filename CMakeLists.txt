cmake_minimum_required(VERSION 2.8)
project(starcraft)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -O3")



file(GLOB SOURCES "src/*.cpp")


add_executable( starcraft ${SOURCES})


target_include_directories(starcraft PRIVATE ${PROJECT_SOURCE_DIR}/include)
