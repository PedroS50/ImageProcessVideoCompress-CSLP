cmake_minimum_required( VERSION 3.10.2 )

project( IPVC )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR} )

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall" )
SET( CMAKE_CXX_FLAGS_RELEASE "-O3" )

# add_executable( BitStreamTests Tests/TestBitStream.cpp )
# add_executable( Conversions Tests/Conversions.cpp )
# add_executable( InterEncoder Tests/InterframeEncoder.cpp )
# add_executable( IntraEncoder Tests/IntraframeEncoder.cpp )
# add_executable( GolombTests Tests/TestGolomb.cpp)
# add_executable( HybridEncoder Tests/HybridEncoder.cpp )
# add_executable( RealHybridEncoder Tests/RealHybridEncoder.cpp )
# add_executable( IntraframeEncoder_Class Tests/IntraframeEncoder_Class.cpp )
# add_executable( InterframeEncoder_Class Tests/InterframeEncoder_Class.cpp )
add_executable( HybridEncoder Tests/HybridEncoder_Class.cpp )

add_library( BitStream Classes/BitStream.cpp)
add_library( Golomb Classes/Golomb.cpp)
add_library( Encoder Classes/Encoder.cpp )

# target_include_directories( BitStreamTests PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
# target_include_directories( InterEncoder PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
# target_include_directories( IntraEncoder PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
# target_include_directories( GolombTests PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
# target_include_directories( HybridEncoder PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
# target_include_directories( RealHybridEncoder PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
# target_include_directories( IntraframeEncoder_Class PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
# target_include_directories( InterframeEncoder_Class PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )
target_include_directories( HybridEncoder PUBLIC "${PROJECT_SOURCE_DIR}/Classes" )

# target_link_libraries( BitStreamTests BitStream ${OpenCV_LIBS} )
# target_link_libraries( Conversions ${OpenCV_LIBS} )
# target_link_libraries( InterEncoder Golomb BitStream ${OpenCV_LIBS} )
# target_link_libraries( IntraEncoder Golomb BitStream ${OpenCV_LIBS} )
# target_link_libraries( GolombTests Golomb BitStream ${OpenCV_LIBS} )
# target_link_libraries( HybridEncoder Golomb BitStream ${OpenCV_LIBS} )
# target_link_libraries( RealHybridEncoder Encoder Golomb BitStream ${OpenCV_LIBS} )
# target_link_libraries( IntraframeEncoder_Class Encoder Golomb BitStream ${OpenCV_LIBS} )
# target_link_libraries( InterframeEncoder_Class Encoder Golomb BitStream ${OpenCV_LIBS} )
target_link_libraries( HybridEncoder Encoder Golomb BitStream ${OpenCV_LIBS} )

