#pragma once

#include "context.h"

typedef struct {
	int id, width, height;
} AU_Texture;

typedef struct {
	AU_Context ctx;
} AU_Engine;

//Intialize the engine with a window title, width, and height
AU_Engine *au_init(char*, int, int);
//Load a texture from a file with a given name
AU_Texture au_load_texture(AU_Engine*, char*);
//Draw a texture to a given area (x, y, width, height)
void au_draw_texture(AU_Engine*, AU_Texture, float, float, float, float);
