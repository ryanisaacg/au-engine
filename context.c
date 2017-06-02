#include "context.h"

#include <SDL2/SDL.h>

#include "memory.h"

AU_Context AU_Context_Init_Stack(char* title, int w, int h) {
	AU_Context ctx;
	ctx.target = *(GPU_Init(w, h, GPU_DEFAULT_INIT_FLAGS));
	SDL_SetWindowTitle(SDL_GetWindowFromID(ctx.target.context->windowID), title);
	ctx.tex_count = 0;
	ctx.tex_capacity = 32;
	ctx.image_buffer = chk_alloc(sizeof(AU_Context_BatchEntry) * ctx.tex_capacity);
	return ctx;
}

AU_Context* AU_Context_Init(char* title, int w, int h) {
	AU_Context* alloc_ctx = chk_alloc(sizeof(AU_Context));
	*alloc_ctx = AU_Context_Init_Stack(title, w, h);
	return alloc_ctx;
}

void AU_Context_Quit(AU_Context* ctx) {
	for(int i = 0; i < ctx->tex_capacity; i++) {
		free(ctx->image_buffer[i].vertices);
		free(ctx->image_buffer[i].indices);
	}
	free(ctx->image_buffer);

}

void AU_Context_Free(AU_Context* ctx) {
	AU_Context_Quit(ctx);
	free(ctx);
}

int AU_Context_RegisterTexture(AU_Context* ctx, GPU_Image* img) {
	if(ctx->tex_count >= ctx->tex_capacity) {
		ctx->tex_capacity *= 2;
		ctx->image_buffer = chk_realloc(ctx->image_buffer, sizeof(AU_Context_BatchEntry) * ctx->tex_capacity);
	}
	AU_Context_BatchEntry* ent = ctx->image_buffer + ctx->tex_count;
	ent->image = *img;
	ent->vertex_capacity = 1024;
	ent->vertex_count = 0;
	ent->vertices = chk_alloc(sizeof(float) * ent->vertex_capacity);
	ent->index_capacity = 64;
	ent->index_count = 0;
	ent->indices = chk_alloc(sizeof(int) * ent->index_capacity);
	ctx->tex_count++;
	return ctx->tex_count - 1;
}

int AU_Context_AddVertex(AU_Context* ctx, int texture, 
		float x, float y, float texX, float texY, float r, float g, float b, float a) {
	AU_Context_BatchEntry* ent = ctx->image_buffer + texture;
	//Reallocate if necessary
	if(ent->vertex_count >= ent->vertex_capacity) {
		ent->vertex_capacity *= 2;
		ent->vertices = chk_realloc(ent->vertices, sizeof(float) * ent->vertex_capacity);
	}
	//Pack parameters into an array and copy it to the main buffer
	float vertex[] = {x, y, texX, texY, r, g, b, a};
	memcpy(ent->vertices + ent->vertex_count * 8, vertex, sizeof(float) * 8);
	ent->vertex_count++;
	return ent->vertex_count - 1;
}

void AU_Context_AddIndex(AU_Context* ctx, int texture, int vertexID) {
	AU_Context_BatchEntry* ent = ctx->image_buffer + texture;
	//Reallocate if necessary
	if(ent->index_count >= ent->index_capacity) {
		ent->index_capacity *= 2;
		ent->indices = chk_realloc(ent->indices, sizeof(int) * ent->index_capacity);
	}
	ent->indices[ent->index_count] = vertexID;
	ent->index_count++;
}

void AU_Context_Clear(AU_Context* ctx) {
	for(int i = 0; i < ctx->tex_count; i++) {
		ctx->image_buffer[i].vertex_count = 0;
		ctx->image_buffer[i].index_count = 0;
	}
	GPU_Clear(&(ctx->target));
}

void AU_Context_Present(AU_Context* ctx) {
	for(int i = 0; i < ctx->tex_count; i++) {
		AU_Context_BatchEntry* ent = ctx->image_buffer + i;
		GPU_TriangleBatch(&(ent->image), &(ctx->target), ent->vertex_count, ent->vertices, ent->index_count, ent->indices, GPU_BATCH_XY_ST_RGBA);
	}
	GPU_Flip(&(ctx->target));
}
