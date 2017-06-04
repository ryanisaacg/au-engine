#pragma once

#include <stdbool.h>

#include "context.h"
#include "geom.h"

#define SDL_NUM_KEYS 284

typedef struct {
	int id, width, height;
} AU_Texture;

typedef struct {
	AU_Context ctx;
	int fps;
	bool should_continue;
	bool current_keys[SDL_NUM_KEYS]; //The total number of SDL keys
	bool previous_keys[SDL_NUM_KEYS];
	int mouse_x, mouse_y;
	bool mouse_left, mouse_right, mouse_middle;
} AU_Engine;

//Intialize the engine with a window title, width, and height
AU_Engine* au_init(char*, int, int);
//Load a texture from a file with a given name
AU_Texture au_load_texture(AU_Engine*, char*);
//Start drawing a frame
void au_begin(AU_Engine*);
//Stop drawing a frame
void au_end(AU_Engine*);
//Draw a texture to a given area (x, y, width, height)
void au_draw_texture(AU_Engine*, AU_Texture, float, float, float, float);
//Draw a texture to a given area with a given transformation and origin (x, y) and dimensions (width, height)
void au_draw_texture_transform(AU_Engine*, AU_Texture, AU_Transform, float, float, float, float);
