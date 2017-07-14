# AU Engine

## An open-source cross-platform 2D game engine

The project is written in ISO C99 for maximum portability. It has been tested and compiles under Windows, macOS, and Linux with clang, gcc, and MinGW. 

## Required tools to build

- [CMake](https://cmake.org/)
- A C build system compatible with CMake
- A C compiler compatible with your build system

### Recommended compilers and toolchains

- Windows: [MinGW-w64](https://mingw-w64.org/doku.php/download)
	- SDL notes that the default 32-bit MinGW will likely not work, and this is known to be the case for the engine
	- There seems to be an issue with GCC 7 and SDL; it is recommended you use GCC version 6 on Windows.
- macOS: clang ( `brew install clang` )
- Linux: gcc (install from your system package manager)

### Dependencies

If your system has installed sdl2 and sdl2\_ttf, AU Engine will use your installed versions, otherwise it will compile its own packed-in versions. It is strongly recommended that you install a system version as it has been pre-configured for your system. For example, Raspbian does not seem compatible with the packed-in build process.

### Project generator

AU Engine contains a project generation script in Python that automates creating a new project with the engine. It should work with either Python 2 or Python 3. It is recommended you place the script on your system's path for easy access.

## Feature List

- Texture loading and rendering
- Auotmatic batched rendering
- Camera / viewport system
- Polygon, rectangle, and circle rendering
- TTF font rendering
- Particle system
- Sprites and animated sprites
- Zero effort game loops
- Cross-platform TCP sockets
- Basic geometry module with circles, rects, etc.
- 2D tilemap for storing and pixel-perfect geometry collision

## To-Do

- [ ] README
	- [ ] Build tutorial that does not require CMake or Make previous knowledge
	- [ ] Build generator tutorial that does not require previous knowledge of system paths
- [ ] Documentation
	- [ ] Write documentation for each header
	- [ ] Generate and host the documentation on a github page
	- [ ] Link to the documentation from the README
- [ ] Featureset
	- [ ] Loading and playing sound
	- [ ] Gamepad API
	- [ ] Screen-shake, inverted screen, etc.
	- [ ] User-created shaders
- [ ] Bugs
	- [ ] Viewports seem not to correctly apply when the window is resized
