#include "au.h"

int main()
{
	AU_Engine *eng = au_init("TEST WINDOW", 800, 600, "../example/img.png");
	AU_Texture img = au_load_texture(eng, "../example/img.png");
	AU_Font* font = au_load_font(eng, 14, AU_WHITE, "../example/example.ttf");
	float x = 0, y = 0;
	while(eng->should_continue) {
		au_begin(eng, AU_BLACK);
		if(eng->current_keys[SDL_SCANCODE_UP])
			y -= 2;
		if(eng->current_keys[SDL_SCANCODE_DOWN])
			y += 2;
		if(eng->current_keys[SDL_SCANCODE_RIGHT])
			x += 2;
		if(eng->current_keys[SDL_SCANCODE_LEFT])
			x -= 2;
		if(eng->mouse_left) {
			x = eng->mouse_x;
			y = eng->mouse_y;
		}
		AU_TextureRegion region = au_tex_region(img);
		region.rect = (AU_Rectangle) {8, 8, 16, 16};
		au_draw_texture_ex(eng, region, AU_WHITE, 0, 0, 32, 32, 0, 0, 0, 1, 1, true, false, 0);
		au_draw_texture_ex(eng, region, AU_WHITE, x, y, 32, 32, 0, 0, 0, 1, 1, true, false, -50);
		au_draw_string(eng, font, "Hello there! test string", 0, 100);
		au_end(eng);
	}
}
