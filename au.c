#include "au.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "memory.h"

AU_Engine* au_init(char* title, int w, int h) {
	AU_Engine* engine = au_memory_alloc(sizeof(AU_Engine));
	engine->ctx = au_context_init_stack(title, w, h);
	engine->fps = 60;
	engine->should_continue = true;
	memset(engine->current_keys, 0, sizeof(bool) * SDL_NUM_KEYS);
	return engine;
}

AU_Texture au_load_texture(AU_Engine* eng, char* name) {
	GPU_Image* image = GPU_LoadImage(name);
	if (image == NULL) {
		fprintf(stderr, "Failed to load image with filename %s", name);
		exit(1);
	}
	int id = au_context_register_texture(&(eng->ctx), image);
	return (AU_Texture) {
		id, image->w, image->h
	};
}

void au_begin(AU_Engine* eng) {
	au_context_clear(&(eng->ctx));
	memcpy(eng->previous_keys, eng->current_keys, sizeof(bool) * SDL_NUM_KEYS);
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT:
				eng->should_continue = false;
				break;
			case SDL_KEYDOWN:
				eng->current_keys[e.key.keysym.scancode] = true;
				break;
			case SDL_KEYUP:
				eng->current_keys[e.key.keysym.scancode] = false;
				break;
		}
	}
}

void au_end(AU_Engine* eng) {
	au_context_present(&(eng->ctx));
	SDL_Delay(1000 / eng->fps);
}

void au_draw_texture(AU_Engine* eng, AU_Texture tex, float x, float y, float w, float h) {
	au_draw_texture_transform(eng, tex, au_geom_identity(), x, y, w, h);
}

void au_draw_texture_transform(AU_Engine* eng, AU_Texture tex, AU_Transform trans, float x, float y, float w, float h) {
	AU_Context* ctx = &(eng->ctx);

	AU_Vector tl = au_geom_transform(trans, (AU_Vector) {
		0, 0
	});
	AU_Vector tr = au_geom_transform(trans, (AU_Vector) {
		w, 0
	});
	AU_Vector br = au_geom_transform(trans, (AU_Vector) {
		w, h
	});
	AU_Vector bl = au_geom_transform(trans, (AU_Vector) {
		0, h
	});

	int tl_index = au_context_add_vertex(ctx, tex.id, tl.x + x, tl.y + y, 0, 0, 1, 1, 1, 1);
	int tr_index = au_context_add_vertex(ctx, tex.id, tr.x + x, tr.y + y, 1, 0, 1, 1, 1, 1);
	int br_index = au_context_add_vertex(ctx, tex.id, br.x + x, br.y + y, 1, 1, 1, 1, 1, 1);
	int bl_index = au_context_add_vertex(ctx, tex.id, bl.x + x, br.y + y, 0, 1, 1, 1, 1, 1);

	au_context_add_index(ctx, tex.id, tl_index);
	au_context_add_index(ctx, tex.id, tr_index);
	au_context_add_index(ctx, tex.id, br_index);

	au_context_add_index(ctx, tex.id, br_index);
	au_context_add_index(ctx, tex.id, bl_index);
	au_context_add_index(ctx, tex.id, tl_index);
}
