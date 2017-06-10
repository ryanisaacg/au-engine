#include "texture.h"

AU_TextureRegion au_tex_region(AU_Texture tex) {
	return (AU_TextureRegion) {
		tex, (AU_Rectangle) {
			0, 0, tex.width, tex.height
		}
	};
}

SDL_Color au_color_to_sdl(AU_Color c) {
	return (SDL_Color) {
		(int)(255 * c.r), (int)(255 * c.g), (int)(255 * c.b), (int)(255 * c.a)
	};
}
