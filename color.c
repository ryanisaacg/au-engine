#include "color.h" 

SDL_Color au_color_to_sdl(AU_Color c) {
	return (SDL_Color) {
		(int)(255 * c.r), (int)(255 * c.g), (int)(255 * c.b), (int)(255 * c.a)
	};
}
