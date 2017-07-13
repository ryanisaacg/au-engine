#include "au.h"

int main()
{
	AU_WindowConfig config = DEFAULT_CONFIG;
	AU_Engine *eng = au_init("TEST WINDOW", 800, 600, "../example/img.png", config);
	AU_Texture img = au_load_texture(eng, "../example/img.png");
	AU_Font* font = au_load_font(eng, 14, AU_WHITE, "../example/example.ttf");
	au_set_viewport(eng, au_viewport_new(AU_VIEWPORT_LETTERBOX, 16.0f / 9));
	eng->camera.height = 450;
	float x = 0, y = 0;
	AU_Vector polygon[] = {{50, 50}, {50, 100}, {100, 100}, {100, 50}};
	while(eng->should_continue) {
		au_begin(eng, AU_BLACK);
		if(eng->current_keys[SDL_SCANCODE_UP])
			y -= 1;
		if(eng->current_keys[SDL_SCANCODE_DOWN])
			y += 1;
		if(eng->current_keys[SDL_SCANCODE_RIGHT])
			x += 1;
		if(eng->current_keys[SDL_SCANCODE_LEFT])
			x -= 1;
		if(eng->mouse_left) {
			x = eng->mouse.x;
			y = eng->mouse.y;
		}
		AU_TextureRegion region = au_tex_region(img);
		au_draw_shape(eng, AU_BLUE, polygon, 4);
		au_draw_texture_ex(eng, region, AU_WHITE, 0, 0, 32, 32, 0, 0, 0, 1, 1, false, false, 0.5f);
//		au_draw_texture_ex(eng, region, AU_GREEN, x, y, 32, 32, 0, 0, 0, 1, 1, false, false, 0);
		au_draw_string(eng, font, "Hello there! test string", 0, 100);
		au_end(eng);
	}
}
