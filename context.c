#include "context.h"

#include <SDL2/SDL.h>

#include "memory.h"

AU_Context au_context_init_stack(char* title, int w, int h) {
	AU_Context ctx;
	ctx.target = *(GPU_Init(w, h, GPU_DEFAULT_INIT_FLAGS));
	SDL_SetWindowTitle(SDL_GetWindowFromID(ctx.target.context->windowID), title);
	ctx.tex_count = 0;
	ctx.tex_capacity = 32;
	ctx.image_buffer = au_memory_alloc(sizeof(AU_BatchEntry) * ctx.tex_capacity);
	return ctx;
}

AU_Context* au_context_init(char* title, int w, int h) {
	AU_Context* alloc_ctx = au_memory_alloc(sizeof(AU_Context));
	*alloc_ctx = au_context_init_stack(title, w, h);
	return alloc_ctx;
}

void au_context_quit(AU_Context* ctx) {
	for (int i = 0; i < ctx->tex_capacity; i++) {
		free(ctx->image_buffer[i].vertices);
		free(ctx->image_buffer[i].indices);
	}
	free(ctx->image_buffer);

}

void au_context_free(AU_Context* ctx) {
	au_context_quit(ctx);
	free(ctx);
}

int au_context_register_texture(AU_Context* ctx, GPU_Image* img) {
	if (ctx->tex_count >= ctx->tex_capacity) {
		ctx->tex_capacity *= 2;
		ctx->image_buffer = au_memory_realloc(ctx->image_buffer, sizeof(AU_BatchEntry) * ctx->tex_capacity);
	}
	AU_BatchEntry* ent = ctx->image_buffer + ctx->tex_count;
	ent->image = *img;
	ent->vertex_capacity = 1024;
	ent->vertex_count = 0;
	ent->vertices = au_memory_alloc(sizeof(float) * ent->vertex_capacity);
	ent->index_capacity = 64;
	ent->index_count = 0;
	ent->indices = au_memory_alloc(sizeof(int) * ent->index_capacity);
	ctx->tex_count++;
	return ctx->tex_count - 1;
}

int au_context_add_vertex(AU_Context* ctx, int texture,
						  float x, float y, float texX, float texY, float r, float g, float b, float a) {
	AU_BatchEntry* ent = ctx->image_buffer + texture;
	//Reallocate if necessary
	if (ent->vertex_count >= ent->vertex_capacity) {
		ent->vertex_capacity *= 2;
		ent->vertices = au_memory_realloc(ent->vertices, sizeof(float) * ent->vertex_capacity);
	}
	//Pack parameters into an array and copy it to the main buffer
	float vertex[] = {x, y, texX, texY, r, g, b, a};
	memcpy(ent->vertices + ent->vertex_count * 8, vertex, sizeof(float) * 8);
	ent->vertex_count++;
	return ent->vertex_count - 1;
}

void au_context_add_index(AU_Context* ctx, int texture, int vertexID) {
	AU_BatchEntry* ent = ctx->image_buffer + texture;
	//Reallocate if necessary
	if (ent->index_count >= ent->index_capacity) {
		ent->index_capacity *= 2;
		ent->indices = au_memory_realloc(ent->indices, sizeof(int) * ent->index_capacity);
	}
	ent->indices[ent->index_count] = vertexID;
	ent->index_count++;
}

void au_context_clear(AU_Context* ctx) {
	for (int i = 0; i < ctx->tex_count; i++) {
		ctx->image_buffer[i].vertex_count = 0;
		ctx->image_buffer[i].index_count = 0;
	}
	GPU_Clear(&(ctx->target));
}

void au_context_present(AU_Context* ctx) {
	for (int i = 0; i < ctx->tex_count; i++) {
		AU_BatchEntry* ent = ctx->image_buffer + i;
		GPU_TriangleBatch(&(ent->image), &(ctx->target), ent->vertex_count, ent->vertices, ent->index_count, ent->indices, GPU_BATCH_XY_ST_RGBA);
	}
	GPU_Flip(&(ctx->target));
}
