#pragma once

#include "color.h"
#include "texture.h"

#include <SDL_ttf.h>

#define FONT_MAX_CHARS 223

typedef struct {
	AU_TextureRegion characters[FONT_MAX_CHARS];
	int height;
} AU_Font;

//Recursive dependency- must come after type declaration
#include "au.h"

//Create a font from a TTF file
AU_Font* au_font_init(AU_Engine*, TTF_Font*, AU_Color);
//Get the texture region corresponding to a character
AU_TextureRegion au_font_get_char(const AU_Font*, char);
//Get the size that the string will take when drawn
AU_Rectangle au_font_get_size(const AU_Font*, const char*);
//Destroy a font
void au_font_destroy(AU_Font*);
