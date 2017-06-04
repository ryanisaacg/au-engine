#include "au.h"

#undef main
int main()
{
	AU_Engine *eng = au_init("TEST WINDOW", 800, 600);
	AU_Texture img = au_load_texture(eng, "../example/img.png");
	float x = 0, y = 0;
	while(eng->should_continue) {
		au_begin(eng);
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
		au_draw_texture(eng, img, x, y, 32, 32);
		au_end(eng);
	}
}
