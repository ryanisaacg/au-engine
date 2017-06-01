#include "context.h"

#include <SDL2/SDL.h>
#include <stdlib.h>

static void init_batch_entry(AU_BatchEntry*, GPU_Image*);

AU_Context* AU_Init(char* title, int w, int h) {
	AU_Context ctx;
	ctx.target = *(GPU_Init(w, h, GPU_DEFAULT_INIT_FLAGS));
	SDL_SetWindowTitle(SDL_GetWindowFromID(ctx.target.context->windowID), title);
	ctx.tex_count = 0;
	ctx.tex_capacity = 32;
	ctx.image_buffer = malloc(sizeof(AU_BatchEntry) * ctx.tex_capacity);
	AU_Context* alloc_ctx = malloc(sizeof(AU_Context));
	*alloc_ctx = ctx;
	return alloc_ctx;
}

void AU_Quit(AU_Context* ctx) {
	for(int i = 0; i < ctx->tex_capacity; i++) {
		free(ctx->image_buffer[i].vertices);
		free(ctx->image_buffer[i].indices);
	}
	free(ctx->image_buffer);
	free(ctx);
}

int AU_RegisterTexture(AU_Context* ctx, GPU_Image* img) {
	if(ctx->tex_count >= ctx->tex_capacity) {
		ctx->tex_capacity *= 2;
		ctx->image_buffer = realloc(ctx->image_buffer, sizeof(AU_BatchEntry) * ctx->tex_capacity);
	}
	init_batch_entry(ctx->image_buffer + ctx->tex_count, img);
	ctx->tex_count++;
	return ctx->tex_count - 1;
}

static void init_batch_entry(AU_BatchEntry* ent, GPU_Image* img) {
	ent->image = *img;
	ent->vertex_capacity = 1024;
	ent->vertex_count = 0;
	ent->vertices = malloc(sizeof(float) * ent->vertex_capacity);
	ent->index_capacity = 64;
	ent->index_count = 0;
	ent->indices = malloc(sizeof(int) * ent->index_capacity);
}

int AU_AddVertex(AU_Context* ctx, int texture, 
		float x, float y, float texX, float texY, float r, float g, float b, float a) {
	AU_BatchEntry* ent = ctx->image_buffer + texture;
	if(ent->vertex_count >= ent->vertex_capacity) {
		ent->vertex_capacity *= 2;
		ent->vertices = realloc(ent->vertices, sizeof(float) * ent->vertex_capacity);
	}
	float vertex[] = {x, y, texX, texY, r, g, b, a};
	memcpy(ent->vertices + ent->vertex_count * 8, vertex, sizeof(float) * 8);
	ent->vertex_count++;
	return ent->vertex_count - 1;
}

void AU_AddIndex(AU_Context* ctx, int texture, int vertexID) {
	AU_BatchEntry* ent = ctx->image_buffer + texture;
	if(ent->index_count >= ent->index_capacity) {
		ent->index_capacity *= 2;
		ent->indices = realloc(ent->indices, sizeof(int) * ent->index_capacity);
	}
	ent->indices[ent->index_count] = vertexID;
	ent->index_count++;
}

void AU_Clear(AU_Context* ctx) {
	for(int i = 0; i < ctx->tex_count; i++) {
		ctx->image_buffer[i].vertex_count = 0;
		ctx->image_buffer[i].index_count = 0;
	}
	GPU_Clear(&(ctx->target));
}

void AU_Present(AU_Context* ctx) {
	for(int i = 0; i < ctx->tex_count; i++) {
		AU_BatchEntry* ent = ctx->image_buffer + i;
		GPU_TriangleBatch(&(ent->image), &(ctx->target), ent->vertex_count, ent->vertices, ent->index_count, ent->indices, GPU_BATCH_XY_ST_RGBA);
	}
	GPU_Flip(&(ctx->target));
}
