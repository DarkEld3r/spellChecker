# Name of the project.
set(PROJECT shared)
project(${PROJECT})

# Create static library.
add_library(${PROJECT} STATIC
	tdefs.h)

# CMake Error: CMake can not determine linker language for target:shared
SET_TARGET_PROPERTIES(${PROJECT} PROPERTIES LINKER_LANGUAGE CXX)