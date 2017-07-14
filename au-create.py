#!/usr/bin/env python
import os
import sys
if sys.argv != 2:
    print("Pass a name for a project which will be created in a folder of the same name")
else:
    main()
def main():
    name = sys.argv[1]
    cmake_contents='''cmake_minimum_required(VERSION 2.4.0)
project (''' + name + ''')
set(CMAKE_BUILD_TYPE Debug)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0015 NEW)

file(GLOB SOURCES *.c)

add_subdirectory(au-engine)

include_directories(${au-engine-include})

add_executable(game ${SOURCES})
target_link_libraries(game ${au-engine-libs})
configure_file(assets assets COPYONLY)
'''
    os.system('mkdir ' + name)
    os.chdir(name)
    os.system('git init')
    os.system('git submodule add https://github.com/ryanisaacg/au-engine')
    os.system('mkdir build')
    os.chdir('build')
    if os.name == 'nt':
        os.system('cmake -G "MinGW Makefiles" ..')
    else:
        os.system('cmake ..')
    print('Created project structure for an AU project')
