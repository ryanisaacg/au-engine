#pragma once
//Get the necessary structures for GPU targets and images
#include "glad/glad.h"

#include "color.h"
#include "geom.h"

typedef struct {
	//The texture for this image
	GLuint image;
	//The number of vertices and the capacity for vertices before realloc
	int vertex_count, vertex_capacity;
	//The vertex buffer, tightly packed
	//(x, y, z, s, t, r, g, b, a) per vertex
	float* vertices;
	//The number of indices and the capacity for indices before realloc
	int index_count, index_capacity;
	//The index buffer (each trio of indices forms a triangle)
	GLuint* indices;
} AU_BatchEntry;

/*
 * Holds all of the graphics context in the engine
 */
typedef struct {
	SDL_Window* window;
	SDL_GLContext ctx;
	GLuint shader, fragment, vertex, vbo, ebo, vao, texture_location;
	//The number of loaded textures stored in the context
	int tex_count;
	//The capacity of the textures without reallocation
	int tex_capacity;
	//A buffer of different image buckets
	AU_BatchEntry* image_buffer;
} AU_Context;

//Initialize the context
AU_Context au_context_init_stack(SDL_Window*);
//Create an AU Context
AU_Context* au_context_init(SDL_Window*);
//Destroy the AU Context (does not invalidate the pointer)
void au_context_quit(AU_Context*);
//Destroy the AU Context (invalidates the pointer also)
void au_context_free(AU_Context*);
//Load a texture into the context
int au_context_register_texture(AU_Context*, GLuint);
//Add a single vertex into the context and return the index of that vertex
int au_context_add_vertex(AU_Context*, int texture,
						  float x, float y, float z, float texX, float texY, float r, float g, float b, float a);
//Add a single index into the context
void au_context_add_index(AU_Context*, int texture, int vertexID);
//Clears all of the stuff from the previous draw call and replaces it with a color
void au_context_clear(AU_Context*, AU_Color);
//Batch-draws each of the shapes
void au_context_present(AU_Context*, AU_Rectangle camera);


