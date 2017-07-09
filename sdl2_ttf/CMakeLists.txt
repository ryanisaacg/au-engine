cmake_minimum_required(VERSION 2.4.0)
project (sdl-ttf)
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0015 NEW)

add_subdirectory(freetype)
add_subdirectory(sdl2)

include_directories(. freetype/include sdl2/include)

add_library(SDL2_ttf SHARED SDL_ttf.c)
target_link_libraries(SDL2_ttf SDL2 freetype)
