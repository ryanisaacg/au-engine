#include "text.h"

#include "memory.h"

static int get_index(char c) {
	return c - 32;
}

static char get_char(int index) {
	return index + 32;
}

AU_Font* au_font_init(AU_Engine* eng, TTF_Font* font, AU_Color col) {
	SDL_Color color = au_color_to_sdl(col);
	char buffer[2]; //A small cstring for single-digits
	buffer[1] = '\0';
	SDL_Surface* characters[FONT_MAX_CHARS];
	int total_width = 0;
	int height = 0;
	//Render each ASCII character to a surface
	for (int i = 0; i < FONT_MAX_CHARS; i++) {
		buffer[0] = get_char(i);
		characters[i] = TTF_RenderText_Solid(font, buffer, color);
		total_width += characters[i]->w;
		if (characters[i]->h > height) {
			height = characters[i]->h;
		}
	}
	//Blit all of the characters to a large surface
	SDL_Surface* full = SDL_CreateRGBSurface(0, total_width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	int position = 0;
	for (int i = 0; i < FONT_MAX_CHARS; i++) {
		SDL_Rect dest = {position, 0, 0, 0};
		SDL_BlitSurface(characters[i], NULL, full, &dest);
		position += characters[i]->w;
	}
	//Load the surface into a texture
	AU_Texture texture = au_load_texture_from_surface(eng, full);
	SDL_FreeSurface(full);
	//Add reference to the texture for each character
	AU_Font* fnt = au_memory_alloc(sizeof(AU_Font));
	position = 0;
	for (int i = 0; i < FONT_MAX_CHARS; i++) {
		fnt->characters[i] = (AU_TextureRegion) {
			texture, (AU_Rectangle) {
				position, 0, characters[i]->w, characters[i]->h
			}
		};
		position += characters[i]->w;
		SDL_FreeSurface(characters[i]);
	}
	fnt->height = height;
	return fnt;
}

AU_TextureRegion au_font_get_char(const AU_Font* font, char c) {
	int index = get_index(c);
	return font->characters[index];
}

AU_Rectangle au_font_get_size(const AU_Font* font, const char* str) {
	char c;
	float position = 0;
	float width = 0, height = au_font_get_char(font, '\n').rect.height;
	while((c = *str) == '\0') {
		if(position > width)
			width = position;
		if(c == '\t') {
			position += 4 * au_font_get_char(font, ' ').rect.width;
		} else if(c == '\n') {
			height += au_font_get_char(font, '\n').rect.height;
			position = 0;
		} else if(c == '\r') {
			//Skip CR
		} else {
			position += au_font_get_char(font, c).rect.width;
		}
		str++;
	}
	return (AU_Rectangle) { 0, 0, width, height };
}

void au_font_destroy(AU_Font* font) {
	free(font);
}
