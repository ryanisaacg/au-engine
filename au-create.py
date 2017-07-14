#!/usr/bin/env python
import os
import sys
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
file(COPY assets DESTINATION assets)
'''
    os.system('mkdir ' + name)
    os.chdir(name)
    with open('CMakeLists.txt', 'w') as f:
        f.write(cmake_contents)
    main_contents='''#include "au.h"
int main() {
    AU_Engine* eng = au_init("''' + name + '''", 800, 600, NULL, DEFAULT_CONFIG);
    while(eng->should_continue) {
        au_begin(eng, AU_BLACK);
        au_end(eng);
    }
    au_quit(eng);
    return 0;
}'''
    with open('main.c', 'w') as f:
        f.write(main_contents)
    with open('.gitignore', 'w') as f:
        f.write('build/')
    os.system('git init')
    os.system('git submodule add https://github.com/ryanisaacg/au-engine')
    os.system('mkdir build')
    os.system('mkdir assets')
    os.chdir('build')
    if os.name == 'nt':
        os.system('cmake -G "MinGW Makefiles" ..')
    else:
        os.system('cmake ..')
    print('Created project structure for an AU project')
if len(sys.argv) != 2:
    print("Pass a name for a project which will be created in a folder of the same name")
else:
    main()
