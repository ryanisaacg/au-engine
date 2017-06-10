#pragma once

#include "texture.h"
#include <SDL2/SDL_ttf.h>

#define FONT_MAX_CHARS 223

typedef struct {
	AU_TextureRegion characters[FONT_MAX_CHARS];
	int height;
} AU_Font;

//Recursive dependency- must come after type declaration
#include "au.h"

AU_Font* au_font_init(AU_Engine*, TTF_Font*, AU_Color);
AU_TextureRegion au_font_get_char(const AU_Font*, char);
void au_font_destroy(AU_Font*);
