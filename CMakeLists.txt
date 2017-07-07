cmake_minimum_required(VERSION 2.4.0)
project (au-engine)
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0015 NEW)

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
set(au-engine-include ${au-engine_SOURCE_DIR} ${au-engine_SOURCE_DIR}/deps)
include_directories(${au-engine-include})

#LINK LIBRARY
add_library(au STATIC ${SOURCES} ${DEPENDENCIES} )
set_property(TARGET au PROPERTY C_STANDARD 99)
set_target_properties(au PROPERTIES LINKER_LANGUAGE C)

set(au-engine-libs au SDL2_ttf SDL2 m)
if(WIN32)
	set(au-engine-libs ${au-engine-libs} opengl32 ws2_32)
elseif(APPLE)
	find_library(OpenGL REQUIRED)
	set(au-engine-libs ${au-engine-libs} ${OpenGL-libs})
else()
	set(au-engine-libs ${au-engine-libs} dl GL)
endif()

execute_process(COMMAND "sdl2-config --cflags" au-engine-include)

#CREATE EXECUTABLE
add_executable(example example/example.c)
target_link_libraries(example ${au-engine-libs})

#CREATE TEST EXECUTABLE
add_executable(tests test/test.c)
target_link_libraries(tests ${au-engine-libs})

#CREATE NETWORKING EXAMPLE
add_executable(net-example example/net-example.c)
target_link_libraries(net-example ${au-engine-libs})

#EXPORT THE VARIABLES
set(au-engine-include ${au-engine-include} PARENT_SCOPE)
set(au-engine-libs ${au-engine-libs} PARENT_SCOPE)
