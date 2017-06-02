#include "context.h"
#include <SDL2/SDL.h>
#include <stdio.h>

#undef main

void draw(AU_Context *ctx, int texid, float xOffset) {
	int tl = au_context_add_vertex(ctx, texid, xOffset, 0, 0, 0, 1, 1, 1, 1);
	int tr = au_context_add_vertex(ctx, texid, xOffset + 32, 0, 1, 0, 1, 1, 1, 1);
	int br = au_context_add_vertex(ctx, texid, xOffset + 32, 32, 1, 1, 1, 1, 1, 1);
	int bl = au_context_add_vertex(ctx, texid, xOffset, 32, 0, 1, 1, 1, 1, 1);

	au_context_add_index(ctx, texid, tl);
	au_context_add_index(ctx, texid, tr);
	au_context_add_index(ctx, texid, br);

	au_context_add_index(ctx, texid, br);
	au_context_add_index(ctx, texid, bl);
	au_context_add_index(ctx, texid, tl);
}

int main()
{
	AU_Context *ctx = au_context_init("TEST WINDOW", 800, 600);
	GPU_Image* img = GPU_LoadImage("img.png");
	if(img == NULL) {
		fprintf(stderr, "Failed to load image\n");
		exit(1);
	}
	GPU_Image* img2 = GPU_LoadImage("img2.png");
	if(img2 == NULL) {
		fprintf(stderr, "Failed to load image\n");
		exit(1);
	}

	int texid1 = au_context_register_texture(ctx, img);
	int texid2 = au_context_register_texture(ctx, img2);

	au_context_clear(ctx);
	draw(ctx, texid1, 0);
	draw(ctx, texid2, 32);
	draw(ctx, texid1, 64);
	draw(ctx, texid2, 96);
	au_context_present(ctx);

	SDL_Delay(1000);
	au_context_free(ctx);
}
