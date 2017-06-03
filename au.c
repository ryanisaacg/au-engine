#include "au.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "memory.h"

AU_Engine *au_init(char* title, int w, int h) {
	AU_Engine* engine = au_memory_alloc(sizeof(AU_Engine));
	engine->ctx = au_context_init_stack(title, w, h);
	engine->fps = 60;
	return engine;
}

AU_Texture au_load_texture(AU_Engine* eng, char* name) {
	GPU_Image* image = GPU_LoadImage(name);
	if(image == NULL) {
		fprintf(stderr, "Failed to load image with filename %s", name);
		exit(1);
	}
	int id = au_context_register_texture(&(eng->ctx), image);
	return (AU_Texture) { id, image->w, image->h };
}

void au_begin(AU_Engine* eng) {
	au_context_clear(&(eng->ctx));
}

void au_end(AU_Engine* eng) {
	au_context_present(&(eng->ctx));
	SDL_Delay(1000 / eng->fps);
}

void au_draw_texture(AU_Engine* eng, AU_Texture tex, float x, float y, float w, float h) {
	AU_Context* ctx = &(eng->ctx);

	int tl = au_context_add_vertex(ctx, tex.id, x, y, 0, 0, 1, 1, 1, 1);
	int tr = au_context_add_vertex(ctx, tex.id, x + w, y, 1, 0, 1, 1, 1, 1);
	int br = au_context_add_vertex(ctx, tex.id, x + w, y + h, 1, 1, 1, 1, 1, 1);
	int bl = au_context_add_vertex(ctx, tex.id, x, y + h, 0, 1, 1, 1, 1, 1);

	au_context_add_index(ctx, tex.id, tl);
	au_context_add_index(ctx, tex.id, tr);
	au_context_add_index(ctx, tex.id, br);

	au_context_add_index(ctx, tex.id, br);
	au_context_add_index(ctx, tex.id, bl);
	au_context_add_index(ctx, tex.id, tl);
}
