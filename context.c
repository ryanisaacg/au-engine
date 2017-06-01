#include "context.h"

#include <stdlib.h>

AU_Context* AU_Init(int w, int h) {
	AU_Context ctx;
	ctx.target = *(GPU_Init(w, h, GPU_DEFAULT_INIT_FLAGS));
	ctx.tex_count = 0;
	ctx.tex_capacity = 32;
	ctx.image_buffer = malloc(sizeof(AU_BatchEntry) * ctx.tex_capacity);
	for(int i = 0; i < ctx.tex_capacity; i++) {
		ctx.image_buffer[i].vertex_capacity = 1024;
		ctx.image_buffer[i].vertex_capacity = 0;
		ctx.image_buffer[i].vertices = malloc(sizeof(float) * ctx.image_buffer[i].vertex_capacity);
		ctx.image_buffer[i].index_capacity = 64;
		ctx.image_buffer[i].index_count = 0;
		ctx.image_buffer[i].indices = malloc(sizeof(int) * ctx.image_buffer[i].index_capacity);
	}
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
