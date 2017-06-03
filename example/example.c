#include "au.h"

#undef main
int main()
{
	AU_Engine *eng = au_init("TEST WINDOW", 800, 600);
	AU_Texture img = au_load_texture(eng, "../example/img.png");
	AU_Texture img2 = au_load_texture(eng, "../example/img2.png");
	while(eng->should_continue) {
		au_begin(eng);
		au_draw_texture(eng, img, 0, 0, 32, 32);
		au_draw_texture(eng, img2, 32, 0, 32, 32);
		au_draw_texture(eng, img, 64, 0, 32, 32);
		au_draw_texture(eng, img2, 96, 0, 32, 32);
		au_end(eng);
	}
}
