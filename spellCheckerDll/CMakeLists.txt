# Name of the project.
set(PROJECT spellCheckerDll)
project(${PROJECT})

# Headers.
file(GLOB HEADERS *.h *.hpp)

# Sources.
file(GLOB SOURCES *.cpp)

# For Visual Studio folders.
source_group("Header Files" FILES ${HEADERS})
source_group("Source Files" FILES ${SOURCES})

# Create DLL.
add_library(${PROJECT} SHARED ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT} Shlwapi.lib)