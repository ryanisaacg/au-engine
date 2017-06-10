#include "text.h"

#include <SDL2/SDL_ttf.h>
#include <string.h>

#include "memory.h"

unsigned long au_text_hash(const char* str) {
	unsigned long hash = 5381;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		hash = ((hash << 5) + hash) + i;
	}
	return hash;
}

AU_TextRenderEntry au_text_render(AU_Font* font, const char* text, AU_Color color) {
	SDL_Color c = { (int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255), (int)(color.a * 255) };
	SDL_Surface* rendered = TTF_RenderUTF8_Solid(font, text, c);
	return (AU_TextRenderEntry) { text, rendered };
}

AU_TextCache* au_text_cache_init() {
	AU_TextCache* cache = au_memory_alloc(sizeof(AU_TextCache));
	cache->entry_count = 0;
	cache->entry_capacity = 128;
	cache->entries = au_memory_calloc(cache->entry_capacity, sizeof(AU_TextRenderEntry));
	return cache;
}

static int probe_index(AU_TextCache* cache, const char* key) {
	unsigned long hash = au_text_hash(key);
	int index = hash % cache->entry_capacity;
	while(cache->entries[index].text != NULL) 
		index++;
	return index;
}

void au_text_cache_add(AU_TextCache* cache, AU_TextRenderEntry entry) {
	if(cache->entry_count >= cache->entry_capacity) {
		int old_capacity = cache->entry_capacity;
		cache->entry_capacity *= 2;
		AU_TextRenderEntry* prev = cache->entries;
		cache->entries = au_memory_calloc(cache->entry_capacity, sizeof(AU_TextRenderEntry));
		for(int i = 0; i < old_capacity; i++) {
			if(prev[i].text != NULL) 
				au_text_cache_add(cache, prev[i]);
		}
	}
	cache->entry_count++;
	int i = probe_index(cache, entry.text);
	cache->entries[i] = entry;
}

SDL_Surface* au_text_cache_get(AU_TextCache* cache, const char* key) {
	return cache->entries[probe_index(cache, key)].surface;
}

void au_text_cache_destroy(AU_TextCache* cache) {
	free(cache->entries);
	free(cache);
}
