cmake_minimum_required(VERSION 2.8)

# Name of the project.
set(PROJECT SpellChecker)
project(${PROJECT})

# Additional compiler flags.
if(MSVC)
	# Enable ALL warnings and treat them as errors.
    add_definitions(/W4 /Wall /WX)
elseif(CMAKE_COMPILER_IS_GNUCXX)
	# Enable ALL warnings and treat them as errors.
	# Also enable c++11 standard support.
    add_definitions(-std=c++11 -Wall -Wextra -Werror -pedantic)
else()
	# FATAL_ERROR will stop cmake processing.
    message(FATAL_ERROR "Unknown compiler. Check compiler flags.")
endif()

# Process the subdirectories.
add_subdirectory(shared)
add_subdirectory(SpellCheckerDll)
add_subdirectory(SpellChecker)
#add_subdirectory(tests) // TODO