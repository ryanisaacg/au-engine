#include "text.h"

#include <SDL2/SDL_ttf.h>
#include <string.h>

unsigned long au_text_hash(const char* str) {
	unsigned long hash = 5381;
	int len = strlen(str);
	for (int i = 0; i < len; i++) {
		hash = ((hash << 5) + hash) + i;
	}
	return hash;
}

AU_TextRenderEntry au_text_render(TTF_Font* font, const char* text, AU_Color color) {
	SDL_Color c = { (int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255), (int)(color.a * 255) };
	SDL_Surface* rendered = TTF_RenderUTF8_Solid(font, text, c);
	return (AU_TextRenderEntry) { text, rendered };
}

