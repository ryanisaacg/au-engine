#pragma once

#include <SDL.h>

typedef struct {
	float r, g, b, a;
} AU_Color;

#define AU_CLEAR  ((AU_Color) {0,    0,    0, 0})
#define AU_WHITE  ((AU_Color) {1,    1,    1, 1})
#define AU_BLACK  ((AU_Color) {0,    0,    0, 1})
#define AU_RED    ((AU_Color) {1,    0,    0, 1})
#define AU_ORANGE ((AU_Color) {1,    0.5f, 0, 1})
#define AU_YELLOW ((AU_Color) {1,    1,    0, 1})
#define AU_GREEN  ((AU_Color) {0,    1,    0, 1})
#define AU_CYAN   ((AU_Color) {0,    1,    1, 1})
#define AU_BLUE   ((AU_Color) {0,    0,    1, 1})
#define AU_PURPLE ((AU_Color) {1,    0,    1, 1})
#define AU_INDIGO ((AU_Color) {0.5f, 0,    1, 1})

//Turn an AU_Color into an SDL_Color
SDL_Color au_color_to_sdl(AU_Color c);
