cmake_minimum_required( VERSION 3.10.2 )

project( IPVC )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin )


SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall" )
SET( CMAKE_CXX_FLAGS_RELEASE "-O3" )

add_executable( BitStreamTests Tests/TestBitStream.cpp )
add_executable( GolombTests Tests/TestGolomb.cpp)
add_executable( IntraframeEncoder Tests/IntraframeEncoder.cpp )
add_executable( HybridEncoder Tests/HybridEncoder.cpp )

add_library( BitStream Classes/BitStream.cpp)
add_library( Golomb Classes/Golomb.cpp)
add_library( Encoder Classes/Encoder.cpp )
add_library( FormatConverter Classes/FormatConverter.cpp)

target_include_directories( BitStreamTests PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
target_include_directories( GolombTests PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
target_include_directories( IntraframeEncoder PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
target_include_directories( HybridEncoder PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )

target_link_libraries( BitStreamTests BitStream ${OpenCV_LIBS} )
target_link_libraries( GolombTests Golomb BitStream ${OpenCV_LIBS} )
target_link_libraries( IntraframeEncoder Encoder Golomb BitStream FormatConverter ${OpenCV_LIBS} )
target_link_libraries( HybridEncoder Encoder Golomb BitStream FormatConverter ${OpenCV_LIBS} )

