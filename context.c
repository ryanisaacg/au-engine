#include "context.h"

#include <stdlib.h>

static void init_batch_entry(AU_BatchEntry*, GPU_Image*);

AU_Context* AU_Init(int w, int h) {
	AU_Context ctx;
	ctx.target = *(GPU_Init(w, h, GPU_DEFAULT_INIT_FLAGS));
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
	ent->vertex_capacity = 0;
	ent->vertices = malloc(sizeof(float) * ent->vertex_capacity);
	ent->index_capacity = 64;
	ent->index_count = 0;
	ent->indices = malloc(sizeof(int) * ent->index_capacity);
}
