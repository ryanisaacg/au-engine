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
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wfatal-errors -pedantic -std=c99 -Wno-unused-parameter")
endif()

file(GLOB SOURCES *.c)
file(GLOB DEPENDENCIES deps/*.c)
include_directories(. deps)

#LINK LIBRARY
add_library(au-engine STATIC ${SOURCES} ${DEPENDENCIES} )
set_property(TARGET au-engine PROPERTY C_STANDARD 99)
set_target_properties(au-engine PROPERTIES LINKER_LANGUAGE C)

#CREATE EXECUTABLE
add_executable(example example/example.c)
set_property(TARGET example PROPERTY C_STANDARD 99)
set_target_properties(example PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(example au-engine m SDL2_ttf SDL2)
if(WIN32)
	target_link_libraries(example opengl32)
else()
	target_link_libraries(example GL)
endif()

#CREATE TEST EXECUTABLE
add_executable(tests test/test.c)
set_property(TARGET tests PROPERTY C_STANDARD 99)
set_target_properties(tests PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(tests au-engine m SDL2_ttf SDL2)
if(WIN32)
	target_link_libraries(tests opengl32)
else()
	target_link_libraries(tests GL)
endif()

#CREATE THE TEXTURE PACKING TOOL
add_executable(aupack packer/packer.c)
set_property(TARGET aupack PROPERTY C_STANDARD 99)
set_target_properties(aupack PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(aupack au-engine m SDL2)
if(WIN32)
	target_link_libraries(aupack opengl32)
else()
	target_link_libraries(aupack GL)
endif()
