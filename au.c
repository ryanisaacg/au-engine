#include "au.h"

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>

#include "memory.h"

AU_Engine* au_init(char* title, int w, int h) {
	AU_Engine* engine = au_memory_alloc(sizeof(AU_Engine));
	engine->ctx = au_context_init_stack(title, w, h);
	engine->fps = 60;
	engine->should_continue = true;
	memset(engine->current_keys, 0, sizeof(bool) * SDL_NUM_KEYS);
	engine->particle_capacity = 128;
	engine->particles = au_memory_alloc(sizeof(AU_Particle) * engine->particle_capacity);
	engine->particle_count = 0;

	TTF_Init(); //initialize the SDL font subsystem

	srand(time(NULL));

	return engine;
}

void au_quit(AU_Engine* eng) {
	au_context_quit(&(eng->ctx));

	TTF_Quit(); //Destroy the SDL font subsystem

	free(eng);
}

AU_Texture au_load_texture(AU_Engine* eng, const char* name) {
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

AU_Texture au_load_texture_from_surface(AU_Engine* eng, SDL_Surface* sur) {
	GPU_Image* image = GPU_CopyImageFromSurface(sur);
	if (image == NULL) {
		fprintf(stderr, "Failed to load image from surface");
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
	for (size_t i = 0; i < eng->particle_count; i++) {
		AU_Particle* part = eng->particles + i;
		au_particle_update(part);
		if (part->lifetime <= 0) {
			eng->particles[i] = eng->particles[eng->particle_count - 1];
			eng->particle_count--;
			i--;
		} else {
			AU_Sprite sprite = au_sprite_new(part->region);
			sprite.transform.x = part->position.x;
			sprite.transform.y = part->position.y;
			au_draw_sprite(eng, &sprite);
		}
	}
	au_context_present(&(eng->ctx));
	SDL_Delay(1000 / eng->fps);
}

void au_draw_texture(AU_Engine* eng, AU_TextureRegion tex, float x, float y) {
	au_draw_texture_sized(eng, tex, x, y, tex.rect.width, tex.rect.height);
}

void au_draw_texture_sized(AU_Engine* eng, AU_TextureRegion tex, float x, float y, float w, float h) {
	au_draw_texture_transform(eng, tex, au_geom_identity(), x, y, w, h);
}

void au_draw_texture_rect(AU_Engine* eng, AU_TextureRegion tex, AU_Rectangle rect) {
	au_draw_texture_sized(eng, tex, rect.x, rect.y, rect.width, rect.height);
}

void au_draw_texture_transform(AU_Engine* eng, AU_TextureRegion tex, AU_Transform trans, float x, float y, float w,
							   float h) {
	au_draw_texture_blend(eng, tex, (AU_Color) {
		1, 1, 1, 1
	}, trans, x, y, w, h, false, false);
}

void au_draw_texture_ex(AU_Engine* eng, AU_TextureRegion tex, AU_Color color, float x, float y, float w, float h,
						float rot, float or_x, float or_y, float scale_x, float scale_y, bool flip_x, bool flip_y) {
	AU_Transform trans = au_geom_identity();
	trans = au_geom_transform_concat(trans, au_geom_transform_rotate(rot));
	trans = au_geom_transform_concat(trans, au_geom_transform_scale(scale_x, scale_y));
	trans = au_geom_transform_concat(trans, au_geom_transform_translate(x, y));
	au_draw_texture_blend(eng, tex, color, trans, or_x, or_y, w, h, flip_x, flip_y);
}

void au_draw_texture_blend(AU_Engine* eng, AU_TextureRegion tex, AU_Color color, AU_Transform trans, float x, float y,
						   float w, float h, bool flip_x, bool flip_y) {
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
	AU_Vector br = au_geom_transform(trans, (AU_Vector) {
		w, h
	});

	//Calculate the source points normalized to [0, 1]
	//The conversion factor for normalizing vectors
	float conv_factor_x = 1.0f / tex.source.width;
	float conv_factor_y = 1.0f / tex.source.height;
	float norm_x = tex.rect.x * conv_factor_x;
	float norm_y = tex.rect.y * conv_factor_y;
	float norm_w = tex.rect.width * conv_factor_x;
	float norm_h = tex.rect.height * conv_factor_y;
	AU_Vector src_tl = { norm_x, norm_y };
	AU_Vector src_tr = { norm_x + norm_w, norm_y };
	AU_Vector src_br = { norm_x + norm_w, norm_y + norm_h };
	AU_Vector src_bl = { norm_x, norm_y + norm_h };
	if (flip_x) {
		AU_Vector tmp = src_tr;
		src_tr = src_tl;
		src_tl = tmp;
		tmp = src_br;
		src_br = src_bl;
		src_bl = tmp;
	}
	if (flip_y) {
		AU_Vector tmp = src_tr;
		src_tr = src_br;
		src_br = tmp;
		tmp = src_tl;
		src_tl = src_bl;
		src_bl = tmp;
	}
	//Add all of the vertices to the context
	int id = tex.source.id;
	int tl_index = au_context_add_vertex(ctx, id, tl.x + x, tl.y + y, src_tl.x, src_tl.y, color.r, color.g, color.b,
										 color.a);
	int tr_index = au_context_add_vertex(ctx, id, tr.x + x, tr.y + y, src_tr.x, src_tr.y, color.r, color.g, color.b,
										 color.a);
	int br_index = au_context_add_vertex(ctx, id, br.x + x, br.y + y, src_br.x, src_br.y, color.r, color.g, color.b,
										 color.a);
	int bl_index = au_context_add_vertex(ctx, id, bl.x + x, bl.y + y, src_bl.x, src_bl.y, color.r, color.g, color.b,
										 color.a);

	//Create the first triangle for the quad
	au_context_add_index(ctx, id, tl_index);
	au_context_add_index(ctx, id, tr_index);
	au_context_add_index(ctx, id, br_index);
	//Create the second triangle for the quad
	au_context_add_index(ctx, id, br_index);
	au_context_add_index(ctx, id, bl_index);
	au_context_add_index(ctx, id, tl_index);
}

static void au_draw_sprite_transformed(AU_Engine* eng, AU_TextureRegion region, AU_SpriteTransform* trans) {
	au_draw_texture_ex(eng, region, trans->color, trans->x, trans->y, trans->width, trans->height, trans->rotation,
					   trans->origin_x, trans->origin_y, trans->scale_x, trans->scale_y, trans->flip_x, trans->flip_y);
}

void au_draw_sprite(AU_Engine* eng, AU_Sprite* sprite) {
	au_draw_sprite_transformed(eng, sprite->region, &(sprite->transform));
}

void au_draw_sprite_animated(AU_Engine* eng, AU_AnimatedSprite* sprite) {
	au_anim_manager_update(&(sprite->animations));
	AU_TextureRegion region = au_anim_manager_get_frame(&(sprite->animations));
	au_draw_sprite_transformed(eng, region, &(sprite->transform));
}

AU_Font* au_load_font(AU_Engine* eng, int size, AU_Color col, const char* filename) {
	TTF_Font* font = TTF_OpenFont(filename, size);
	if (font == NULL) {
		fprintf(stderr, "Font with filename %s not found\n", filename);
		exit(1);
	}
	AU_Font* bitmap_font = au_font_init(eng, font, col);
	TTF_CloseFont(font);
	return bitmap_font;
}

int au_draw_char(AU_Engine* eng, AU_Font* font, char c, float x, float y) {
	AU_TextureRegion renderChar = au_font_get_char(font, c);
	au_draw_texture(eng, renderChar, x, y);
	return renderChar.rect.width;
}

void au_draw_string(AU_Engine* eng, AU_Font* font, const char* str, float x, float y) {
	char c;
	int position = 0;
	//Loop from the beginning to end of the string
	while ((c = *str) != '\0') {
		if (c == '\t') {
			for (int i = 0; i < 4; i++) {
				position += au_draw_char(eng, font, ' ', position + x, y);
			}
		} else if (c == '\n') {
			y += font->height;
		} else if (c == '\r') {
			//just ignore CR characters
		} else {
			position += au_draw_char(eng, font, c, position + x, y);
		}
		str++;
	}
}

void au_add_particles(AU_Engine* eng, AU_ParticleEmitter* emitter) {
	int parts = au_util_randi_range(emitter->particle_min, emitter->particle_max);
	if (eng->particle_count + parts >= eng->particle_capacity) {
		eng->particle_capacity *= 2;
		eng->particles = au_memory_realloc(eng->particles, sizeof(AU_Particle) * eng->particle_capacity);
	}
	for (int i = 0; i < parts; i++) {
		eng->particles[eng->particle_count] = au_particle_emitter_emit(emitter);
		eng->particle_count++;
	}
}
