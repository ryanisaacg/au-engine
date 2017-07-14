cmake_minimum_required(VERSION 2.4.0)
project (au-engine)
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0015 NEW)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
	pkg_check_modules(SDL2 REQUIRED sdl2 sdl2_ttf)
endif()

if(NOT SDL2_FOUND)
	#Add the dependency libraries
	add_subdirectory(sdl2)
	add_subdirectory(sdl2_ttf)
	set(SDL2_INCLUDE_DIRS ${au-engine_SOURCE_DIR}/sdl2_ttf ${au-engine_SOURCE_DIR}/sdl2_ttf/freetype/include ${au-engine_SOURCE_DIR}/sdl2/include)
	set(SDL2_LIBRARIES SDL2 SDL2_ttf freetype)
endif()

#SET WARNINGS
if(MSVC)
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wfatal-errors -std=c99 -Wno-unused-parameter")
endif()

file(GLOB SOURCES *.c)
file(GLOB DEPENDENCIES deps/*.c)
set(au-engine-include ${au-engine_SOURCE_DIR} ${SDL2_INCLUDE_DIRS} ${au-engine_SOURCE_DIR}/deps)
include_directories(${au-engine-include})

set(au-engine-libs ${SDL2_LIBRARIES} freetype m)
if(WIN32)
	set(au-engine-libs ${au-engine-libs} opengl32 ws2_32)
elseif(APPLE)
	find_library(OpenGL REQUIRED)
	set(au-engine-libs ${au-engine-libs} ${OpenGL-libs})
else()
	set(au-engine-libs ${au-engine-libs} dl GL)
endif()

#LINK LIBRARY
add_library(au SHARED ${SOURCES} ${DEPENDENCIES} )
set_property(TARGET au PROPERTY C_STANDARD 99)
set_target_properties(au PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(au ${au-engine-libs})

set(au-engine-libs au ${au-engine-libs})

#CREATE EXECUTABLE
add_executable(example example/example.c)
target_link_libraries(example ${au-engine-libs})

#CREATE TEST EXECUTABLE
add_executable(tests test/test.c)
target_link_libraries(tests ${au-engine-libs})

#CREATE NETWORKING EXAMPLE
add_executable(net-example example/net-example.c)
target_link_libraries(net-example au ${au-engine-libs})

#EXPORT THE VARIABLES IF NECESSARY
get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
	set(au-engine-include ${au-engine-include} PARENT_SCOPE)
	set(au-engine-libs ${au-engine-libs} PARENT_SCOPE)
endif()
