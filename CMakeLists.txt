cmake_minimum_required(VERSION 2.8)

project(VideoStabilization)
find_package(OpenCV REQUIRED )
include_directories( include ${OpenCV_INCLUDE_DIRS} )
add_executable( VideoStabilization src/main.cpp )
target_link_libraries( VideoStabilization ${OpenCV_LIBS} )