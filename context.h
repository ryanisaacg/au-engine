#pragma once
//Get the necessary structures for GPU targets and images
#include <SDL_gpu.h>

#include "texture.h"

typedef struct {
	//The texture for this image
	GPU_Image image;
	//The number of vertices and the capacity for vertices before realloc
	int vertex_count, vertex_capacity;
	//The vertex buffer, tightly packed
	//(x, y, s, t, r, g, b, a) per vertex
	float* vertices;
	//The number of indices and the capacity for indices before realloc
	int index_count, index_capacity;
	//The index buffer (each trio of indices forms a triangle)
	unsigned short int* indices;
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

//Initialize the engine without heap allocating the context (set image path to null for no icon)
AU_Context au_context_init_stack(char*, int, int, char* image_path);
//Create an AU Context with a given title, width, and height
AU_Context* au_context_init(char*, int, int, char* image_path);
//Destroy the AU Context (does not invalidate the pointer)
void au_context_quit(AU_Context*);
//Destroy the AU Context (invalidates the pointer also)
void au_context_free(AU_Context*);
//Load a texture into the context
int au_context_register_texture(AU_Context*, GPU_Image*);
//Add a single vertex into the context and return the index of that vertex
int au_context_add_vertex(AU_Context*, int texture,
						  float x, float y, float z, float texX, float texY, float r, float g, float b, float a);
//Add a single index into the context
void au_context_add_index(AU_Context*, int texture, int vertexID);
//Clears all of the stuff from the previous draw call and replaces it with a color
void au_context_clear(AU_Context*, AU_Color);
//Batch-draws each of the shapes
void au_context_present(AU_Context*);

//Private facing functions:


