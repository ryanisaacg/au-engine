#pragma once

#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "context.h"
#include "geom.h"
#include "particle.h"
#include "sprite.h"
#include "texture.h"
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
	AU_Particle* particles;
	size_t particle_count, particle_capacity;
	AU_Tilemap map;
	unsigned int previous_ticks;
} AU_Engine;

//Intialize the engine with a window title, width, and height
AU_Engine* au_init(char*, int, int);
//Quit the engine and free the memory
void au_quit(AU_Engine*);
//Load a texture from a file with a given name
AU_Texture au_load_texture(AU_Engine*, const char*);
//Load a texture from a surface
AU_Texture au_load_texture_from_surface(AU_Engine*, SDL_Surface*);
//Start drawing a frame
void au_begin(AU_Engine*);
//Stop drawing a frame
void au_end(AU_Engine*);
//Draw a texture region with its natural size (x, y)
void au_draw_texture(AU_Engine*, AU_TextureRegion, float, float);
//Draw a texture region to a given area (x, y, width, height)
void au_draw_texture_sized(AU_Engine*, AU_TextureRegion, float, float, float, float);
//Draw a texture region to a given rectangle
void au_draw_texture_rect(AU_Engine*, AU_TextureRegion, AU_Rectangle);
//Draw a texture region to a given area with a given transformation and origin (x, y) and dimensions (width, height)
void au_draw_texture_transform(AU_Engine*, AU_TextureRegion, AU_Transform, float, float, float, float);
//Draw a texture region blended with a texture
void au_draw_texture_blend(AU_Engine*, AU_TextureRegion, AU_Color, AU_Transform, float, float, float, float,
						   bool flip_x, bool flip_y);
//Draw a texture without a precalculated transform
void au_draw_texture_ex(AU_Engine*, AU_TextureRegion, AU_Color, float x, float y, float w, float h, float rot,
						float or_x, float or_y, float scale_x, float scale_y, bool flip_x, bool flip_y);
//Draw a sprite
void au_draw_sprite(AU_Engine*, AU_Sprite*);
//Draw an animated sprite
void au_draw_sprite_animated(AU_Engine*, AU_AnimatedSprite*);
//Add a burst of particles
void au_add_particles(AU_Engine*, AU_ParticleEmitter*);
//Recursive dependency- must be included after declarations
#include "text.h"
//Load a font with a given size and color from a file
AU_Font* au_load_font(AU_Engine*, int size, AU_Color, const char* filename);
//Draw a single character to the screen (returns the width of the character)
int au_draw_char(AU_Engine*, AU_Font*, char, float x, float y);
//Draw text to the screen
void au_draw_string(AU_Engine*, AU_Font*, const char*, float x, float y);

#undef main
