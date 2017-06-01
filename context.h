#pragma once
//Get the necessary structures for GPU targets and images
#include <SDL_gpu.h>

typedef struct {
	//The texture for this image
	GPU_Image image;
	//The number of vertices and the capacity for vertices before realloc
	int vertex_count, vertex_capacity;
	//The vertex buffer, tightly packed
	//(x, y, s, t, r, g, b, a) per vertex
	float *vertices;
	//The number of indices and the capacity for indices before realloc
	int index_count, index_capacity;
	//The index buffer (each trio of indices forms a triangle)
	int *indices;
} AU_BatchEntry;

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
	//A buffer of different image buckets
	AU_BatchEntry* image_buffer;
} AU_Context;

//Public facing functions:

//Create an AU Context with a given width and height
AU_Context* AU_Init(int, int);
//Destroy the AU Context (invalidates the pointer also
void AU_Quit(AU_Context*);
//Load a texture into the context
int AU_RegisterTexture(AU_Context*, GPU_Image*);

//Private facing functions:


