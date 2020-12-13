cmake_minimum_required( VERSION 3.10.2 )

project( IPVC )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} )

add_executable( BitStreamTests Tests/TestBitStream.cpp )
add_executable( GolombTests Tests/TestGolomb.cpp)

add_library( BitStream Classes/BitStream.cpp)
add_library( Golomb Classes/Golomb.cpp)

target_include_directories( BitStreamTests PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
target_include_directories( GolombTests PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )

target_link_libraries( BitStreamTests BitStream ${OpenCV_LIBS} )
target_link_libraries( GolombTests Golomb BitStream ${OpenCV_LIBS})

