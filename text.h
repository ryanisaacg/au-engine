#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "tex.h"

typedef struct {
	const char* text;
	SDL_Surface* surface;
} AU_TextRenderEntry;

unsigned long au_text_hash(const char* str);
AU_TextRenderEntry au_text_render(TTF_Font* font, const char* text, AU_Color color);

typedef struct {
	AU_TextRenderEntry* entries;
	size_t entry_count, entry_capacity;
} AU_TextCache;

AU_TextCache* au_text_cache_init();
void au_text_cache_add(AU_TextCache*, AU_TextRenderEntry);
SDL_Surface* au_text_cache_get(AU_TextCache*, const char*);
void au_text_cache_destroy(AU_TextCache*);
