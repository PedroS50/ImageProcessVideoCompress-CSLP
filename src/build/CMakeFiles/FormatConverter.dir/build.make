# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pedro/ImageProcessVideoCompress-CSLP/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pedro/ImageProcessVideoCompress-CSLP/src/build

# Include any dependencies generated for this target.
include CMakeFiles/FormatConverter.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FormatConverter.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FormatConverter.dir/flags.make

CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o: CMakeFiles/FormatConverter.dir/flags.make
CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o: ../Classes/FormatConverter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pedro/ImageProcessVideoCompress-CSLP/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o -c /home/pedro/ImageProcessVideoCompress-CSLP/src/Classes/FormatConverter.cpp

CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pedro/ImageProcessVideoCompress-CSLP/src/Classes/FormatConverter.cpp > CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.i

CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pedro/ImageProcessVideoCompress-CSLP/src/Classes/FormatConverter.cpp -o CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.s

CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o.requires:

.PHONY : CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o.requires

CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o.provides: CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o.requires
	$(MAKE) -f CMakeFiles/FormatConverter.dir/build.make CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o.provides.build
.PHONY : CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o.provides

CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o.provides.build: CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o


# Object files for target FormatConverter
FormatConverter_OBJECTS = \
"CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o"

# External object files for target FormatConverter
FormatConverter_EXTERNAL_OBJECTS =

libFormatConverter.a: CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o
libFormatConverter.a: CMakeFiles/FormatConverter.dir/build.make
libFormatConverter.a: CMakeFiles/FormatConverter.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pedro/ImageProcessVideoCompress-CSLP/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libFormatConverter.a"
	$(CMAKE_COMMAND) -P CMakeFiles/FormatConverter.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FormatConverter.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FormatConverter.dir/build: libFormatConverter.a

.PHONY : CMakeFiles/FormatConverter.dir/build

CMakeFiles/FormatConverter.dir/requires: CMakeFiles/FormatConverter.dir/Classes/FormatConverter.cpp.o.requires

.PHONY : CMakeFiles/FormatConverter.dir/requires

CMakeFiles/FormatConverter.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FormatConverter.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FormatConverter.dir/clean

CMakeFiles/FormatConverter.dir/depend:
	cd /home/pedro/ImageProcessVideoCompress-CSLP/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pedro/ImageProcessVideoCompress-CSLP/src /home/pedro/ImageProcessVideoCompress-CSLP/src /home/pedro/ImageProcessVideoCompress-CSLP/src/build /home/pedro/ImageProcessVideoCompress-CSLP/src/build /home/pedro/ImageProcessVideoCompress-CSLP/src/build/CMakeFiles/FormatConverter.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FormatConverter.dir/depend
