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

void au_quit(AU_Engine* eng) {
	au_context_quit(&(eng->ctx));
	free(eng);
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
	int button_mask = SDL_GetMouseState(&(eng->mouse_x), &(eng->mouse_y));
	eng->mouse_left = button_mask & SDL_BUTTON(SDL_BUTTON_LEFT);
	eng->mouse_right = button_mask & SDL_BUTTON(SDL_BUTTON_RIGHT);
	eng->mouse_middle = button_mask & SDL_BUTTON(SDL_BUTTON_MIDDLE);
}

void au_end(AU_Engine* eng) {
	au_context_present(&(eng->ctx));
	SDL_Delay(1000 / eng->fps);
}

void au_draw_texture(AU_Engine* eng, AU_TextureRegion tex, float x, float y, float w, float h) {
	au_draw_texture_transform(eng, tex, au_geom_identity(), x, y, w, h);
}

void au_draw_texture_rect(AU_Engine* eng, AU_TextureRegion tex, AU_Rectangle rect) {
	au_draw_texture(eng, tex, rect.x, rect.y, rect.width, rect.height);
}

void au_draw_texture_transform(AU_Engine* eng, AU_TextureRegion tex, AU_Transform trans, float x, float y, float w,
							   float h) {
	au_draw_texture_blend(eng, tex, trans, (AU_Color) { 1, 1, 1, 1}, x, y, w, h);
}

void au_draw_texture_blend(AU_Engine* eng, AU_TextureRegion tex, AU_Transform trans, AU_Color color, float x, float y, float w, float h) {
	AU_Context* ctx = &(eng->ctx);

	//Calculate the destination points with the transformation
	AU_Vector tl = au_geom_transform(trans, (AU_Vector) {
		0, 0
	});
	AU_Vector tr = au_geom_transform(trans, (AU_Vector) {
		w, 0
	});
	AU_Vector bl = au_geom_transform(trans, (AU_Vector) {
		0, h
	});
	AU_Vector br = au_geom_vec_add(tr, bl);	

	//Calculate the source points normalized to [0, 1]
	//The conversion factor for normalizing vectors
	float conv_factor_x = 1 / tex.rect.width;
	float conv_factor_y = 1 / tex.rect.height;
	float norm_x = tex.rect.x * conv_factor_x;
	float norm_y = tex.rect.y * conv_factor_y;
	float norm_w = tex.rect.width * conv_factor_x;
	float norm_h = tex.rect.height * conv_factor_y;
	AU_Vector src_tl = { norm_x, norm_y };
	AU_Vector src_tr = { norm_x + norm_w, norm_y };
	AU_Vector src_br = { norm_x + norm_w, norm_y + norm_h };
	AU_Vector src_bl = { norm_x, norm_y + norm_h };

	//Add all of the vertices to the context
	int id = tex.source.id;
	int tl_index = au_context_add_vertex(ctx, id, tl.x + x, tl.y + y, src_tl.x, src_tl.y, color.r, color.g, color.b, color.a);
	int tr_index = au_context_add_vertex(ctx, id, tr.x + x, tr.y + y, src_tr.x, src_tr.y, color.r, color.g, color.b, color.a);
	int br_index = au_context_add_vertex(ctx, id, br.x + x, br.y + y, src_br.x, src_br.y, color.r, color.g, color.b, color.a);
	int bl_index = au_context_add_vertex(ctx, id, bl.x + x, bl.y + y, src_bl.x, src_bl.y, color.r, color.g, color.b, color.a);

	//Create the first triangle for the quad
	au_context_add_index(ctx, id, tl_index);
	au_context_add_index(ctx, id, tr_index);
	au_context_add_index(ctx, id, br_index);
	//Create the second triangle for the quad
	au_context_add_index(ctx, id, br_index);
	au_context_add_index(ctx, id, bl_index);
	au_context_add_index(ctx, id, tl_index);

}
