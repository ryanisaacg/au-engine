#pragma once

#include <stdbool.h>

#include "context.h"
#include "geom.h"
#include "tex.h"
#include "tmap.h"
#include "util.h"

#define SDL_NUM_KEYS 284

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
//Quit the engine and free the memory
void au_quit(AU_Engine*);
//Load a texture from a file with a given name
AU_Texture au_load_texture(AU_Engine*, char*);
//Start drawing a frame
void au_begin(AU_Engine*);
//Stop drawing a frame
void au_end(AU_Engine*);
//Draw a texture region to a given area (x, y, width, height)
void au_draw_texture(AU_Engine*, AU_TextureRegion, float, float, float, float);
//Draw a texture region to a given rectangle
void au_draw_texture_rect(AU_Engine*, AU_TextureRegion, AU_Rectangle);
//Draw a texture region to a given area with a given transformation and origin (x, y) and dimensions (width, height)
void au_draw_texture_transform(AU_Engine*, AU_TextureRegion, AU_Transform, float, float, float, float);
//Draw a texture region blended with a texture
void au_draw_texture_blend(AU_Engine*, AU_TextureRegion, AU_Color, AU_Transform, float, float, float, float);
//Draw a texture without a precalculated transform
void au_draw_texture_ex(AU_Engine*, AU_TextureRegion, AU_Color, float x, float y, float w, float h, float rot,
						float or_x, float or_y, float scale_x, float scale_y);
