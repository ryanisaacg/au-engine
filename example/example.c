#include "context.h"
#include <SDL2/SDL.h>

#undef main

int main()
{
	AU_Context *ctx = AU_Init(800, 600);
	GPU_Image* img = GPU_LoadImage("img.bmp");

	int texid = AU_RegisterTexture(ctx, img);

	AU_Clear(ctx);
	int tl = AU_AddVertex(ctx, texid, 0, 0, 0, 0, 1, 1, 1, 1);
	int tr = AU_AddVertex(ctx, texid, 32, 0, 1, 0, 1, 1, 1, 1);
	int br = AU_AddVertex(ctx, texid, 32, 32, 1, 1, 1, 1, 1, 1);
	int bl = AU_AddVertex(ctx, texid, 0, 32, 0, 1, 1, 1, 1, 1);
	
	AU_AddIndex(ctx, texid, tl);
	AU_AddIndex(ctx, texid, tr);
	AU_AddIndex(ctx, texid, br);

	AU_AddIndex(ctx, texid, tr);
	AU_AddIndex(ctx, texid, br);
	AU_AddIndex(ctx, texid, bl);

	AU_Present(ctx);

	SDL_Delay(1000);
	AU_Quit(ctx);
}
