#include "context.h"
#include <SDL2/SDL.h>

#undef main

void draw(AU_Context *ctx, int texid, float xOffset) {
	int tl = AU_AddVertex(ctx, texid, xOffset, 0, 0, 0, 1, 1, 1, 1);
	int tr = AU_AddVertex(ctx, texid, xOffset + 32, 0, 1, 0, 1, 1, 1, 1);
	int br = AU_AddVertex(ctx, texid, xOffset + 32, 32, 1, 1, 1, 1, 1, 1);
	int bl = AU_AddVertex(ctx, texid, xOffset, 32, 0, 1, 1, 1, 1, 1);
	
	AU_AddIndex(ctx, texid, tl);
	AU_AddIndex(ctx, texid, tr);
	AU_AddIndex(ctx, texid, br);

	AU_AddIndex(ctx, texid, br);
	AU_AddIndex(ctx, texid, bl);
	AU_AddIndex(ctx, texid, tl);
}

int main()
{
	AU_Context *ctx = AU_Init("TEST WINDOW", 800, 600);
	GPU_Image* img = GPU_LoadImage("img.png");
	GPU_Image* img2 = GPU_LoadImage("img2.png");

	int texid1 = AU_RegisterTexture(ctx, img);
	int texid2 = AU_RegisterTexture(ctx, img2);

	AU_Clear(ctx);
	draw(ctx, texid1, 0);
	draw(ctx, texid2, 32);
	draw(ctx, texid1, 64);
	draw(ctx, texid2, 96);
	AU_Present(ctx);

	SDL_Delay(1000);
	AU_Quit(ctx);
}
