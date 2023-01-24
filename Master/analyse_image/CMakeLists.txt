cmake_minimum_required(VERSION 2.8)
project( DisplayImage )
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

####################################################

add_executable( RegionGrowing RG.cpp )
target_link_libraries( RegionGrowing ${OpenCV_LIBS} )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable( Contours contours.cpp )
target_link_libraries( Contours ${OpenCV_LIBS} )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")