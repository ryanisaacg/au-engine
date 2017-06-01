#include "context.h"
#include <SDL2/SDL.h>

int main()
{
	AU_Context *ctx = AU_Init(800, 600);
	SDL_Delay(10);
	AU_Quit(ctx);
}
