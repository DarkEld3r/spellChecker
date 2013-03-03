# Name of the project.
set(PROJECT spellChecker)
project(${PROJECT})

# Headers.
file(GLOB HEADERS *.h *.hpp)

# Sources.
file(GLOB SOURCES *.cpp)
# Additional compiler flags.

# For Visual Studio folders.
source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})

add_executable(${PROJECT} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT} spellCheckerDll)