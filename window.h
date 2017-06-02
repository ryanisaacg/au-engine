#pragma once

#include "context.h"

typedef struct {
	int id, width, height;
} AU_Texture;

typedef struct {
	AU_Context ctx;
} AU_Engine;

//Intialize the engine with a window title, width, and height
AU_Engine *AU_Init(char*, int, int);
//Load a texture from a file with a given name
AU_Texture AU_LoadTexture(AU_Engine*, char* name);
//Draw a texture at a rectangular position
void AU_DrawTextureRect(AU_Engine*, AU_Texture, 
