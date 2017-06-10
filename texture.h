#pragma once

#include <SDL2/SDL.h>

#include "geom.h"

typedef struct {
	int id, width, height;
} AU_Texture;

typedef struct {
	AU_Texture source;
	AU_Rectangle rect;
} AU_TextureRegion;

typedef struct {
	float r, g, b, a;
} AU_Color;

//Turns the entire texture into a texture region
AU_TextureRegion au_tex_region(AU_Texture);
//Turn an AU_Color into an SDL_Color
SDL_Color au_color_to_sdl(AU_Color c);
