#pragma once
//Get the necessary structures for GPU targets and images
#include <SDL_gpu.h>
/*
 * Holds all of the graphics context in the engine
 */
typedef struct {
	//Store the target from the SDL_gpu library
	GPU_Target target;
	//The number of loaded textures stored in the context
	int tex_count;
	//The capacity of the textures without reallocation
	int tex_capacity;
	//A buffer of the textures that have been loaded
	GPU_Image *images;
	//A buffer of sprite counts for each image
	int *sprite_count;
	//The capacity for sprites for each image
	int *sprite_capacity;
	//A buffer of vertex buffers for each sprite (x, y, s, t, r, g, b, a) per vertex
	float **values;
	//A buffer of index buffers for each sprite
	int **indices;
} AUContext;

