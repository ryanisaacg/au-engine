#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "tex.h"

typedef struct {
	const char* text;
	SDL_Surface* surface;
} AU_TextRenderEntry;

typedef struct {
	AU_TextRenderEntry* entries;
	size_t entry_count, entry_capacity;
} AU_TextCache;

unsigned long au_text_hash(const char* str);
AU_TextRenderEntry au_text_render(TTF_Font* font, const char* text, AU_Color color);
