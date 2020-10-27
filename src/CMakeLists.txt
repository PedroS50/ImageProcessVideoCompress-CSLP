cmake_minimum_required(VERSION 2.8)

project( VideoProcessing )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( project VideoProcessing.cpp )
target_link_libraries( project ${OpenCV_LIBS} )
