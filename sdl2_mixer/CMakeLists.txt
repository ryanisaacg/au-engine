cmake_minimum_required(VERSION 2.4.0)
project (sdl-mixer)
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0015 NEW)

include_directories(. ../sdl2/include)

file(GLOB SOURCES *.c)

add_library(SDL2_mixer SHARED ${SOURCES})
target_link_libraries(SDL2_mixer SDL2)
